# SafeTakeAll (NG)

### English
SafeTakeAll is a specialized SKSE plugin designed to eliminate crashes and instability when using the "Take All" feature in Skyrim. It implements a robust, engine-level safety layer during mass inventory transfers to prevent memory corruption, and quest state corruption.

**Key Features:**
- **Enhanced Atomic Transfer:** Uses the engine's optimized `RemoveItem` logic to move entire stacks safely. This preserves all enchantments, stolen tags, and custom data without risk of duplication or infinite loops.
- **Quest & Safety Filters:** Automatically skips quest items and non-playable objects to prevent broken triggers and physics overflows.
- **Smart Menu Fallback:** The plugin intelligently detects if you are in a `ContainerMenu`. If the "Take All" function is called in other contexts (like Barter or custom menus), it safely falls back to the original game logic, ensuring total compatibility.
- **Zero-Loop Logic:** Replaced dangerous manual iterations with engine-level batch transfers, eliminating the "hundreds of items" bug.
- **Universal Compatibility:** Supports Skyrim SE, AE, GOG, and VR (1.5.97 - 1.6.1170+).

---

### Türkçe
SafeTakeAll, Skyrim'de "Hepsini Al" özelliği kullanıldığında meydana gelen çökmeleri, kararsızlıkları ve eşya katlanmalarını gidermek için tasarlanmış özel bir SKSE eklentisidir.

**Ana Özellikler:**
- **Gelişmiş Atomik Transfer:** Eşyaları tek tek taşımak yerine, oyun motorunun kendi optimize edilmiş `RemoveItem` mantığını kullanarak tüm yığınları güvenli bir şekilde taşır. Bu, efsunların ve özel verilerin korunmasını sağlarken sonsuz döngü riskini ortadan kaldırır.
- **Görev ve Güvenlik Filtreleri:** Görevlerin bozulmasını önlemek için görev eşyalarını ve oynanamaz (gizli) nesneleri otomatik olarak atlar.
- **Akıllı Menü Uyumluluğu (Fallback):** Eklenti, sadece `ContainerMenu` (sandık/ceset) açıkken devreye girer. Diğer durumlarda (Satıcı menüsü vb.) oyunun orijinal mantığının çalışmasına izin vererek maksimum uyumluluk sağlar.
- **Hata Giderimleri:** Manuel döngüden kaynaklanan "bir anda yüzlerce eşya gelmesi" hatası tamamen giderilmiştir.
- **Evrensel Uyumluluk:** CommonLibSSE-NG tabanlıdır; Skyrim SE, AE, GOG ve VR sürümlerini destekler.

---

### Author / Yazar
- **Developed by:** [arifkulpu](https://github.com/arifkulpu)
- **Geliştiren:** [arifkulpu](https://github.com/arifkulpu)

### License / Lisans
Copyright (c) 2026 Arif KULPU. All Rights Reserved. — Tüm Hakları Saklıdır. See LICENSE for details.
