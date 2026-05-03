#include "SafeTakeAll.h"

namespace SafeTakeAll
{
    std::atomic<bool> g_isTransferring = false;

    struct ExtractLootHook
    {
        // Hook thunk: PlayerCharacter::ExtractLoot(RE::TESObjectREFR* a_container)
        static void thunk(RE::PlayerCharacter* a_this, RE::TESObjectREFR* a_container)
        {
            if (!a_this || !a_container) return;

            // 1. Sadece ContainerMenu açıkken bizim güvenli aktarım mantığımız çalışsın
            auto ui = RE::UI::GetSingleton();
            if (ui && ui->IsMenuOpen(RE::ContainerMenu::MENU_NAME)) {
                if (!g_isTransferring.load()) {
                    ExecuteSafeTransfer(a_this, a_container);
                    return;
                }
            }

            // 2. Diğer tüm durumlarda (veya işlem zaten devam ediyorsa) orijinal fonksiyonu çağır
            // Bu, diğer menülerin veya sistemlerin bozulmasını engeller.
            func(a_this, a_container);
        }

        static void ExecuteSafeTransfer(RE::PlayerCharacter* a_player, RE::TESObjectREFR* a_container)
        {
            if (!a_player || !a_container) return;

            g_isTransferring.store(true);
            
            // Konteyner envanterinin anlık görüntüsünü al
            auto inventory = a_container->GetInventory();
            if (inventory.empty()) {
                g_isTransferring.store(false);
                return;
            }

            uint32_t totalStacks = 0;
            uint32_t itemsMoved = 0;

            for (auto& [object, data] : inventory) {
                auto& [count, entry] = data;
                
                if (!object || count <= 0) continue;

                // Güvenlik Kontrolleri: Görev eşyalarını ve oynanamaz eşyaları atla
                if (entry && entry->IsQuestObject()) continue;
                if (!object->GetPlayable()) continue;

                // En güvenli ve performanslı yol: Tüm yığını tek seferde taşı.
                // a_player hedef olarak verildiğinde ve extraList nullptr olduğunda, 
                // motor otomatik olarak tüm stack'leri (efsunlar dahil) koruyarak taşır.
                a_container->RemoveItem(object, count, RE::ITEM_REMOVE_REASON::kRemove, nullptr, a_player);
                
                totalStacks++;
                itemsMoved += static_cast<uint32_t>(count);
            }

            g_isTransferring.store(false);
            spdlog::info("SafeTakeAll: Transfer complete. {} stacks ({} items) moved safely.", totalStacks, itemsMoved);
        }

        static inline REL::Relocation<decltype(thunk)> func;
    };

    void Install()
    {
        auto& trampoline = SKSE::GetTrampoline();
        
        // PlayerCharacter::ExtractLoot
        // SE ID: 39546, AE ID: 40632
        // Fonksiyonun başlangıcına 5 byte'lık bir branch yazarak araya giriyoruz.
        // trampoline.write_branch orijinal fonksiyonun devam adresini döndürür, bunu 'func' içinde saklıyoruz.
        REL::Relocation<std::uintptr_t> extractLoot{ REL::RelocationID(39546, 40632) };
        ExtractLootHook::func = trampoline.write_branch<5>(extractLoot.address(), (std::uintptr_t)ExtractLootHook::thunk);
        
        spdlog::info("SafeTakeAll: Protection hooks installed.");
    }
}
