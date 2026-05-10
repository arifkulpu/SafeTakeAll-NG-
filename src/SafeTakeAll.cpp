#include "SafeTakeAll.h"

namespace SafeTakeAll
{
    // Re-entry guard: prevents recursive calls during transfer on the same thread
    static thread_local bool g_isTransferring = false;

    struct ExtractLootHook
    {
        // Completely replaces PlayerCharacter::ExtractLoot.
        // We do NOT chain back to the original — the game only calls this
        // function from ContainerMenu context anyway, so the silent return
        // for other cases is safe and avoids any trampoline/address issues.
        static void thunk(RE::PlayerCharacter* a_this, RE::TESObjectREFR* a_container)
        {
            if (!a_this || !a_container) return;

            // Safety: Ensure we only trigger for the player to avoid affecting followers
            if (a_this != RE::PlayerCharacter::GetSingleton()) return;

            // Re-entry guard
            if (g_isTransferring) return;

            auto ui = RE::UI::GetSingleton();
            if (!ui || !ui->IsMenuOpen(RE::ContainerMenu::MENU_NAME)) return;

            g_isTransferring = true;
            try {
                ExecuteSafeTransfer(a_this, a_container);
            } catch (...) {
                spdlog::error("SafeTakeAll: Critical error during transfer.");
            }
            g_isTransferring = false;
        }

        static void ExecuteSafeTransfer(RE::PlayerCharacter* a_player, RE::TESObjectREFR* a_container)
        {
            if (!a_player || !a_container) return;

            auto inventory = a_container->GetInventory();
            struct SnapshotEntry {
                RE::TESBoundObject* obj;
                std::int32_t count;
                RE::ExtraDataList* extraList;
            };
            std::vector<SnapshotEntry> snapshot;

            for (auto& [object, data] : inventory) {
                if (!object) continue;
                
                auto& [count, entry] = data;
                if (count <= 0) continue;

                // Skip non-playable items
                if (!object->GetPlayable()) continue;

                // Skip quest items
                if (entry && entry->IsQuestObject()) continue;

                // Extract ExtraDataList to preserve item properties and prevent skee64 access violations
                RE::ExtraDataList* xList = nullptr;
                if (entry && entry->extraLists && !entry->extraLists->empty()) {
                    xList = entry->extraLists->front();
                }

                snapshot.push_back({ object, count, xList });
            }

            if (snapshot.empty()) return;

            for (auto& item : snapshot) {
                // Use kStoreInContainer to silently bypass MuJointFix and TNG recursive hooks
                // Pass item.extraList to ensure RaceMenu (skee64.dll) doesn't access violate on null data
                a_container->RemoveItem(
                    item.obj, 
                    item.count, 
                    RE::ITEM_REMOVE_REASON::kStoreInContainer, 
                    item.extraList, 
                    a_player
                );
            }

            spdlog::info("SafeTakeAll: Successfully processed {} stacks.", snapshot.size());
        }

        static inline REL::Relocation<decltype(thunk)> func;
    };

    void Install()
    {
        auto& trampoline = SKSE::GetTrampoline();

        // PlayerCharacter::ExtractLoot — SE: 39546 / AE: 40632
        REL::Relocation<std::uintptr_t> target{ REL::RelocationID(39546, 40632) };

        // write_branch<5>: patches a JMP at the function entry.
        // We intentionally discard the return value (trampoline stub) because
        // we do NOT call back into the original — this is the safe pattern
        // used by the original working version of this plugin.
        trampoline.write_branch<5>(target.address(),
            reinterpret_cast<std::uintptr_t>(ExtractLootHook::thunk));

        spdlog::info("SafeTakeAll: Hook installed at 0x{:X}.", target.address());
    }
}
