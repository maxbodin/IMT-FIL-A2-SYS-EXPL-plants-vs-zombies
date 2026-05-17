#include <Applications/PlantsVsZombies/ScreenDoorZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/screendoor/screendoor_zombie_walk_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/screendoor/screendoor_zombie_walk_door_damaged_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/basic/basic_zombie_walk_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/screendoor/screendoor_zombie_fight_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/screendoor/screendoor_zombie_fight_door_damaged_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/basic/basic_zombie_fight_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/basic/basic_zombie_death_sprite.h>

ScreenDoorZombie::ScreenDoorZombie(int x, int y, int speedBonus)
    : Zombie(x, y, SCREENDOOR_HP, speedBonus), lastDamageStage(0) {}

int ScreenDoorZombie::getDamageStage() const {
    if (state == DYING) return lastDamageStage;
    if (hp > DAMAGED_THRESHOLD)  return 0; // full
    if (hp > NO_DOOR_THRESHOLD)  return 1; // damaged
    return 2; // no door
}

void ScreenDoorZombie::onUpdate() {
    if (hp > DAMAGED_THRESHOLD)       lastDamageStage = 0;
    else if (hp > NO_DOOR_THRESHOLD)  lastDamageStage = 1;
    else                              lastDamageStage = 2;
}

int ScreenDoorZombie::getWidth()  const { return SCREENDOOR_ZOMBIE_WALK_FULL_WIDTH; }
int ScreenDoorZombie::getHeight() const { return SCREENDOOR_ZOMBIE_WALK_FULL_HEIGHT; }

const unsigned char* ScreenDoorZombie::currentWalkFrame(int f) const {
    switch (getDamageStage()) {
        case 1:  return screendoor_zombie_walk_door_damaged_frames[f % SCREENDOOR_ZOMBIE_WALK_DOOR_DAMAGED_FRAMES];
        case 2:  return basic_zombie_walk_full_frames[f % BASIC_ZOMBIE_WALK_FULL_FRAMES];
        default: return screendoor_zombie_walk_full_frames[f];
    }
}

const unsigned char* ScreenDoorZombie::currentFightFrame(int f) const {
    switch (getDamageStage()) {
        case 1:  return screendoor_zombie_fight_door_damaged_frames[f % SCREENDOOR_ZOMBIE_FIGHT_DOOR_DAMAGED_FRAMES];
        case 2:  return basic_zombie_fight_full_frames[f % BASIC_ZOMBIE_FIGHT_FULL_FRAMES];
        default: return screendoor_zombie_fight_full_frames[f];
    }
}

int ScreenDoorZombie::currentWalkFrameCount() const { return SCREENDOOR_ZOMBIE_WALK_FULL_FRAMES; }
int ScreenDoorZombie::currentFightFrameCount() const { return SCREENDOOR_ZOMBIE_FIGHT_FULL_FRAMES; }

int ScreenDoorZombie::currentWalkWidth() const {
    switch (getDamageStage()) {
        case 1:  return SCREENDOOR_ZOMBIE_WALK_DOOR_DAMAGED_WIDTH;
        case 2:  return BASIC_ZOMBIE_WALK_FULL_WIDTH;
        default: return SCREENDOOR_ZOMBIE_WALK_FULL_WIDTH;
    }
}

int ScreenDoorZombie::currentWalkHeight() const {
    switch (getDamageStage()) {
        case 1:  return SCREENDOOR_ZOMBIE_WALK_DOOR_DAMAGED_HEIGHT;
        case 2:  return BASIC_ZOMBIE_WALK_FULL_HEIGHT;
        default: return SCREENDOOR_ZOMBIE_WALK_FULL_HEIGHT;
    }
}

int ScreenDoorZombie::currentFightWidth() const {
    switch (getDamageStage()) {
        case 1:  return SCREENDOOR_ZOMBIE_FIGHT_DOOR_DAMAGED_WIDTH;
        case 2:  return BASIC_ZOMBIE_FIGHT_FULL_WIDTH;
        default: return SCREENDOOR_ZOMBIE_FIGHT_FULL_WIDTH;
    }
}

int ScreenDoorZombie::currentFightHeight() const {
    switch (getDamageStage()) {
        case 1:  return SCREENDOOR_ZOMBIE_FIGHT_DOOR_DAMAGED_HEIGHT;
        case 2:  return BASIC_ZOMBIE_FIGHT_FULL_HEIGHT;
        default: return SCREENDOOR_ZOMBIE_FIGHT_FULL_HEIGHT;
    }
}

const unsigned char* ScreenDoorZombie::currentDeathFrame(int f) const {
    switch (getDamageStage()) {
        case 1:  return basic_zombie_death_frames[f];
        case 2:  return basic_zombie_death_frames[f];
        default: return basic_zombie_death_frames[f];
    }
}

int ScreenDoorZombie::currentDeathFrameCount() const {
    switch (getDamageStage()) {
        case 1:  return BASIC_ZOMBIE_DEATH_FRAMES;
        case 2:  return BASIC_ZOMBIE_DEATH_FRAMES;
        default: return BASIC_ZOMBIE_DEATH_FRAMES;
    }
}

int ScreenDoorZombie::currentDeathWidth() const {
    switch (getDamageStage()) {
        case 1:  return BASIC_ZOMBIE_DEATH_WIDTH;
        case 2:  return BASIC_ZOMBIE_DEATH_WIDTH;
        default: return BASIC_ZOMBIE_DEATH_WIDTH;
    }
}

int ScreenDoorZombie::currentDeathHeight() const {
    switch (getDamageStage()) {
        case 1:  return BASIC_ZOMBIE_DEATH_HEIGHT;
        case 2:  return BASIC_ZOMBIE_DEATH_HEIGHT;
        default: return BASIC_ZOMBIE_DEATH_HEIGHT;
    }
}

bool ScreenDoorZombie::hasDeathAnimation() const { return true; }
