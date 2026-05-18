<div style="page-break-after: always; text-align: center; padding-top: 60px;">

<img src="IMT_Atlantique.svg.png" alt="IMT Atlantique" width="320" />

<br/><br/><br/>

<img src="assets/sprites/sources/ui/pvz_titlescreen-title.png" alt="Plants vs Zombies" width="500" />

<br/><br/><br/>

<h1 style="border: none; font-size: 28px;">Plants vs Zombies - Sextant OS</h1>

<h3 style="font-weight: normal; color: #555;">Projet Systèmes d'Exploitation</h3>

<h3 style="font-weight: normal; color: #555;">FIL-A2-Sys-expl</h3>

<br/>

<p style="font-size: 16px;"><strong>Auteurs :</strong> Nathan Marie - Mathilde Burgevin - Maxime Bodin</p>

<p style="font-size: 14px; color: #777;">FIL A2</p>

</div>

# Plants vs Zombies - Sextant OS
**Auteurs : Nathan Marie - Mathilde Burgevin - Maxime Bodin**

## 1. Introduction

### Contexte du projet

Ce projet s'inscrit dans le cadre du module **Systèmes d'Exploitation** du parcours Back de la filière FIL A2 à IMT Atlantique. L'objectif est d'implémenter un jeu vidéo complet, **Plants vs Zombies**, directement sur **Sextant OS**, un noyau x86 bare-metal éducatif. Le jeu tourne sans OS sous-jacent (pas de Linux, pas de stdlib) dans un environnement émulé via **QEMU/KVM**, avec un boot assuré par **GRUB** (Multiboot).

Ce contexte impose des contraintes fortes : gestion manuelle de la mémoire, absence d'exceptions C++, accès direct au matériel (VGA, clavier, timer), et écriture de primitives de synchronisation bas-niveau (spinlocks, sémaphores). L'enjeu est de concevoir un système temps réel capable de maintenir une boucle de jeu fluide, tout en gérant le parallélisme entre logique de jeu et rendu graphique.

### Fonctionnalités du jeu

Le jeu reprend les mécaniques classiques de Plants vs Zombies, adaptées à un affichage **VGA mode 13h** (320×200 pixels, 256 couleurs) :

- **24 types de plantes** aux comportements variés : tir (Peashooter, Repeater, Gatling Pea, Threepeater), ralentissement (Snow Peashooter, Winter Melon), explosion (Cherry Bomb, Jalapeno, Potato Mine), défense (Wall-Nut, Garlic), support (Sunflower, Twin Sunflower, Marigold), bombardement (Cabbage-pult, Kernel-pult, Melon-pult, Cob Cannon), utilitaires (Blover, Squash, Chomper, Cactus, Torchwood, Split Pea)
- **16 types de zombies** avec comportements spéciaux : basique, Conehead, Buckethead, Screen Door, Football (rapide + résistant), Disco (invoque 4 Backup Dancers), Balloon (vol), Pogo (saute par-dessus les plantes), Digger (tunnel), Pole Vaulting (saut), Newspaper (accélère), Jack-in-the-Box (explosion), Catapult (tir à distance), Gargantuar (écrase), Yeti (rare), Jalapeno Zombie (feu)
- **Système de vagues** progressif (20 vagues en boucle) avec scaling du nombre de zombies
- **Deux joueurs simultanés** avec curseurs indépendants et files de plantes séparées (3 slots, remplissage aléatoire toutes les 2–5 s)
- **Économie de soleils** : gain automatique de 25 soleils toutes les 5 s + collecte des soleils produits par les Sunflowers
- **Système de vies** (3 cœurs) avec écran de Game Over et possibilité de redémarrage
- **HUD complet** : compteur de soleils, vagues, FPS, timer, file de plantes avec coûts, indicateurs de dégâts flottants
- **Sprites animés** multi-frames pour toutes les entités (plantes, zombies, projectiles, soleils) avec variantes de dégâts (armless, headless, etc.)
- **Object pooling** pour les projectiles (90 slots) et indicateurs de dégâts (16 slots), évitant la fragmentation mémoire
- **Centrage automatique** des sprites sur les tiles (32×32) quelle que soit leur taille réelle

