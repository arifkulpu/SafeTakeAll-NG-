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

            auto inventory = a_container->GetInventory();
            if (inventory.empty()) return;

            std::vector<std::pair<RE::TESBoundObject*, std::int32_t>> itemsToTake;

            for (auto& [obj, itemData] : inventory) {
                if (obj && itemData.first > 0) {
                    // Skip non-playable items
                    if (!obj->GetPlayable()) continue;

                    // Skip quest items
                    if (itemData.second && itemData.second->IsQuestObject()) continue;

                    itemsToTake.push_back({ obj, itemData.first });
                }
            }

            if (itemsToTake.empty()) return;

            for (auto& [obj, count] : itemsToTake) {
                try {
                    // We use the standard engine transfer logic.
                    // The g_isTransferring recursion guard at the hook level
                    // prevents other mods' hooks from causing a loop back into ExtractLoot.
                    a_container->RemoveItem(obj, count, RE::ITEM_REMOVE_REASON::kRemove, nullptr, a_player);
                } catch (...) {
                    continue;
                }
            }

            spdlog::info("SafeTakeAll: Successfully processed {} stacks.", itemsToTake.size());
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
