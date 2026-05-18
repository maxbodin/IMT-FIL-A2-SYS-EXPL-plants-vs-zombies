#include <Applications/PlantsVsZombies/BaseballZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/baseball/baseball_zombie_walk_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/baseball/baseball_zombie_walk_no_arm_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/baseball/baseball_zombie_walk_no_head_no_arm_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/baseball/baseball_zombie_fight_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/baseball/baseball_zombie_fight_no_arm_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/baseball/baseball_zombie_fight_no_head_no_arm_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/baseball/baseball_zombie_death_sprite.h>

BaseballZombie::BaseballZombie(int x, int y, int speedBonus)
    : Zombie(x, y, BASEBALL_HP, speedBonus + SPEED_BONUS), lastDamageStage(0) {}

int BaseballZombie::getDamageStage() const {
    if (state == DYING) return lastDamageStage;
    if (hp > DAMAGED_THRESHOLD) return 0;
    if (hp > NO_BAT_THRESHOLD)  return 1;
    if (hp > HEADLESS_THRESHOLD) return 2;
    return 3;
}

void BaseballZombie::onUpdate() {
    if (hp > DAMAGED_THRESHOLD) lastDamageStage = 0;
    else if (hp > NO_BAT_THRESHOLD) lastDamageStage = 1;
    else if (hp > HEADLESS_THRESHOLD) lastDamageStage = 2;
    else lastDamageStage = 3;
}

int BaseballZombie::getWidth()  const { return BASEBALL_ZOMBIE_WALK_FULL_WIDTH; }
int BaseballZombie::getHeight() const { return BASEBALL_ZOMBIE_WALK_FULL_HEIGHT; }

const unsigned char* BaseballZombie::currentWalkFrame(int f) const {
    switch (getDamageStage()) {
        case 1:  return baseball_zombie_walk_no_arm_frames[f];
        case 2:  return baseball_zombie_walk_no_arm_frames[f];
        case 3:  return baseball_zombie_walk_no_head_no_arm_frames[f % BASEBALL_ZOMBIE_WALK_NO_HEAD_NO_ARM_FRAMES];
        default: return baseball_zombie_walk_full_frames[f];
    }
}

const unsigned char* BaseballZombie::currentFightFrame(int f) const {
    switch (getDamageStage()) {
        case 1:  return baseball_zombie_fight_no_arm_frames[f];
        case 2:  return baseball_zombie_fight_no_head_no_arm_frames[f];
        case 3:  return baseball_zombie_fight_no_head_no_arm_frames[f];
        default: return baseball_zombie_fight_full_frames[f];
    }
}

int BaseballZombie::currentWalkFrameCount() const { return BASEBALL_ZOMBIE_WALK_FULL_FRAMES; }
int BaseballZombie::currentFightFrameCount() const {
    if (getDamageStage() >= 2) return BASEBALL_ZOMBIE_FIGHT_NO_HEAD_NO_ARM_FRAMES;
    if (getDamageStage() == 1) return BASEBALL_ZOMBIE_FIGHT_NO_ARM_FRAMES;
    return BASEBALL_ZOMBIE_FIGHT_FULL_FRAMES;
}
int BaseballZombie::currentWalkWidth() const {
    if (getDamageStage() == 3) return BASEBALL_ZOMBIE_WALK_NO_HEAD_NO_ARM_WIDTH;
    if (getDamageStage() == 1 || getDamageStage() == 2) return BASEBALL_ZOMBIE_WALK_NO_ARM_WIDTH;
    return BASEBALL_ZOMBIE_WALK_FULL_WIDTH;
}
int BaseballZombie::currentWalkHeight() const {
    if (getDamageStage() == 3) return BASEBALL_ZOMBIE_WALK_NO_HEAD_NO_ARM_HEIGHT;
    if (getDamageStage() == 1 || getDamageStage() == 2) return BASEBALL_ZOMBIE_WALK_NO_ARM_HEIGHT;
    return BASEBALL_ZOMBIE_WALK_FULL_HEIGHT;
}
int BaseballZombie::currentFightWidth() const {
    if (getDamageStage() >= 2) return BASEBALL_ZOMBIE_FIGHT_NO_HEAD_NO_ARM_WIDTH;
    if (getDamageStage() == 1) return BASEBALL_ZOMBIE_FIGHT_NO_ARM_WIDTH;
    return BASEBALL_ZOMBIE_FIGHT_FULL_WIDTH;
}
int BaseballZombie::currentFightHeight() const {
    if (getDamageStage() >= 2) return BASEBALL_ZOMBIE_FIGHT_NO_HEAD_NO_ARM_HEIGHT;
    if (getDamageStage() == 1) return BASEBALL_ZOMBIE_FIGHT_NO_ARM_HEIGHT;
    return BASEBALL_ZOMBIE_FIGHT_FULL_HEIGHT;
}

const unsigned char* BaseballZombie::currentDeathFrame(int f) const {
    if (getDamageStage() >= 1) return baseball_zombie_death_frames[f % BASEBALL_ZOMBIE_DEATH_FRAMES];
    return baseball_zombie_death_frames[f];
}
int BaseballZombie::currentDeathFrameCount() const {
    if (getDamageStage() >= 1) return BASEBALL_ZOMBIE_DEATH_FRAMES;
    return BASEBALL_ZOMBIE_DEATH_FRAMES;
}
int BaseballZombie::currentDeathWidth() const {
    if (getDamageStage() >= 1) return BASEBALL_ZOMBIE_DEATH_WIDTH;
    return BASEBALL_ZOMBIE_DEATH_WIDTH;
}
int BaseballZombie::currentDeathHeight() const {
    if (getDamageStage() >= 1) return BASEBALL_ZOMBIE_DEATH_HEIGHT;
    return BASEBALL_ZOMBIE_DEATH_HEIGHT;
}
bool BaseballZombie::hasDeathAnimation() const { return true; }