---

## 2. Architecture du projet

### Structure des dossiers

```
/
├── hal/                        # Hardware Abstraction Layer (boot Multiboot, ports I/O)
├── sextant/                    # Noyau Sextant
│   ├── main.cpp                #   Point d'entrée : init IDT, IRQ, timer, threads, lancement du jeu
│   ├── interruptions/          #   IDT, PIC 8259, wrappers IRQ, handlers timer/clavier
│   ├── memoire/                #   Allocateur heap (new/delete)
│   ├── ordonnancements/        #   Context switch, ordonnanceur Round-Robin
│   ├── sync/                   #   Spinlock, Semaphore, KeyboardQueue
│   └── threads/                #   Abstraction Threads (classe de base)
├── drivers/                    # Ecran (texte), Clavier, Timer 82C54, Port Série
├── vga/                        # Mode graphique VGA 13h (framebuffer, palette, sprites, texte)
├── Applications/
│   └── PlantsVsZombies/        # Code du jeu
│       ├── PlantsVsZombies.cpp #   Classe principale : boucles logique/rendu, gestion du jeu
│       ├── Entity.cpp/h        #   Classe de base des entités (position, HP, état)
│       ├── Plant.cpp/h         #   Base des plantes (animation, render centré sur tile)
│       ├── ShooterPlant.cpp/h  #   Base des plantes tireuses (cooldown, animation tir)
│       ├── Peashooter.cpp/h    #   Plante de base (tir, cooldown, animation)
│       ├── Zombie.cpp/h        #   Zombie de base (déplacement, attaque, slow, feu, variantes sprite)
│       ├── Bullet.cpp/h        #   Projectile (déplacement, impact, 10 types)
│       ├── ObjectPool.h        #   Pool générique thread-safe (Spinlock)
│       ├── WaveManager.cpp/h   #   Gestion des vagues (spawn progressif, 16 types de zombies)
│       ├── PlantQueue.cpp/h    #   File de plantes par joueur (producteur/consommateur)
│       ├── Grid.cpp/h          #   Grille 9×5 cases (placement, conversion pixel↔tile)
│       ├── Sun.cpp/h           #   Soleil collectible (spawn, durée de vie, valeur)
│       ├── LivesSystem.cpp/h   #   Système de vies (3 cœurs)
│       ├── MainMenu.cpp/h      #   Écran titre
│       └── sprites/            #   Données sprites (tableaux C générés depuis PNG)
│           ├── plants/         #     24 plantes (idle + variantes damaged/attacking)
│           ├── zombies/        #     16 types (walk/fight/death x variantes dégâts)
│           ├── objects/        #     Projectiles, impacts, feu, soleils
│           └── ui/             #     Background, tiles, textes start
└── build/                      # Artefacts : sextant.elf, grub.iso
```

### Schéma d'architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                          MATÉRIEL (x86)                            │
│  Timer 82C54 (1000 Hz)    Clavier PS/2 (IRQ1)    VGA 0xA0000      │
└────────┬──────────────────────┬───────────────────────┬────────────┘
         │ IRQ0                 │ IRQ1                  │
┌────────▼──────────────────────▼───────────────────────┤────────────┐
│                     SEXTANT KERNEL                    │            │
│  ┌──────────┐  ┌──────────────┐  ┌─────────────────┐ │            │
│  │  Timer    │  │ KeyboardQueue│  │  VGA Driver     │ │            │
│  │ handler   │  │  (ring buf)  │  │  mode 13h       │ │            │
│  │ compt++   │  │  push(IRQ)   │  │  backbuffer     │ │            │
│  └─────┬─────┘  └──────┬──────┘  └────────▲────────┘ │            │
│        │               │                  │           │            │
│  ┌─────▼───────────────────────────────────┤──────────┤────────────┤
│  │           ORDONNANCEUR Round-Robin      │          │            │
│  │  thread_yield() — context switch coopératif        │            │
│  └─────┬──────────────────┬───────────────────────────┘            │
│        │                  │                                        │
└────────┼──────────────────┼────────────────────────────────────────┘
         │                  │
