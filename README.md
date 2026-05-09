# SafeTakeAll (NG)

### English
SafeTakeAll is a specialized SKSE plugin designed to eliminate crashes and instability when using the "Take All" feature in Skyrim. It implements a robust, engine-level safety layer during mass inventory transfers to prevent memory corruption and quest state corruption.

**Key Features:**
- **Snapshot-Based Transfer:** Builds a complete snapshot of the container's inventory before moving anything. Now uses **FormID-based tracking** to prevent crashes caused by memory pointers being invalidated during transfer (critical for items from SPID and SkyPatcher).
- **Dynamic Item Stability:** Specifically optimized for items distributed at runtime by mods like SPID and SkyPatcher.
- **Silent Transfer Mode:** Uses the engine's `kNone` removal reason to suppress unnecessary script events and UI update signals, preventing race conditions during mass transfers.
- **Exception Shielding:** Integrated try-catch blocks and validity handles (`ObjectRefHandle`) ensure that even if a single item fails to transfer, the game remains stable and continues processing the rest of the inventory.
- **Quest & Safety Filters:** Automatically skips quest items and non-playable objects to prevent broken quest triggers.
- **Smart Menu Detection:** The plugin only activates when a `ContainerMenu` (chest/corpse) is open. Other contexts (Barter, Gift menus) are completely unaffected.
- **Zero Trampoline Conflicts:** Fully replaces `ExtractLoot` to avoid conflicts with other mods hooking the same address.
- **Universal Compatibility:** Built on CommonLibSSE-NG; supports Skyrim SE, AE, GOG, and VR (1.5.97 – 1.6.1170+).

---

### Türkçe
SafeTakeAll, Skyrim'de "Hepsini Al" özelliği kullanıldığında meydana gelen çökmeleri, kararsızlıkları ve eşya katlanmalarını gidermek için tasarlanmış özel bir SKSE eklentisidir.

**Ana Özellikler:**
- **Anlık Görüntü (Snapshot) Transferi:** Eşyaları taşımadan önce envanterin tam bir görüntüsünü oluşturur. Artık bellek adresleri yerine **FormID tabanlı takip** kullanarak, transfer sırasında geçersizleşen objelerden kaynaklı çökmeleri (özellikle SPID ve SkyPatcher eşyalarında görülen) tamamen engeller.
- **Dinamik Eşya Desteği:** SPID ve SkyPatcher gibi modlar tarafından çalışma zamanında eklenen eşyalar için özel kararlılık optimizasyonları içerir.
- **Sessiz Transfer Modu:** Gereksiz script olaylarını ve arayüz güncelleme sinyallerini bastırmak için motorun `kNone` kaldırma nedenini kullanır, böylece toplu transferlerdeki yarış durumlarını (race conditions) önler.
- **Hata Koruması:** Entegre hata yakalama blokları ve geçerlilik tutamaçları (`ObjectRefHandle`), tek bir eşyada sorun çıksa bile oyunun çökmesini önler ve envanterin geri kalanının işlenmesine devam edilmesini sağlar.
- **Görev ve Güvenlik Filtreleri:** Görevlerin bozulmasını önlemek için görev eşyalarını ve oynanamaz nesneleri otomatik olarak atlar.
- **Akıllı Menü Tespiti:** Eklenti yalnızca `ContainerMenu` açıkken devreye girer. Diğer menüler (Satıcı, Hediye vb.) tamamen dokunulmadan kalır.
- **Evrensel Uyumluluk:** CommonLibSSE-NG tabanlıdır; Skyrim SE, AE, GOG ve VR sürümlerini destekler (1.5.97 – 1.6.1170+).

---

### Author / Yazar
- **Developed by:** [arifkulpu](https://github.com/arifkulpu)
- **Geliştiren:** [arifkulpu](https://github.com/arifkulpu)

### License / Lisans
Copyright (c) 2026 Arif KULPU. All Rights Reserved. — Tüm Hakları Saklıdır. See LICENSE for details.
