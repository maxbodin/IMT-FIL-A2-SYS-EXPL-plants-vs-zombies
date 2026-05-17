#include <Applications/PlantsVsZombies/DiscoZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/disco_zombie_walk_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/disco_zombie_walk_headless_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/disco_zombie_walk_armless_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/disco_zombie_walk_armless_headless_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/disco_zombie_dance_walk_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/disco_zombie_fight_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/disco_zombie_fight_headless_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/disco_zombie_death_sprite.h>

DiscoZombie::DiscoZombie(int x, int y, int speedBonus)
    : Zombie(x, y, DISCO_HP, speedBonus), summonTriggered(false),
      summonPending(false), dancing(true), lastDamageStage(0) {}

int DiscoZombie::getDamageStage() const {
    if (state == DYING) return lastDamageStage;
    if (hp > HEADLESS_THRESHOLD)          return 0; // full
    if (hp > ARMLESS_THRESHOLD)           return 1; // headless
    if (hp > ARMLESS_HEADLESS_THRESHOLD)  return 2; // armless
    return 3; // armless + headless
}

void DiscoZombie::onUpdate() {
    if (hp > HEADLESS_THRESHOLD)          lastDamageStage = 0;
    else if (hp > ARMLESS_THRESHOLD)      lastDamageStage = 1;
    else if (hp > ARMLESS_HEADLESS_THRESHOLD) lastDamageStage = 2;
    else lastDamageStage = 3;

    if (!summonTriggered && x <= SUMMON_X) {
        summonTriggered = true;
        summonPending   = true;
        dancing         = false; // stop dancing after summoning
    }
}

int DiscoZombie::getWidth()  const { return DISCO_ZOMBIE_WALK_FULL_WIDTH; }
int DiscoZombie::getHeight() const { return DISCO_ZOMBIE_WALK_FULL_HEIGHT; }

const unsigned char* DiscoZombie::currentWalkFrame(int f) const {
    if (dancing) return disco_zombie_dance_walk_frames[f % DISCO_ZOMBIE_DANCE_WALK_FRAMES];
    switch (getDamageStage()) {
        case 1:  return disco_zombie_walk_headless_frames[f % DISCO_ZOMBIE_WALK_HEADLESS_FRAMES];
        case 2:  return disco_zombie_walk_armless_frames[f % DISCO_ZOMBIE_WALK_ARMLESS_FRAMES];
        case 3:  return disco_zombie_walk_armless_headless_frames[f % DISCO_ZOMBIE_WALK_ARMLESS_HEADLESS_FRAMES];
        default: return disco_zombie_walk_full_frames[f];
    }
}

const unsigned char* DiscoZombie::currentFightFrame(int f) const {
    switch (getDamageStage()) {
        case 1:
        case 2:
        case 3:  return disco_zombie_fight_headless_frames[f % DISCO_ZOMBIE_FIGHT_HEADLESS_FRAMES];
        default: return disco_zombie_fight_full_frames[f];
    }
    if (!summonTriggered && x <= SUMMON_X) {
        summonTriggered = true;
        summonPending   = true;
        dancing         = false; // stop dancing after summoning
    }
}

int DiscoZombie::getWidth()  const { return DISCO_ZOMBIE_WALK_FULL_WIDTH; }
int DiscoZombie::getHeight() const { return DISCO_ZOMBIE_WALK_FULL_HEIGHT; }

const unsigned char* DiscoZombie::currentWalkFrame(int f) const {
    if (dancing) return disco_zombie_dance_walk_frames[f % DISCO_ZOMBIE_DANCE_WALK_FRAMES];
    switch (getDamageStage()) {
        case 1:  return disco_zombie_walk_headless_frames[f % DISCO_ZOMBIE_WALK_HEADLESS_FRAMES];
        case 2:  return disco_zombie_walk_armless_frames[f % DISCO_ZOMBIE_WALK_ARMLESS_FRAMES];
        case 3:  return disco_zombie_walk_armless_headless_frames[f % DISCO_ZOMBIE_WALK_ARMLESS_HEADLESS_FRAMES];
        default: return disco_zombie_walk_full_frames[f];
    }
}

const unsigned char* DiscoZombie::currentFightFrame(int f) const {
    switch (getDamageStage()) {
        case 1:
        case 2:
        case 3:  return disco_zombie_fight_headless_frames[f % DISCO_ZOMBIE_FIGHT_HEADLESS_FRAMES];
        default: return disco_zombie_fight_full_frames[f];
    }
}

int DiscoZombie::currentWalkFrameCount() const {
    if (dancing) return DISCO_ZOMBIE_DANCE_WALK_FRAMES;
    return DISCO_ZOMBIE_WALK_FULL_FRAMES;
}

int DiscoZombie::currentFightFrameCount() const { return DISCO_ZOMBIE_FIGHT_FULL_FRAMES; }

int DiscoZombie::currentWalkWidth() const {
    if (dancing) return DISCO_ZOMBIE_DANCE_WALK_WIDTH;
    return DISCO_ZOMBIE_WALK_FULL_WIDTH;
}

int DiscoZombie::currentWalkHeight() const {
    if (dancing) return DISCO_ZOMBIE_DANCE_WALK_HEIGHT;
    return DISCO_ZOMBIE_WALK_FULL_HEIGHT;
}

int DiscoZombie::currentFightWidth() const { return DISCO_ZOMBIE_FIGHT_FULL_WIDTH; }
int DiscoZombie::currentFightHeight() const { return DISCO_ZOMBIE_FIGHT_FULL_HEIGHT; }

const unsigned char* DiscoZombie::currentDeathFrame(int f) const { return disco_zombie_death_frames[f]; }
int DiscoZombie::currentDeathFrameCount() const { return DISCO_ZOMBIE_DEATH_FRAMES; }
int DiscoZombie::currentDeathWidth() const { return DISCO_ZOMBIE_DEATH_WIDTH; }
int DiscoZombie::currentDeathHeight() const { return DISCO_ZOMBIE_DEATH_HEIGHT; }
bool DiscoZombie::hasDeathAnimation() const { return true; }

bool DiscoZombie::hasPendingSummon() const { return summonPending; }
void DiscoZombie::consumeSummon() { summonPending = false; }
