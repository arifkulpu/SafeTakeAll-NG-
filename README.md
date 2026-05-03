# SafeTakeAll (NG)

### English
SafeTakeAll is a specialized SKSE plugin designed to eliminate crashes and instability when using the "Take All" feature in Skyrim. It implements a robust, engine-level safety layer during mass inventory transfers to prevent memory corruption and quest state corruption.

**Key Features:**
- **Snapshot-Based Transfer:** Builds a complete snapshot of the container's inventory before moving anything. This prevents the "hundreds of items" duplication bug caused by iterating and modifying the live inventory list simultaneously.
- **Quest & Safety Filters:** Automatically skips quest items and non-playable objects to prevent broken quest triggers.
- **Smart Menu Detection:** The plugin only activates when a `ContainerMenu` (chest/corpse) is open. Other contexts (Barter, Gift menus) are completely unaffected — the engine's original code runs unchanged for those cases.
- **Zero Trampoline Conflicts:** Instead of chaining back to the original function (which caused ACCESS_VIOLATION crashes when other mods hooked the same address), the function is fully replaced. This is safe because the engine only calls `ExtractLoot` from ContainerMenu context.
- **Atomic Re-Entry Guard:** Uses `std::atomic<bool>` to prevent any recursive or duplicate calls during the transfer.
- **Universal Compatibility:** Built on CommonLibSSE-NG; supports Skyrim SE, AE, GOG, and VR (1.5.97 – 1.6.1170+).

---

### Türkçe
SafeTakeAll, Skyrim'de "Hepsini Al" özelliği kullanıldığında meydana gelen çökmeleri, kararsızlıkları ve eşya katlanmalarını gidermek için tasarlanmış özel bir SKSE eklentisidir.

**Ana Özellikler:**
- **Anlık Görüntü (Snapshot) Transferi:** Eşyaları taşımadan önce konteyner envanterinin tam bir görüntüsünü oluşturur. Bu, canlı liste üzerinde aynı anda yineleme ve değiştirmeden kaynaklanan "yüzlerce eşya gelme" çoğaltma hatasını ortadan kaldırır.
- **Görev ve Güvenlik Filtreleri:** Görevlerin bozulmasını önlemek için görev eşyalarını ve oynanamaz (gizli) nesneleri otomatik olarak atlar.
- **Akıllı Menü Tespiti:** Eklenti yalnızca `ContainerMenu` (sandık/ceset) açıkken devreye girer. Diğer menüler (Satıcı, Hediye vb.) tamamen dokunulmadan kalır; motor orijinal kodu sorunsuz çalıştırır.
- **Sıfır Trampoline Çakışması:** Aynı adrese kanca atan diğer modlarla yaşanan ACCESS_VIOLATION çökmelerini önlemek için orijinal fonksiyona geri dönmek yerine fonksiyon tamamen değiştirilir. Motor `ExtractLoot`'u yalnızca ContainerMenu bağlamından çağırdığı için bu yaklaşım tamamen güvenlidir.
- **Atomik Yeniden Giriş Koruması:** Transfer sırasında yinelemeli veya çift çağrıları önlemek için `std::atomic<bool>` kullanılır.
- **Evrensel Uyumluluk:** CommonLibSSE-NG tabanlıdır; Skyrim SE, AE, GOG ve VR sürümlerini destekler (1.5.97 – 1.6.1170+).

---

### Author / Yazar
- **Developed by:** [arifkulpu](https://github.com/arifkulpu)
- **Geliştiren:** [arifkulpu](https://github.com/arifkulpu)

### License / Lisans
Copyright (c) 2026 Arif KULPU. All Rights Reserved. — Tüm Hakları Saklıdır. See LICENSE for details.
