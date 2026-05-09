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

            // Get a snapshot of the inventory to avoid iterator invalidation or pointer issues.
            // Using a map first to aggregate counts, then a vector of FormIDs for safety.
            auto inventory = a_container->GetInventory();
            if (inventory.empty()) return;

            struct SnapshotEntry {
                RE::FormID formID;
                std::int32_t count;
            };
            std::vector<SnapshotEntry> snapshot;

            for (auto& [object, data] : inventory) {
                if (!object) continue;
                
                auto& [count, entry] = data;
                if (count <= 0) continue;

                // Skip non-playable items (like hidden outfits or engine items)
                if (!object->GetPlayable()) continue;

                // Skip quest items
                if (entry && entry->IsQuestObject()) continue;

                snapshot.push_back({ object->GetFormID(), count });
            }

            if (snapshot.empty()) return;

            // Use a handle to track the container's validity throughout the loop
            auto containerHandle = a_container->GetHandle();

            for (const auto& item : snapshot) {
                // Verify container still exists and player is valid
                if (!containerHandle || !a_player) break;
                auto* currentContainer = containerHandle.get().get();
                if (!currentContainer) break;

                // Lookup the object by ID to ensure it hasn't been deleted
                auto* obj = RE::TESForm::LookupByID<RE::TESBoundObject>(item.formID);
                if (!obj) continue;

                try {
                    // Use reason 3 (kDropping) for better compatibility with mods like TNG and ImmersiveWeaponSwitch.
                    // These mods often hook kRemove (0) to trigger heavy mesh/behavior updates, 
                    // which can lead to stack overflows during mass transfers.
                    currentContainer->RemoveItem(
                        obj, 
                        item.count, 
                        static_cast<RE::ITEM_REMOVE_REASON>(3), 
                        nullptr, 
                        a_player
                    );
                } catch (...) {
                    // Silently continue to the next item if one fails
                }
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
