#include <Applications/PlantsVsZombies/YetiZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/yeti_zombie_walk_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/yeti_zombie_walk_damaged_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/yeti_zombie_walk_headless_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/yeti_zombie_walk_no_arm_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/yeti_zombie_fight_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/yeti_zombie_death_sprite.h>

YetiZombie::YetiZombie(int x, int y, int speedBonus)
    : Zombie(x, y, YETI_HP, speedBonus), lastDamageStage(0) {}

int YetiZombie::getDamageStage() const {
    if (state == DYING) return lastDamageStage;
    if (hp > DAMAGED_THRESHOLD)  return 0; // full
    if (hp > HEADLESS_THRESHOLD) return 1; // damaged
    if (hp > NO_ARM_THRESHOLD)   return 2; // headless
    return 3; // no arm
}

void YetiZombie::onUpdate() {
    if (hp > DAMAGED_THRESHOLD)       lastDamageStage = 0;
    else if (hp > HEADLESS_THRESHOLD) lastDamageStage = 1;
    else if (hp > NO_ARM_THRESHOLD)   lastDamageStage = 2;
    else                              lastDamageStage = 3;
}

int YetiZombie::getWidth()  const { return YETI_ZOMBIE_WALK_WIDTH; }
int YetiZombie::getHeight() const { return YETI_ZOMBIE_WALK_HEIGHT; }

const unsigned char* YetiZombie::currentWalkFrame(int f) const {
    switch (getDamageStage()) {
        case 1:  return yeti_zombie_walk_damaged_frames[f % YETI_ZOMBIE_WALK_DAMAGED_FRAMES];
        case 2:  return yeti_zombie_walk_headless_frames[f % YETI_ZOMBIE_WALK_HEADLESS_FRAMES];
        case 3:  return yeti_zombie_walk_no_arm_frames[f % YETI_ZOMBIE_WALK_NO_ARM_FRAMES];
        default: return yeti_zombie_walk_frames[f];
    }
}

const unsigned char* YetiZombie::currentFightFrame(int f) const {
    return yeti_zombie_fight_frames[f];
}

int YetiZombie::currentWalkFrameCount() const { return YETI_ZOMBIE_WALK_FRAMES; }
int YetiZombie::currentFightFrameCount() const { return YETI_ZOMBIE_FIGHT_FRAMES; }

int YetiZombie::currentWalkWidth() const {
    switch (getDamageStage()) {
        case 1:  return YETI_ZOMBIE_WALK_DAMAGED_WIDTH;
        case 2:  return YETI_ZOMBIE_WALK_HEADLESS_WIDTH;
        case 3:  return YETI_ZOMBIE_WALK_NO_ARM_WIDTH;
        default: return YETI_ZOMBIE_WALK_WIDTH;
    }
}

int YetiZombie::currentWalkHeight() const {
    switch (getDamageStage()) {
        case 1:  return YETI_ZOMBIE_WALK_DAMAGED_HEIGHT;
        case 2:  return YETI_ZOMBIE_WALK_HEADLESS_HEIGHT;
        case 3:  return YETI_ZOMBIE_WALK_NO_ARM_HEIGHT;
        default: return YETI_ZOMBIE_WALK_HEIGHT;
    }
}

int YetiZombie::currentFightWidth() const { return YETI_ZOMBIE_FIGHT_WIDTH; }
int YetiZombie::currentFightHeight() const { return YETI_ZOMBIE_FIGHT_HEIGHT; }

const unsigned char* YetiZombie::currentDeathFrame(int f) const { return yeti_zombie_death_frames[f]; }
int YetiZombie::currentDeathFrameCount() const { return YETI_ZOMBIE_DEATH_FRAMES; }
int YetiZombie::currentDeathWidth() const { return YETI_ZOMBIE_DEATH_WIDTH; }
int YetiZombie::currentDeathHeight() const { return YETI_ZOMBIE_DEATH_HEIGHT; }
bool YetiZombie::hasDeathAnimation() const { return true; }
