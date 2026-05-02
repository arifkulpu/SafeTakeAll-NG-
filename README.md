# SafeTakeAll (NG)

### English
SafeTakeAll is a specialized SKSE plugin designed to eliminate crashes and instability when using the "Take All" feature in Skyrim. It implements a robust, engine-level safety layer during mass inventory transfers to prevent memory corruption, and quest state corruption.

**Key Features:**
- **Atomic Transfer Logic:** Uses the engine's internal `AddObjectToContainer` mechanisms to ensure items are moved safely without duplication or loss.
- **Quest Item Protection:** Automatically detects and skips quest-related items during mass transfers to prevent broken quest triggers.
- **Non-Playable Filter:** Intelligently skips internal or non-playable items (such as body parts or hidden gear from mods like TNG/Underwear overhauls), preventing physics engine overflows.
- **UI-Only Triggering:** Strictly guarded by `ContainerMenu` state checks, ensuring it only fires when you manually press the "Take All" button, preventing conflicts with auto-loot mods or QuickLoot.
- **Silent & Optimized:** Reduces engine overhead by processing transfers without sound buffer congestion.
- **Universal Compatibility:** Built on CommonLibSSE-NG, supporting Skyrim SE, AE, GOG, and VR (1.5.97 - 1.6.1170+).

---

### Türkçe
SafeTakeAll, Skyrim'de "Hepsini Al" özelliği kullanıldığında meydana gelen çökmeleri ve kararsızlıkları gidermek için tasarlanmış özel bir SKSE eklentisidir. Toplu envanter transferleri sırasında bellek bozulmalarını, görev durumlarının bozulmasını önlemek için motor seviyesinde bir güvenlik katmanı uygular.

**Ana Özellikler:**
- **Atomik Transfer Mantığı:** Eşyaların duplikasyon (katlanma) veya kayıp olmadan güvenli bir şekilde taşınmasını sağlamak için oyunun dahili `AddObjectToContainer` mekanizmalarını kullanır.
- **Görev Eşyası Koruması:** Görevlerin bozulmasını önlemek için toplu transferler sırasında görevle ilgili eşyaları otomatik olarak tespit eder ve atlar.
- **Oynanamaz Eşya Filtresi:** Dahili veya oynanamaz eşyaları (TNG veya iç çamaşırı modlarından gelen gizli parçalar gibi) akıllıca atlayarak fizik motoru taşmalarını engeller.
- **Sadece Arayüz Tetiklemesi:** `ContainerMenu` durum kontrolleri ile korunur; böylece sadece manuel olarak "Hepsini Al" butonuna bastığınızda çalışır, QuickLoot veya otomatik toplama modlarıyla çakışmaz.
- **Sessiz ve Optimize:** Ses tamponu yoğunluğunu azaltarak ve motor yükünü minimize ederek transferleri gerçekleştirir.
- **Evrensel Uyumluluk:** CommonLibSSE-NG tabanlıdır; Skyrim SE, AE, GOG ve VR (1.5.97 - 1.6.1170+) sürümlerini destekler.

---

### Author / Yazar
- **Developed by:** [arifkulpu](https://github.com/arifkulpu)
- **Geliştiren:** [arifkulpu](https://github.com/arifkulpu)

### License / Lisans
Copyright (c) 2026 Arif KULPU. All Rights Reserved. — Tüm Hakları Saklıdır. See LICENSE for details.
