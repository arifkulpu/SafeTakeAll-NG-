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

            // 1. İşlem devam ediyorsa tekrar tetikleme
            if (g_isTransferring.load()) {
                return;
            }

            // 2. Sadece ContainerMenu açıkken çalış (UI Güvenliği)
            auto ui = RE::UI::GetSingleton();
            if (!ui || !ui->IsMenuOpen(RE::ContainerMenu::MENU_NAME)) {
                return;
            }

            spdlog::info("SafeTakeAll: Intercepting Take All for {} ({:08X})", a_container->GetName(), a_container->GetFormID());
            
            ExecuteSafeTransfer(a_this, a_container);
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

            uint32_t stacksMoved = 0;
            for (auto& [object, data] : inventory) {
                auto& [count, entry] = data;
                
                if (!object || count <= 0) continue;

                // Güvenlik Kontrolleri: Görev eşyalarını ve oynanamaz eşyaları atla
                if (entry && entry->IsQuestObject()) continue;
                if (!object->GetPlayable()) continue;

                // ELEŞTİRİ FİX: ExtraDataList'i koruyarak taşıma işlemi
                // Her bir alt-yığın (farklı efsunlar/özellikler) için ayrı taşıma yapılır.
                if (entry && entry->extraLists) {
                    int32_t movedCountWithExtra = 0;
                    for (auto* xList : *entry->extraLists) {
                        if (xList) {
                            int32_t xCount = 1;
                            if (xList->HasType(RE::ExtraDataType::kCount)) {
                                xCount = xList->GetByType<RE::ExtraCount>()->count;
                            }
                            
                            // RemoveItem'a destination (a_player) ve xList vererek tüm özellikleri koruyoruz.
                            a_container->RemoveItem(object, xCount, RE::ItemRemoveReason::kRemove, xList, a_player);
                            movedCountWithExtra += xCount;
                            stacksMoved++;
                        }
                    }

                    // Eğer ExtraList'te olmayan (ham/temiz) eşyalar kaldıysa onları da taşı
                    int32_t remainingCount = count - movedCountWithExtra;
                    if (remainingCount > 0) {
                        a_container->RemoveItem(object, remainingCount, RE::ItemRemoveReason::kRemove, nullptr, a_player);
                        stacksMoved++;
                    }
                } else {
                    // Hiç extra data yoksa direkt tüm yığını taşı
                    a_container->RemoveItem(object, count, RE::ItemRemoveReason::kRemove, nullptr, a_player);
                    stacksMoved++;
                }
            }

            g_isTransferring.store(false);
            spdlog::info("SafeTakeAll: Transfer complete. {} stacks moved safely.", stacksMoved);
        }

        static inline REL::Relocation<decltype(thunk)> func;
    };

    void Install()
    {
        auto& trampoline = SKSE::GetTrampoline();
        
        // PlayerCharacter::ExtractLoot
        // SE ID: 39546, AE ID: 40632
        // Fonksiyonun başlangıcına 5 byte'lık bir branch (atlama) yazarak tamamen override ediyoruz.
        REL::Relocation<std::uintptr_t> extractLoot{ REL::RelocationID(39546, 40632) };
        trampoline.write_branch<5>(extractLoot.address(), (std::uintptr_t)ExtractLootHook::thunk);
        
        spdlog::info("SafeTakeAll: Protection hooks installed.");
    }
}
