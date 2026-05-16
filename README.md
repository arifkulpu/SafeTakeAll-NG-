# SafeTakeAll (NG)

### English
SafeTakeAll is a specialized SKSE plugin designed to eliminate crashes, instability, and unintended item transfers when using the "Take All" feature in Skyrim. It implements a robust, engine-level safety layer during mass inventory transfers to prevent memory corruption and quest state corruption.

**Key Features:**
- **Snapshot-Based Transfer:** Builds a complete snapshot of the container's inventory before moving anything, preventing crashes caused by memory pointers being invalidated during transfer (critical for items from SPID and SkyPatcher).
- **Dual-Guard Menu Validation:** The plugin now uses two strict conditions before any transfer is allowed:
  1. A `ContainerMenu` must be open.
  2. The container passed by the engine to `ExtractLoot` must exactly match the reference currently shown in the ContainerMenu. This prevents other mods or engine callbacks from triggering mass auto-looting of nearby actors when a single NPC is killed.
- **Dynamic Item Stability:** Specifically optimized for items distributed at runtime by mods like SPID and SkyPatcher.
- **Silent Transfer Mode:** Uses the engine's `kStoreInContainer` removal reason to suppress unnecessary script events and UI update signals, preventing race conditions during mass transfers.
- **Exception Shielding:** Integrated try-catch blocks and validity handles ensure that even if a single item fails to transfer, the game remains stable and continues processing the rest of the inventory.
- **Quest & Safety Filters:** Automatically skips quest items and non-playable objects to prevent broken quest triggers.
- **Zero Trampoline Conflicts:** Fully replaces `ExtractLoot` to avoid conflicts with other mods hooking the same address.
- **Universal Compatibility:** Built on CommonLibSSE-NG; supports Skyrim SE, AE, GOG, and VR (1.5.97 – 1.6.1170+).

**Compatibility & Interactions:**
- **QuickLoot RE / EE / GOG:** Fully compatible. Stabilizes the "Take All" hotkey/button in these modern loot menus.
- **TNG (The New Gentleman) & MuJointFix:** Uses `kStoreInContainer` to bypass engine-level transfer events, completely eliminating stack overflow crashes triggered by these mods.
- **SPID & SkyPatcher:** Specifically fixes crashes involving items distributed at runtime which often lack persistent engine handles.
- **Auto Loot Mods:** Provides a safer backbone for any mod that triggers the player's native `ExtractLoot` function.

**Bug Fixes (Changelog):**
- **v1.3** — Fixed a critical bug where killing an NPC caused the items of all nearby actors to be automatically transferred to the player's inventory. Root cause: the engine (or other mods) can call `PlayerCharacter::ExtractLoot` for unrelated refs even while a ContainerMenu is open. Fix: added a second guard that validates `a_container == ContainerMenu::GetTargetRefHandle()` before any transfer proceeds.

---

### Türkçe
SafeTakeAll, Skyrim'de "Hepsini Al" özelliği kullanıldığında meydana gelen çökmeleri, kararsızlıkları ve istenmeyen eşya transferlerini gidermek için tasarlanmış özel bir SKSE eklentisidir.

**Ana Özellikler:**
- **Anlık Görüntü (Snapshot) Transferi:** Eşyaları taşımadan önce envanterin tam bir görüntüsünü oluşturur; transfer sırasında geçersizleşen bellek adreslerinden kaynaklanan çökmeleri (özellikle SPID ve SkyPatcher eşyalarında görülen) tamamen engeller.
- **Çift Katmanlı Menü Doğrulaması:** Eklenti artık transfer başlatmadan önce iki katı koşulu birden denetler:
  1. Bir `ContainerMenu` açık olmalıdır.
  2. Motorun `ExtractLoot`'a ilettiği konteyner referansı, ContainerMenu'nun **tam o anda** gösterdiği ref ile birebir örtüşmelidir. Bu kontrol, bir NPC öldürüldüğünde yakındaki diğer aktörlerin eşyalarının da otomatik olarak envantere gelmesini engeller.
- **Dinamik Eşya Desteği:** SPID ve SkyPatcher gibi modlar tarafından çalışma zamanında eklenen eşyalar için özel kararlılık optimizasyonları içerir.
- **Sessiz Transfer Modu:** Gereksiz script olaylarını ve arayüz güncelleme sinyallerini bastırmak için motorun `kStoreInContainer` kaldırma nedenini kullanır.
- **Hata Koruması:** Entegre hata yakalama blokları, tek bir eşyada sorun çıksa bile oyunun çökmesini önler.
- **Görev ve Güvenlik Filtreleri:** Görev eşyalarını ve oynanamaz nesneleri otomatik olarak atlar.
- **Evrensel Uyumluluk:** CommonLibSSE-NG tabanlıdır; Skyrim SE, AE, GOG ve VR sürümlerini destekler (1.5.97 – 1.6.1170+).

**Uyumluluk ve Etkileşimler:**
- **QuickLoot RE / EE / GOG:** Tam uyumlu.
- **TNG (The New Gentleman) & MuJointFix:** `kStoreInContainer` kullanarak yığın taşması çökmelerini tamamen ortadan kaldırır.
- **SPID & SkyPatcher:** Çalışma zamanında dağıtılan eşyalarla ilgili çökmeleri özel olarak düzeltir.
- **Otomatik Loot Modları:** Oyuncunun yerel `ExtractLoot` fonksiyonunu tetikleyen tüm modlar için daha güvenli bir altyapı sağlar.

**Hata Düzeltmeleri (Değişiklik Geçmişi):**
- **v1.3** — Kritik bir hata düzeltildi: Bir NPC öldürüldüğünde yakındaki tüm aktörlerin eşyaları otomatik olarak oyuncunun envanterine geçiyordu. Kök neden: Oyun motoru (veya diğer modlar), ContainerMenu açıkken bile `PlayerCharacter::ExtractLoot`'u ilgisiz referanslar için çağırabiliyor. Düzeltme: Herhangi bir transfer başlamadan önce `a_container == ContainerMenu::GetTargetRefHandle()` koşulunu doğrulayan ikinci bir güvenlik katmanı eklendi.

---

### Author / Yazar
- **Developed by:** [arifkulpu](https://github.com/arifkulpu)
- **Geliştiren:** [arifkulpu](https://github.com/arifkulpu)

### License / Lisans
Copyright (c) 2026 Arif KULPU. All Rights Reserved. — Tüm Hakları Saklıdır. See LICENSE for details.