┌────────▼────────┐ ┌──────▼──────────┐
│  THREAD LOGIQUE │ │  THREAD RENDU   │
│  (~62 fps)      │ │                 │
│                 │ │                 │
│  handleInput()  │ │  tryP(frameSem) │
│  ┌────────────┐ │ │    │            │
│  │ pop(KbdQ)  │ │ │    ▼            │
│  │ curseurs   │ │ │  renderFrame()  │
│  │ placement  │ │ │  ┌────────────┐ │
│  └────────────┘ │ │  │ backbuffer │ │
│  updateLogic()  │ │  │ → VGA blit │ │
│  ┌────────────┐ │ │  └────────────┘ │
│  │ WaveManager│ │ │                 │
│  │ collisions │ │ │  Yield()        │
│  │ bullets    │ │ └─────────────────┘
│  │ suns       │ │
│  └────────────┘ │         ▲
│                 │         │
│  V(frameSem) ───┼─────────┘
│  Yield()        │    Semaphore
└─────────────────┘   (signal frame)
```

### Flux d'exécution d'une itération

```
LogicThread : handleInput() + updateLogic()  →  frameSem.V()  →  Yield()
RenderThread: tryP(frameSem)                 →  renderFrame() →  Yield()
IdleThread  : Yield() en boucle (thread initial)
```

---

## 3. Détails et justifications d'implémentation

### 3.1 Threads et ordonnanceur

#### Choix de l'ordonnancement

L'ordonnancement adopté est un **Round-Robin coopératif** (`Yield()` explicite). Chaque thread s'exécute sans être interrompu par l'ordonnanceur, et cède volontairement le CPU via `thread_yield()` à la fin de chaque itération. Ce choix repose sur les constats suivants :

- **Mono-core** : Sextant tourne sur un seul cœur (QEMU sans SMP). Un seul thread est actif à tout instant.
- **Pas de préemption involontaire** : les wrappers IRQ (`irq_wrappers.S`) exécutent le handler puis font `iret`. Un appel à `cpu_context_switch()` depuis un handler d'IRQ corromprait la pile car l'`iret` restaurerait le mauvais contexte. L'ordonnancement coopératif contourne ce problème.
- **Réactivité garantie** : le timer matériel (82C54) est configuré à **1000 Hz**. Le compteur global `compt` est incrémenté à chaque tick. Le thread logique attend le prochain tick via `Yield()` plutôt que par attente active (`while(compt == lastTick) spin`), évitant d'affamer le thread rendu.

#### Cadence à ~62 FPS

Le thread logique exécute la mise à jour toutes les **16 ms** (`compt % 16 == 0`), soit environ 62 images par seconde. Ce cadencement est dérivé du compteur de ticks matériel et non d'un timer software, garantissant une régularité indépendante de la charge de calcul :

```cpp
while (compt == lastTick) thread_yield();   // attente coopérative
if ((compt % 16) == 0) {
    handleInput();
    updateLogic();
    frameSem.V();          // signal au thread rendu
}
thread_yield();            // cède le CPU
```

#### Organisation des threads

Trois threads coexistent dans le système :

| Thread | Rôle | Boucle |
|--------|------|--------|
| **LogicThread** | Lecture clavier, mise à jour de l'état (vagues, collisions, projectiles, soleils) | `runLogic()` |
| **RenderThread** | Composition du backbuffer, copie vers le framebuffer VGA | `runRender()` |
| **IdleThread** | Thread initial (`Sextant_main`), cède le CPU indéfiniment | `while(true) Yield()` |

### 3.2 Sémaphores et synchronisation

#### Données partagées entre threads

Les structures suivantes sont accédées par les deux threads de jeu :

- **`plants[MAX_PLANTS]`**: tableau de pointeurs vers les plantes actives
- **`zombies[MAX_ZOMBIES]`**: tableau de pointeurs vers les zombies actifs
- **`bulletPool`** / **`dmgPool`**: pools d'objets pour les projectiles et indicateurs de dégâts
- **`suns_on_ground[MAX_SUNS]`**: soleils collectibles au sol
- **`suns`**, **`lives`**, **`gameOver`**: état global du jeu

#### Risques sans protection

Sans mécanisme de synchronisation, des accès concurrents entre le thread logique et le thread rendu provoqueraient :

- **Entités fantômes** : le thread rendu itère sur `plants[]` pendant que le thread logique supprime une plante morte → accès à un pointeur invalidé → crash ou affichage d'une entité inexistante.
- **Positionnement incohérent** : le thread logique met à jour la position d'un zombie pendant que le thread rendu lit cette même position → le sprite est dessiné à une coordonnée intermédiaire (tearing logique).
- **Corruption du pool** : le thread logique appelle `bulletPool.acquire()` pendant que le thread rendu itère sur le pool → un slot peut passer de inactif à actif entre le test `isActive()` et l'appel `render()`.

#### Protection des sections critiques

La synchronisation repose sur trois mécanismes complémentaires :

**1. Sémaphore `frameSem` (logique → rendu)**

Le sémaphore de comptage `frameSem` (valeur initiale 0) synchronise la production et la consommation de frames :

```cpp
// Thread logique (producteur)
frameSem.V();       // signale qu'une frame est prête

