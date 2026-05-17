#include <Applications/PlantsVsZombies/PogoZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/pogo_zombie_pogo_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/pogo_zombie_pogo_headless_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/pogo_zombie_walk_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/pogo_zombie_walk_headless_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/pogo_zombie_walk_armless_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/pogo_zombie_fight_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/pogo_zombie_fight_headless_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/pogo_zombie_death_sprite.h>

PogoZombie::PogoZombie(int x, int y, int speedBonus)
    : Zombie(x, y, POGO_TOTAL_HP, speedBonus + POGO_SPEED), pogoHp(POGO_HP), lastDamageStage(0) {}

int PogoZombie::getDamageStage() const {
    if (state == DYING) return lastDamageStage;
    if (pogoHp > 0) return 0;                  // on pogo
    if (hp > HEADLESS_THRESHOLD) return 1;      // walking full
    if (hp > ARMLESS_THRESHOLD)  return 2;      // headless
    return 3;                                   // armless
}

void PogoZombie::onUpdate() {
    if (pogoHp > 0) {
        int damageTaken = POGO_TOTAL_HP - hp;
        if (damageTaken >= POGO_HP) {
            pogoHp = 0;
            lastDamageStage = 1;
        } else {
            lastDamageStage = 0;
        }
    } else if (hp > HEADLESS_THRESHOLD) {
        lastDamageStage = 1;
    } else if (hp > ARMLESS_THRESHOLD) {
        lastDamageStage = 2;
    } else {
        lastDamageStage = 3;
    }
}

bool PogoZombie::canBeBlocked() const {
    return pogoHp <= 0;
}

int PogoZombie::getWidth() const {
    if (pogoHp > 0) return POGO_ZOMBIE_POGO_FULL_WIDTH;
    return POGO_ZOMBIE_WALK_FULL_WIDTH;
}

int PogoZombie::getHeight() const {
    if (pogoHp > 0) return POGO_ZOMBIE_POGO_FULL_HEIGHT;
    return POGO_ZOMBIE_WALK_FULL_HEIGHT;
}

const unsigned char* PogoZombie::currentWalkFrame(int f) const {
    switch (getDamageStage()) {
        case 0:  // on pogo, check headless variant
            if (hp <= HEADLESS_THRESHOLD)
                return pogo_zombie_pogo_headless_frames[f % POGO_ZOMBIE_POGO_HEADLESS_FRAMES];
            return pogo_zombie_pogo_full_frames[f];
        case 2:  return pogo_zombie_walk_headless_frames[f % POGO_ZOMBIE_WALK_HEADLESS_FRAMES];
        case 3:  return pogo_zombie_walk_armless_frames[f % POGO_ZOMBIE_WALK_ARMLESS_FRAMES];
        default: return pogo_zombie_walk_full_frames[f];
    }
}

const unsigned char* PogoZombie::currentFightFrame(int f) const {
    if (getDamageStage() >= 2)
        return pogo_zombie_fight_headless_frames[f % POGO_ZOMBIE_FIGHT_HEADLESS_FRAMES];
    return pogo_zombie_fight_full_frames[f];
}

int PogoZombie::currentWalkFrameCount() const {
    if (pogoHp > 0) return POGO_ZOMBIE_POGO_FULL_FRAMES;
    return POGO_ZOMBIE_WALK_FULL_FRAMES;
}

int PogoZombie::currentFightFrameCount() const { return POGO_ZOMBIE_FIGHT_FULL_FRAMES; }

int PogoZombie::currentWalkWidth() const {
    switch (getDamageStage()) {
        case 0:
            if (hp <= HEADLESS_THRESHOLD) return POGO_ZOMBIE_POGO_HEADLESS_WIDTH;
            return POGO_ZOMBIE_POGO_FULL_WIDTH;
        case 2:  return POGO_ZOMBIE_WALK_HEADLESS_WIDTH;
        case 3:  return POGO_ZOMBIE_WALK_ARMLESS_WIDTH;
        default: return POGO_ZOMBIE_WALK_FULL_WIDTH;
    }
}

int PogoZombie::currentWalkHeight() const {
    switch (getDamageStage()) {
        case 0:
            if (hp <= HEADLESS_THRESHOLD) return POGO_ZOMBIE_POGO_HEADLESS_HEIGHT;
            return POGO_ZOMBIE_POGO_FULL_HEIGHT;
        case 2:  return POGO_ZOMBIE_WALK_HEADLESS_HEIGHT;
        case 3:  return POGO_ZOMBIE_WALK_ARMLESS_HEIGHT;
        default: return POGO_ZOMBIE_WALK_FULL_HEIGHT;
    }
}

int PogoZombie::currentFightWidth() const {
    if (getDamageStage() >= 2) return POGO_ZOMBIE_FIGHT_HEADLESS_WIDTH;
    return POGO_ZOMBIE_FIGHT_FULL_WIDTH;
}

int PogoZombie::currentFightHeight() const {
    if (getDamageStage() >= 2) return POGO_ZOMBIE_FIGHT_HEADLESS_HEIGHT;
    return POGO_ZOMBIE_FIGHT_FULL_HEIGHT;
}

const unsigned char* PogoZombie::currentDeathFrame(int f) const { return pogo_zombie_death_frames[f]; }
int PogoZombie::currentDeathFrameCount() const { return POGO_ZOMBIE_DEATH_FRAMES; }
int PogoZombie::currentDeathWidth() const { return POGO_ZOMBIE_DEATH_WIDTH; }
int PogoZombie::currentDeathHeight() const { return POGO_ZOMBIE_DEATH_HEIGHT; }
bool PogoZombie::hasDeathAnimation() const { return true; }
