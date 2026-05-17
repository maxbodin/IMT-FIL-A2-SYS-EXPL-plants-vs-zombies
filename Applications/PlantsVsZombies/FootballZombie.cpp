#include <Applications/PlantsVsZombies/FootballZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/football_zombie_walk_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/football_zombie_walk_damage1_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/football_zombie_walk_damage2_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/football_zombie_walk_no_helmet_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/football_zombie_walk_headless_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/football_zombie_walk_armless_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/football_zombie_fight_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/football_zombie_fight_damage_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/football_zombie_fight_no_helmet_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/football_zombie_fight_headless_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/football_zombie_fight_armless_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/football_zombie_death_sprite.h>

FootballZombie::FootballZombie(int x, int y, int speedBonus)
    : Zombie(x, y, FOOTBALL_HP, speedBonus + SPEED_BONUS), lastDamageStage(0) {}

int FootballZombie::getDamageStage() const {
    if (state == DYING) return lastDamageStage;
    if (hp > DAMAGE1_THRESHOLD)     return 0;
    if (hp > DAMAGE2_THRESHOLD)     return 1;
    if (hp > NO_HELMET_THRESHOLD)   return 2;
    if (hp > HEADLESS_THRESHOLD)    return 3;
    if (hp > ARMLESS_THRESHOLD)     return 4;
    return 5;
}

int FootballZombie::getWidth()  const { return FOOTBALL_ZOMBIE_WALK_FULL_WIDTH; }
int FootballZombie::getHeight() const { return FOOTBALL_ZOMBIE_WALK_FULL_HEIGHT; }

void FootballZombie::onUpdate() {
    if (hp > DAMAGE1_THRESHOLD)        lastDamageStage = 0;
    else if (hp > DAMAGE2_THRESHOLD)   lastDamageStage = 1;
    else if (hp > NO_HELMET_THRESHOLD) lastDamageStage = 2;
    else if (hp > HEADLESS_THRESHOLD)  lastDamageStage = 3;
    else if (hp > ARMLESS_THRESHOLD)   lastDamageStage = 4;
    else                               lastDamageStage = 5;
}

const unsigned char* FootballZombie::currentWalkFrame(int f) const {
    switch (getDamageStage()) {
        case 1:  return football_zombie_walk_damage1_frames[f % FOOTBALL_ZOMBIE_WALK_DAMAGE1_FRAMES];
        case 2:  return football_zombie_walk_damage2_frames[f % FOOTBALL_ZOMBIE_WALK_DAMAGE2_FRAMES];
        case 3:  return football_zombie_walk_no_helmet_frames[f % FOOTBALL_ZOMBIE_WALK_NO_HELMET_FRAMES];
        case 4:  return football_zombie_walk_headless_frames[f % FOOTBALL_ZOMBIE_WALK_HEADLESS_FRAMES];
        case 5:  return football_zombie_walk_armless_frames[f % FOOTBALL_ZOMBIE_WALK_ARMLESS_FRAMES];
        default: return football_zombie_walk_full_frames[f];
    }
}

const unsigned char* FootballZombie::currentFightFrame(int f) const {
    switch (getDamageStage()) {
        case 1:
        case 2:  return football_zombie_fight_damage_frames[f % FOOTBALL_ZOMBIE_FIGHT_DAMAGE_FRAMES];
        case 3:  return football_zombie_fight_no_helmet_frames[f % FOOTBALL_ZOMBIE_FIGHT_NO_HELMET_FRAMES];
        case 4:  return football_zombie_fight_headless_frames[f % FOOTBALL_ZOMBIE_FIGHT_HEADLESS_FRAMES];
        case 5:  return football_zombie_fight_armless_frames[f % FOOTBALL_ZOMBIE_FIGHT_ARMLESS_FRAMES];
        default: return football_zombie_fight_full_frames[f];
    }
}

int FootballZombie::currentWalkFrameCount() const {
    switch (getDamageStage()) {
        case 3:  return FOOTBALL_ZOMBIE_WALK_NO_HELMET_FRAMES;
        case 4:  return FOOTBALL_ZOMBIE_WALK_HEADLESS_FRAMES;
        case 5:  return FOOTBALL_ZOMBIE_WALK_ARMLESS_FRAMES;
        default: return FOOTBALL_ZOMBIE_WALK_FULL_FRAMES;
    }
}

int FootballZombie::currentFightFrameCount() const {
    switch (getDamageStage()) {
        case 3:  return FOOTBALL_ZOMBIE_FIGHT_NO_HELMET_FRAMES;
        case 4:  return FOOTBALL_ZOMBIE_FIGHT_HEADLESS_FRAMES;
        case 5:  return FOOTBALL_ZOMBIE_FIGHT_ARMLESS_FRAMES;
        default: return FOOTBALL_ZOMBIE_FIGHT_FULL_FRAMES;
    }
}

int FootballZombie::currentWalkWidth() const {
    switch (getDamageStage()) {
        case 3:  return FOOTBALL_ZOMBIE_WALK_NO_HELMET_WIDTH;
        case 4:  return FOOTBALL_ZOMBIE_WALK_HEADLESS_WIDTH;
        case 5:  return FOOTBALL_ZOMBIE_WALK_ARMLESS_WIDTH;
        default: return FOOTBALL_ZOMBIE_WALK_FULL_WIDTH;
    }
}

int FootballZombie::currentWalkHeight() const {
    switch (getDamageStage()) {
        case 3:  return FOOTBALL_ZOMBIE_WALK_NO_HELMET_HEIGHT;
        case 4:  return FOOTBALL_ZOMBIE_WALK_HEADLESS_HEIGHT;
        case 5:  return FOOTBALL_ZOMBIE_WALK_ARMLESS_HEIGHT;
        default: return FOOTBALL_ZOMBIE_WALK_FULL_HEIGHT;
    }
}

int FootballZombie::currentFightWidth() const {
    switch (getDamageStage()) {
        case 3:  return FOOTBALL_ZOMBIE_FIGHT_NO_HELMET_WIDTH;
        case 4:  return FOOTBALL_ZOMBIE_FIGHT_HEADLESS_WIDTH;
        case 5:  return FOOTBALL_ZOMBIE_FIGHT_ARMLESS_WIDTH;
        default: return FOOTBALL_ZOMBIE_FIGHT_FULL_WIDTH;
    }
}

int FootballZombie::currentFightHeight() const {
    switch (getDamageStage()) {
        case 3:  return FOOTBALL_ZOMBIE_FIGHT_NO_HELMET_HEIGHT;
        case 4:  return FOOTBALL_ZOMBIE_FIGHT_HEADLESS_HEIGHT;
        case 5:  return FOOTBALL_ZOMBIE_FIGHT_ARMLESS_HEIGHT;
        default: return FOOTBALL_ZOMBIE_FIGHT_FULL_HEIGHT;
    }
}

const unsigned char* FootballZombie::currentDeathFrame(int f) const { return football_zombie_death_frames[f]; }
int FootballZombie::currentDeathFrameCount() const { return FOOTBALL_ZOMBIE_DEATH_FRAMES; }
int FootballZombie::currentDeathWidth() const { return FOOTBALL_ZOMBIE_DEATH_WIDTH; }
int FootballZombie::currentDeathHeight() const { return FOOTBALL_ZOMBIE_DEATH_HEIGHT; }
bool FootballZombie::hasDeathAnimation() const { return true; }
