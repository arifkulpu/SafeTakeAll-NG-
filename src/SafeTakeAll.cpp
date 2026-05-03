#include "SafeTakeAll.h"

namespace SafeTakeAll
{
    // Atomic re-entry guard: prevents recursive calls during transfer
    static std::atomic<bool> g_isTransferring = false;

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
            if (g_isTransferring.load()) return;

            // Only handle ContainerMenu (chest / corpse loot)
            // For barter / gift menus the game uses different code paths,
            // so silently returning here causes no side effects.
            auto ui = RE::UI::GetSingleton();
            if (!ui || !ui->IsMenuOpen(RE::ContainerMenu::MENU_NAME)) return;

            spdlog::info("SafeTakeAll: Take All triggered on '{}'.", a_container->GetName());

            g_isTransferring.store(true);
            ExecuteSafeTransfer(a_this, a_container);
            g_isTransferring.store(false);
        }

        static void ExecuteSafeTransfer(RE::PlayerCharacter* a_player, RE::TESObjectREFR* a_container)
        {
            if (!a_player || !a_container) return;

            // Build a snapshot BEFORE moving anything.
            // Iterating and modifying the live list simultaneously causes
            // the "hundreds of items" duplication bug, so we snapshot first.
            struct Entry { RE::TESBoundObject* obj; std::int32_t count; };
            std::vector<Entry> snapshot;

            auto inventory = a_container->GetInventory();
            for (auto& [object, data] : inventory) {
                auto& [count, entry] = data;
                if (!object || count <= 0) continue;
                if (entry && entry->IsQuestObject()) continue;  // never steal quest items
                if (!object->GetPlayable()) continue;           // skip hidden/internal items
                snapshot.push_back({ object, count });
            }

            // Move every stack as one atomic call — no ExtraList iteration.
            // Passing nullptr for extraList lets the engine move the whole stack
            // (including all enchantment / condition data) in one safe operation.
            for (const auto& e : snapshot) {
                a_container->RemoveItem(e.obj, e.count,
                    RE::ITEM_REMOVE_REASON::kRemove, nullptr, a_player);
            }

            spdlog::info("SafeTakeAll: Transferred {} stacks.", snapshot.size());
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
