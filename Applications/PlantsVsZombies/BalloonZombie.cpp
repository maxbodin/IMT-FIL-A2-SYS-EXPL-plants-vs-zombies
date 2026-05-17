#include <Applications/PlantsVsZombies/BalloonZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/balloon_zombie_fly_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/balloon_zombie_pop_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/balloon_zombie_walk_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/balloon_zombie_walk_no_arm_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/balloon_zombie_fight_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/balloon_zombie_fight_no_arm_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/balloon_zombie_death_sprite.h>

BalloonZombie::BalloonZombie(int x, int y, int speedBonus)
    : Zombie(x, y, BALLOON_TOTAL_HP, speedBonus), balloonHp(BALLOON_HP),
      balloonPopped(false), popping(false), popFrame(0), popAnimTick(0),
      lastDamageStage(0) {}

int BalloonZombie::getDamageStage() const {
    if (state == DYING) return lastDamageStage;
    if (!balloonPopped) return 0;           // flying
    if (hp > NO_ARM_THRESHOLD) return 1;    // walking full
    return 2;                               // no arm
}

void BalloonZombie::onUpdate() {
    if (!balloonPopped && balloonHp > 0) {
        int damageTaken = BALLOON_TOTAL_HP - hp;
        if (damageTaken >= BALLOON_HP) {
            balloonHp = 0;
            popping = true;
            popFrame = 0;
            popAnimTick = 0;
        }
    }

    if (popping) {
        ++popAnimTick;
        if (popAnimTick >= POP_ANIM_SPEED) {
            popAnimTick = 0;
            ++popFrame;
            if (popFrame >= BALLOON_ZOMBIE_POP_FRAMES) {
                popping = false;
                balloonPopped = true;
            }
        }
    }

    if (!balloonPopped)             lastDamageStage = 0;
    else if (hp > NO_ARM_THRESHOLD) lastDamageStage = 1;
    else                            lastDamageStage = 2;
}

bool BalloonZombie::canBeBlocked() const {
    return balloonPopped;
}

bool BalloonZombie::isFlying() const {
    return !balloonPopped && !popping;
}

int BalloonZombie::getWidth() const {
    if (popping) return BALLOON_ZOMBIE_POP_WIDTH;
    if (!balloonPopped) return BALLOON_ZOMBIE_FLY_WIDTH;
    return BALLOON_ZOMBIE_WALK_WIDTH;
}

int BalloonZombie::getHeight() const {
    if (popping) return BALLOON_ZOMBIE_POP_HEIGHT;
    if (!balloonPopped) return BALLOON_ZOMBIE_FLY_HEIGHT;
    return BALLOON_ZOMBIE_WALK_HEIGHT;
}

const unsigned char* BalloonZombie::currentWalkFrame(int f) const {
    if (popping)
        return balloon_zombie_pop_frames[popFrame % BALLOON_ZOMBIE_POP_FRAMES];
    if (!balloonPopped)
        return balloon_zombie_fly_frames[f];
    if (getDamageStage() == 2)
        return balloon_zombie_walk_no_arm_frames[f % BALLOON_ZOMBIE_WALK_NO_ARM_FRAMES];
    return balloon_zombie_walk_frames[f];
}

const unsigned char* BalloonZombie::currentFightFrame(int f) const {
    if (getDamageStage() == 2)
        return balloon_zombie_fight_no_arm_frames[f % BALLOON_ZOMBIE_FIGHT_NO_ARM_FRAMES];
    return balloon_zombie_fight_frames[f];
}

int BalloonZombie::currentWalkFrameCount() const {
    if (popping) return BALLOON_ZOMBIE_POP_FRAMES;
    if (!balloonPopped) return BALLOON_ZOMBIE_FLY_FRAMES;
    return BALLOON_ZOMBIE_WALK_FRAMES;
}

int BalloonZombie::currentFightFrameCount() const { return BALLOON_ZOMBIE_FIGHT_FRAMES; }

int BalloonZombie::currentWalkWidth() const {
    if (popping) return BALLOON_ZOMBIE_POP_WIDTH;
    if (!balloonPopped) return BALLOON_ZOMBIE_FLY_WIDTH;
    if (getDamageStage() == 2) return BALLOON_ZOMBIE_WALK_NO_ARM_WIDTH;
    return BALLOON_ZOMBIE_WALK_WIDTH;
}

int BalloonZombie::currentWalkHeight() const {
    if (popping) return BALLOON_ZOMBIE_POP_HEIGHT;
    if (!balloonPopped) return BALLOON_ZOMBIE_FLY_HEIGHT;
    if (getDamageStage() == 2) return BALLOON_ZOMBIE_WALK_NO_ARM_HEIGHT;
    return BALLOON_ZOMBIE_WALK_HEIGHT;
}

int BalloonZombie::currentFightWidth() const {
    if (getDamageStage() == 2) return BALLOON_ZOMBIE_FIGHT_NO_ARM_WIDTH;
    return BALLOON_ZOMBIE_FIGHT_WIDTH;
}

int BalloonZombie::currentFightHeight() const {
    if (getDamageStage() == 2) return BALLOON_ZOMBIE_FIGHT_NO_ARM_HEIGHT;
    return BALLOON_ZOMBIE_FIGHT_HEIGHT;
}

const unsigned char* BalloonZombie::currentDeathFrame(int f) const { return balloon_zombie_death_frames[f]; }
int BalloonZombie::currentDeathFrameCount() const { return BALLOON_ZOMBIE_DEATH_FRAMES; }
int BalloonZombie::currentDeathWidth() const { return BALLOON_ZOMBIE_DEATH_WIDTH; }
int BalloonZombie::currentDeathHeight() const { return BALLOON_ZOMBIE_DEATH_HEIGHT; }
bool BalloonZombie::hasDeathAnimation() const { return true; }
