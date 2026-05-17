#include <Applications/PlantsVsZombies/BackupDancerZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/backupdancer_zombie_walk_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/backupdancer_zombie_walk_damaged_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/backupdancer_zombie_walk_no_arm_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/backupdancer_zombie_fight_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/backupdancer_zombie_fight_damaged_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/backupdancer_zombie_fight_no_arm_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/backupdancer_zombie_dance_walk_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/backupdancer_zombie_dance_walk_damaged_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/backupdancer_zombie_dance_walk_no_arm_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/backupdancer_zombie_rising_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/backupdancer_zombie_death_sprite.h>
#include <vga/vga.h>

BackupDancerZombie::BackupDancerZombie(int x, int y, int speedBonus)
    : Zombie(x, y, DANCER_HP, speedBonus), lastDamageStage(0),
      rising(true), risingFrame(0), risingAnimTick(0) {}

int BackupDancerZombie::getDamageStage() const {
    if (state == DYING) return lastDamageStage;
    if (hp > DAMAGED_THRESHOLD) return 0;
    if (hp > NO_ARM_THRESHOLD)  return 1;
    return 2;
}

void BackupDancerZombie::onUpdate() {
    if (hp > DAMAGED_THRESHOLD) lastDamageStage = 0;
    else if (hp > NO_ARM_THRESHOLD) lastDamageStage = 1;
    else lastDamageStage = 2;
}

void BackupDancerZombie::update() {
    /* Rising animation plays before normal zombie behavior */
    if (rising) {
        if (++risingAnimTick >= RISING_ANIM_SPEED) {
            risingAnimTick = 0;
            risingFrame++;
            if (risingFrame >= BACKUPDANCER_ZOMBIE_RISING_FRAMES) {
                rising = false;
            }
        }
        return;
    }
    Zombie::update();
}

void BackupDancerZombie::render() {
    if (state == DEAD) return;

    if (rising) {
        int rf = risingFrame < BACKUPDANCER_ZOMBIE_RISING_FRAMES
                 ? risingFrame : BACKUPDANCER_ZOMBIE_RISING_FRAMES - 1;
        draw_sprite(backupdancer_zombie_rising_frames[rf],
                    BACKUPDANCER_ZOMBIE_RISING_WIDTH,
                    BACKUPDANCER_ZOMBIE_RISING_HEIGHT, x, y);
        return;
    }
    Zombie::render();
}

bool BackupDancerZombie::isRising() const { return rising; }

int BackupDancerZombie::getWidth()  const { return BACKUPDANCER_ZOMBIE_WALK_FULL_WIDTH; }
int BackupDancerZombie::getHeight() const { return BACKUPDANCER_ZOMBIE_WALK_FULL_HEIGHT; }

const unsigned char* BackupDancerZombie::currentWalkFrame(int f) const {
    switch (getDamageStage()) {
        case 1:  return backupdancer_zombie_dance_walk_damaged_frames[f % BACKUPDANCER_ZOMBIE_DANCE_WALK_DAMAGED_FRAMES];
        case 2:  return backupdancer_zombie_dance_walk_no_arm_frames[f % BACKUPDANCER_ZOMBIE_DANCE_WALK_NO_ARM_FRAMES];
        default: return backupdancer_zombie_dance_walk_frames[f % BACKUPDANCER_ZOMBIE_DANCE_WALK_FRAMES];
    }
}

const unsigned char* BackupDancerZombie::currentFightFrame(int f) const {
    switch (getDamageStage()) {
        case 1:  return backupdancer_zombie_fight_damaged_frames[f];
        case 2:  return backupdancer_zombie_fight_no_arm_frames[f];
        default: return backupdancer_zombie_fight_full_frames[f];
    }
}

int BackupDancerZombie::currentWalkFrameCount() const { return BACKUPDANCER_ZOMBIE_DANCE_WALK_FRAMES; }
int BackupDancerZombie::currentFightFrameCount() const { return BACKUPDANCER_ZOMBIE_FIGHT_FULL_FRAMES; }
int BackupDancerZombie::currentWalkWidth() const { return BACKUPDANCER_ZOMBIE_DANCE_WALK_WIDTH; }
int BackupDancerZombie::currentWalkHeight() const { return BACKUPDANCER_ZOMBIE_DANCE_WALK_HEIGHT; }
int BackupDancerZombie::currentFightWidth() const { return BACKUPDANCER_ZOMBIE_FIGHT_FULL_WIDTH; }
int BackupDancerZombie::currentFightHeight() const { return BACKUPDANCER_ZOMBIE_FIGHT_FULL_HEIGHT; }

const unsigned char* BackupDancerZombie::currentDeathFrame(int f) const { return backupdancer_zombie_death_frames[f]; }
int BackupDancerZombie::currentDeathFrameCount() const { return BACKUPDANCER_ZOMBIE_DEATH_FRAMES; }
int BackupDancerZombie::currentDeathWidth() const { return BACKUPDANCER_ZOMBIE_DEATH_WIDTH; }
int BackupDancerZombie::currentDeathHeight() const { return BACKUPDANCER_ZOMBIE_DEATH_HEIGHT; }
bool BackupDancerZombie::hasDeathAnimation() const { return true; }
