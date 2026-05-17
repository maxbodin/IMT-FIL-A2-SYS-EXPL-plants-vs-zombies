#include <Applications/PlantsVsZombies/BucketheadZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/buckethead_zombie_walk_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/buckethead_zombie_walk_damaged_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/buckethead_zombie_walk_no_bucket_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/buckethead_zombie_fight_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/buckethead_zombie_fight_damaged_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/buckethead_zombie_fight_no_bucket_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/buckethead_zombie_death_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/buckethead_zombie_death_damaged_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/buckethead_zombie_death_no_bucket_sprite.h>

BucketheadZombie::BucketheadZombie(int x, int y, int speedBonus)
    : Zombie(x, y, BUCKET_HP, speedBonus), lastDamageStage(0) {}

int BucketheadZombie::getDamageStage() const {
    if (state == DYING) return lastDamageStage;
    if (hp > BUCKET_DAMAGED_THRESHOLD) return 0;
    if (hp > BUCKET_LOST_THRESHOLD)    return 1;
    return 2;
}

void BucketheadZombie::onUpdate() {
    if (hp > BUCKET_DAMAGED_THRESHOLD) lastDamageStage = 0;
    else if (hp > BUCKET_LOST_THRESHOLD) lastDamageStage = 1;
    else lastDamageStage = 2;
}

int BucketheadZombie::getWidth()  const { return BUCKETHEAD_ZOMBIE_WALK_FULL_WIDTH; }
int BucketheadZombie::getHeight() const { return BUCKETHEAD_ZOMBIE_WALK_FULL_HEIGHT; }

const unsigned char* BucketheadZombie::currentWalkFrame(int f) const {
    switch (getDamageStage()) {
        case 1:  return buckethead_zombie_walk_damaged_frames[f];
        case 2:  return buckethead_zombie_walk_no_bucket_frames[f];
        default: return buckethead_zombie_walk_full_frames[f];
    }
}

const unsigned char* BucketheadZombie::currentFightFrame(int f) const {
    switch (getDamageStage()) {
        case 1:  return buckethead_zombie_fight_damaged_frames[f];
        case 2:  return buckethead_zombie_fight_no_bucket_frames[f];
        default: return buckethead_zombie_fight_full_frames[f];
    }
}

int BucketheadZombie::currentWalkFrameCount()  const { return BUCKETHEAD_ZOMBIE_WALK_FULL_FRAMES; }
int BucketheadZombie::currentFightFrameCount() const { return BUCKETHEAD_ZOMBIE_FIGHT_FULL_FRAMES; }
int BucketheadZombie::currentWalkWidth()  const { return BUCKETHEAD_ZOMBIE_WALK_FULL_WIDTH; }
int BucketheadZombie::currentWalkHeight() const { return BUCKETHEAD_ZOMBIE_WALK_FULL_HEIGHT; }
int BucketheadZombie::currentFightWidth()  const { return BUCKETHEAD_ZOMBIE_FIGHT_FULL_WIDTH; }
int BucketheadZombie::currentFightHeight() const { return BUCKETHEAD_ZOMBIE_FIGHT_FULL_HEIGHT; }

const unsigned char* BucketheadZombie::currentDeathFrame(int f) const {
    switch (getDamageStage()) {
        case 1:  return buckethead_zombie_death_damaged_frames[f];
        case 2:  return buckethead_zombie_death_no_bucket_frames[f];
        default: return buckethead_zombie_death_full_frames[f];
    }
}

int BucketheadZombie::currentDeathFrameCount() const { return BUCKETHEAD_ZOMBIE_DEATH_FULL_FRAMES; }
int BucketheadZombie::currentDeathWidth()  const { return BUCKETHEAD_ZOMBIE_DEATH_FULL_WIDTH; }
int BucketheadZombie::currentDeathHeight() const { return BUCKETHEAD_ZOMBIE_DEATH_FULL_HEIGHT; }
bool BucketheadZombie::hasDeathAnimation() const { return true; }