// Thread rendu (consommateur)
while (!frameSem.tryP()) thread_yield();  // attend une frame sans bloquer
renderFrame();
```

`tryP()` est non-bloquant : si aucune frame n'est prête, le thread cède immédiatement le CPU. Cela évite le deadlock sur mono-core, où un `P()` bloquant empêcherait le thread logique de s'exécuter.

**2. Spinlocks (ObjectPool, PlantQueue)**

Les structures de données partagées (`ObjectPool`, `PlantQueue`) sont protégées par des **Spinlocks** basés sur l'instruction atomique `XCHG` x86 :

```cpp
void Spinlock::acquire() {
    int tmp;
    do {
        asm volatile ("movl $1, %0\n\t"
                      "xchgl %0, %1"
                      : "=r"(tmp), "+m"(locked) ::: "memory");
    } while (tmp != 0);
}
```

`XCHG` verrouille le bus mémoire pendant l'échange, garantissant l'atomicité. Chaque opération `acquire()` / `release()` sur l'`ObjectPool` protège l'itération sur les slots :

```cpp
T* ObjectPool::acquire() {
    lock.acquire();
    for (int i = 0; i < SIZE; i++)
        if (!items[i].isActive()) { lock.release(); return &items[i]; }
    lock.release();
    return 0;
}
```

**3. Désactivation des IRQ (KeyboardQueue)**

La file clavier suit un modèle **producteur/consommateur** entre le handler IRQ1 (producteur) et la boucle de jeu (consommateur). Le consommateur doit désactiver les interruptions avant d'appeler `tryP()` :

```cpp
bool KeyboardQueue::pop(KeyEvent& out) {
    ui32_t flags;
    disable_IRQs(flags);      // IF=0 : empêche l'IRQ clavier
    bool ok = count.tryP();   // consomme un token sans risque de deadlock
    restore_IRQs(flags);      // IF=1
    if (!ok) return false;
    out = buffer[head];
    head = (head + 1) % KBD_QUEUE_CAP;
    return true;
}
```

Sans cette précaution, si `tryP()` tient le spinlock interne et qu'une IRQ clavier appelle `V()` sur le même sémaphore, le `V()` tenterait d'acquérir le même spinlock → **deadlock** sur mono-core.

### 3.3 Système de vagues (WaveManager)

Le `WaveManager` contrôle le rythme du jeu avec un système de vagues progressif :

- **Scaling** : le nombre de zombies par vague suit une progression en racine carrée de `BASE_ZOMBIES` (2) à `WAVE_MAX_ZOMBIES` (30) sur 20 vagues. Cette courbe offre une difficulté qui croît rapidement au début puis se stabilise.
- **16 types de zombies** : l'apparition des types est conditionnée par le numéro de vague (basique dès la vague 1, Cone/Buckethead/Screen Door dès la vague 2, Disco/Baseball/Newspaper dès la vague 3, Pogo/Balloon dès la vague 4, Football/Catapult dès la vague 5, Digger dès la 6, Gargantuar dès la 7, Yeti dès la 8).
- **Spawn espacé** : les zombies d'une vague apparaissent avec un intervalle de **3 secondes** (`SPAWN_INTERVAL = 3000 ticks`).
- **Pause inter-vagues** : **12 secondes** (`WAVE_PAUSE`) entre deux vagues, pendant lesquelles le jeu attend que tous les zombies soient éliminés (`wavePendingClear`).
- **Texte d'annonce** : séquence « Wave N → Ready → Set → Plant! » affichée pendant 3.2 secondes avant chaque vague, durant laquelle la logique de jeu est mise en pause et les inputs bloqués.
- **Boucle infinie** : après la vague 20, le cycle reprend avec les mêmes paramètres de difficulté.

Le placement aléatoire des zombies sur les 5 lignes utilise un **LCG** (Linear Congruential Generator) dont la graine est perturbée par le compteur de ticks `compt`, assurant une variance suffisante entre les parties.

### 3.4 Système de curseurs (2 joueurs)

Chaque joueur dispose d'un **curseur de grille** et d'un **curseur de roster** :

- **Curseur de grille** (`cursorCol`/`cursorRow` pour P1, `cursorCol2`/`cursorRow2` pour P2) : navigué via les touches directionnelles, borné aux limites de la grille 9×5. Affiché comme un rectangle coloré (blanc pour P1, vert pour P2) qui passe au rouge si la case est occupée.
- **Curseur de roster** : sélectionne la plante à placer parmi la file du joueur. Navigué via deux touches dédiées (W/X pour P1, Y/U pour P2).

Les événements clavier transitent via la `KeyboardQueue` (ring buffer IRQ → boucle de jeu). Chaque appui physique génère exactement un événement `make` dans la file, éliminant les problèmes de détection de front montant et de typematic.

### 3.5 Système de points de vie

Chaque entité hérite de la classe `Entity` qui fournit un système de HP générique :

- **Plantes** : HP variables selon le type (300 HP pour un Peashooter, 4000 HP pour un Wall-Nut, 300 HP pour un Blover). Barre de vie rendue au-dessus du sprite via `renderHpBar()` (vert → rouge proportionnel). Sprites centrés sur la tile via un offset calculé à partir de `Grid::TILE_SIZE`.
- **Zombies** : 150 HP de base (Basique), jusqu'à 3000 HP (Gargantuar). Variantes de sprites par niveau de dégâts (full → no_arm → no_head_no_arm) avec dimensions adaptatives. Effets de status : ralentissement (Snow Pea), feu (Jalapeno), push-back (Blover).
- **Joueur** : 3 vies (cœurs pixel-art dans le HUD). Un zombie atteignant le bord gauche de l'écran retire une vie. À 0 : écran Game Over.

### 3.6 Object Pooling

Le système d'**Object Pooling** évite les allocations dynamiques répétées pour les objets à forte rotation :

```cpp
template<typename T, int SIZE>
class ObjectPool {
    T items[SIZE];       // tableau statique pré-alloué
    Spinlock lock;       // protection thread-safe
public:
    T* acquire();        // retourne le premier slot inactif (ou 0)
    void release(T*);    // marque le slot comme inactif
    T* get(int i);       // accès direct pour itération
};
```

**Pourquoi le pooling ?** Sur Sextant, l'allocateur heap (`memoire/`) est minimal. Les projectiles et indicateurs de dégâts sont créés et détruits à haute fréquence (plusieurs dizaines par seconde). Sans pooling :
- **Fragmentation mémoire** : les `new`/`delete` successifs fragmenteraient le heap, risquant une allocation échouée en pleine partie.
- **Coût d'allocation** : `getmem()` parcourt une liste chaînée, O(n) dans le pire cas.

Le pool pré-alloue un tableau statique de taille fixe (**90 projectiles**, **16 indicateurs de dégâts**). L'acquisition et la libération sont en O(n) sur la taille du pool (parcours linéaire), mais le pool est petit et la constante est faible. Le **Spinlock** interne protège contre les accès concurrents lors de l'acquisition/libération.

### 3.7 Utilisation du mode VGA 13h

Le rendu graphique utilise le **mode VGA 13h** : un framebuffer linéaire de 64 000 octets à l'adresse `0xA0000` (320×200, 1 octet/pixel, 256 couleurs indexées). La palette de 256 couleurs (triplets RGB 6 bits) est chargée via les ports DAC (`0x3C8`/`0x3C9`) au démarrage.

Le système utilise un **double buffering** :
1. Le thread rendu compose l'image dans un `backbuffer` alloué en mémoire.
2. Une fois la frame complète, le contenu est copié d'un bloc vers le framebuffer VGA (`video`).

Cette technique élimine le **tearing** (déchirure d'image) : le joueur ne voit jamais un état intermédiaire de la composition.

Les sprites (32×32 pixels, multi-frames pour l'animation) sont générés à partir de fichiers PNG via un script Python (`sprite.py`) qui quantifie les couleurs sur la palette partagée. L'index 255 est réservé à la transparence.

---

## 4. Conclusion

Ce projet démontre qu'il est possible d'implémenter un jeu vidéo complet et jouable directement sur un noyau bare-metal x86, en respectant les contraintes d'un système temps réel : gestion matérielle directe, absence de bibliothèque standard, et primitives de synchronisation écrites à la main.

Les choix techniques, ordonnancement coopératif Round-Robin, sémaphore producteur/consommateur pour la synchronisation logique/rendu, spinlocks atomiques pour les structures partagées, et object pooling pour les allocations fréquentes, répondent chacun à un problème concret identifié lors du développement. Le modèle à deux threads (logique + rendu) avec signalisation par sémaphore offre une séparation claire des responsabilités tout en garantissant une cadence stable de ~62 FPS.

Le mode VGA 13h, bien que limité en résolution (320×200), s'est révélé adapté au genre tower-defense : la grille 9×5 cases de 32 pixels offre un espace de jeu lisible, et la palette de 256 couleurs permet des sprites expressifs avec des variantes de dégâts progressifs. Le double buffering élimine le tearing sans nécessiter de mécanisme complexe de synchronisation verticale.

Le support de deux joueurs simultanés, le système de 20 vagues à difficulté croissante, les 24 types de plantes aux comportements variés, les 16 types de zombies avec sprites multi-variantes, et les indicateurs visuels (barres de vie, dégâts flottants, animations multi-frames) font de ce projet une application non triviale qui exploite l'ensemble des concepts étudiés en cours : interruptions, ordonnancement, synchronisation, et gestion mémoire.

---

### Générer un sprite
```bash
python3 vga/sprite.py <source_png_dir> -p Applications/PlantsVsZombies/sprites/shared_palette.cpp -n <nom_sprite> -o <output_dir>
```

Exemple pour régénérer un sprite zombie :
```bash
python3 vga/sprite.py assets/sprites/sources/zombies/basic/walk_full/ \
  -p Applications/PlantsVsZombies/sprites/shared_palette.cpp \
  -n basic_zombie_walk_full \
  -o Applications/PlantsVsZombies/sprites/zombies/basic/
```

Les sprites sont organisés en sous-dossiers : `sprites/plants/`, `sprites/zombies/`, `sprites/objects/`, `sprites/ui/`. Le Makefile détecte automatiquement tous les `*_sprite.cpp` via `$(shell find)`.