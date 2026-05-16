#include <Applications/PlantsVsZombies/WaveManager.h>
#include <Applications/PlantsVsZombies/sprites/zombies/zombie_walk_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/cone_zombie_walk_sprite.h>
#include <Applications/PlantsVsZombies/sprites/ui/start_text_sprite.h>
#include <vga/vga.h>

extern volatile int compt;

/* PRNG — LCG, sufficient for gameplay randomness. */
static unsigned int wave_rng = 54321;
static unsigned int wave_lcg() {
    wave_rng = wave_rng * 1103515245 + 12345;
    return (wave_rng >> 16) & 0x7FFF;
}

WaveManager::WaveManager()
    : wave(0), remaining(0), nextSpawnTick(0), pauseUntil(0), startTextTick(0),
      currentZombieCount(0), wavePendingClear(false) {}

void WaveManager::reset() {
    wave = 0;
    remaining = 0;
    nextSpawnTick = 0;
    pauseUntil = 0;
    startTextTick = 0;
    currentZombieCount = 0;
    wavePendingClear = false;
}

int WaveManager::zombiesForWave(int w) const {
    // Linear scaling: BASE_ZOMBIES at wave 1, MAX_ZOMBIES at MAX_WAVE
    if (w <= 1) return BASE_ZOMBIES;
    if (w >= MAX_WAVE) return MAX_ZOMBIES;
    return BASE_ZOMBIES + (w - 1) * (MAX_ZOMBIES - BASE_ZOMBIES) / (MAX_WAVE - 1);
}

int WaveManager::randomLane() {
    wave_rng ^= (unsigned int)compt;
    return (int)(wave_lcg() % Grid::ROWS);
}

Zombie* WaveManager::update() {
    /* Inter-wave pause */
    if (pauseUntil > 0) {
        if (compt < pauseUntil) return 0;
        pauseUntil = 0;
        /* After pause expires, wait for all zombies to be killed */
        wavePendingClear = true;
    }

    /* Wait until all zombies from previous wave are dead */
    if (wavePendingClear) {
        if (currentZombieCount > 0) return 0;
        wavePendingClear = false;
    }

    /* Start next wave when current one is exhausted — loops after MAX_WAVE */
    if (remaining <= 0) {
        wave++;
        remaining     = zombiesForWave(((wave - 1) % MAX_WAVE) + 1);
        nextSpawnTick = compt + START_TEXT_TOTAL; // delay spawning until text finishes
        startTextTick = compt; // begin Ready/Set/Plant! sequence
    }

    if (compt < nextSpawnTick) return 0;

    int lane = randomLane();

    /* Speed bonus: +1 per wave, capped by Zombie::MIN_ANIM_SPEED internally */
    int speedBonus = wave - 1;

    int laneCenter = Grid::OFFSET_Y + lane * Grid::TILE_SIZE + Grid::TILE_SIZE / 2;
    int zx = 320 - ZOMBIE_WALK_WIDTH;

    /* Spawn type depends on wave progression:
       Wave 1   : basic only
       Wave 2+  : cone zombies (~25%)
       Wave 3+  : disco zombies (~15%)
       Wave 4+  : pogo zombies (~15%)
       Wave 5+  : football zombies (~10%) */
    int roll = wave_lcg() % 100;
    Zombie* z;

    if (wave >= 5 && roll < 10) {
        z = new FootballZombie(zx, laneCenter - ZOMBIE_WALK_HEIGHT, speedBonus);
    } else if (wave >= 4 && roll < 25) {
        z = new PogoZombie(zx, laneCenter - ZOMBIE_WALK_HEIGHT, speedBonus);
    } else if (wave >= 3 && roll < 40) {
        z = new DiscoZombie(zx, laneCenter - ZOMBIE_WALK_HEIGHT, speedBonus);
    } else if (wave >= 2 && roll < 65) {
        z = new ConeZombie(320 - CONE_ZOMBIE_WALK_WIDTH, laneCenter - CONE_ZOMBIE_WALK_HEIGHT, speedBonus);
    } else {
        z = new Zombie(zx, laneCenter - ZOMBIE_WALK_HEIGHT, speedBonus);
    }

    remaining--;
    nextSpawnTick = compt + SPAWN_INTERVAL;

    /* Set pause before next wave once this one is fully spawned. */
    if (remaining <= 0)
        pauseUntil = compt + WAVE_PAUSE;

    return z;
}

int WaveManager::getStartTextPhase() const {
    if (startTextTick == 0) return 0;
    int elapsed = compt - startTextTick;
    if (elapsed >= START_TEXT_TOTAL) return 0;
    return (elapsed / START_TEXT_DURATION) + 1; // 1=Wave N, 2=Ready, 3=Set, 4=Plant!
}

void WaveManager::renderStartText() {
    int phase = getStartTextPhase();
    if (phase == 0) return;

    /* Phase 1: "Wave N" — drawn with draw_text/draw_number */
    if (phase == 1) {
        const char* label = "wave ";
        // Compute total width: "wave " (5 chars) + digits of wave number
        int numDigits = 0;
        int tmp = wave;
        if (tmp == 0) numDigits = 1;
        else while (tmp > 0) { numDigits++; tmp /= 10; }
        int scale = 3;
        int charW = 4 * scale; // each char is 4px wide at scale
        int totalW = (5 + numDigits) * charW;
        int dx = (320 - totalW) / 2;
        int dy = (200 - 5 * scale) / 2; // 5px char height
        draw_text(label, dx, dy, 15, scale);
        draw_number(wave, dx + 5 * charW, dy, 15, scale);
        return;
    }

    const unsigned char* data;
    int srcW = START_TEXT_WIDTH;
    int srcH = START_TEXT_HEIGHT;
    int frameIdx = phase - 2; // phase 2=frame 0, 3=frame 1, 4=frame 2
    if (frameIdx < 0 || frameIdx >= START_TEXT_FRAMES) return;
    data = start_text_frames[frameIdx];

    /* Draw centered on screen (320x200). Scale to fit nicely. */
    int dstW = srcW;
    int dstH = srcH;
    /* Cap to screen width with some margin */
    if (dstW > 280) {
        dstH = dstH * 280 / dstW;
        dstW = 280;
    }
    int dx = (320 - dstW) / 2;
    int dy = (200 - dstH) / 2;
    draw_sprite_scaled(data, srcW, srcH, dx, dy, dstW, dstH);
}
