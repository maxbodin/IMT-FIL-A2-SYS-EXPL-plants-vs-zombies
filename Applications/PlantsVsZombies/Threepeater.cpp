#include <Applications/PlantsVsZombies/Threepeater.h>
#include <Applications/PlantsVsZombies/sprites/plants/threepeater/threepeater_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/threepeater/threepeater_shooting_sprite.h>

Threepeater::Threepeater(int x, int y)
    : ShooterPlant(x, y, HP, SHOOT_DELAY), pendingLane(0), staggerDelay(0) {}

void Threepeater::update() {
    ShooterPlant::update();
    if (staggerDelay > 0) staggerDelay--;
}

BulletType Threepeater::getBulletType() const { return BULLET_PEASHOOTER; }
PlantType Threepeater::getPlantType() const { return PLANT_THREEPEATER; }

bool Threepeater::canShoot() const {
    if (state != ALIVE) return false;
    if (pendingLane > 0 && staggerDelay == 0) return true;
    return cooldown == 0;
}

void Threepeater::resetCooldown() {
    if (pendingLane > 0) {
        pendingLane--;
        staggerDelay = SHOT_STAGGER;
        shooting = true;
        shootIdx = 0;
        shootAnimTick = 0;
        shootHoldTicks = shootHold;
        if (pendingLane == 0) {
            cooldown = shootDelay;
        }
    } else {
        pendingLane = 2;
        staggerDelay = SHOT_STAGGER;
        shooting = true;
        shootIdx = 0;
        shootAnimTick = 0;
        shootHoldTicks = shootHold;
    }
}

const unsigned char* Threepeater::idleFrame(int f) const { return threepeater_frames[f]; }
int Threepeater::idleFrameCount() const { return THREEPEATER_FRAMES; }
int Threepeater::spriteWidth() const { return THREEPEATER_WIDTH; }
int Threepeater::spriteHeight() const { return THREEPEATER_HEIGHT; }

const unsigned char* Threepeater::shootingSprite(int f) const { return threepeater_shooting_frames[f]; }
int Threepeater::shootingSpriteCount() const { return THREEPEATER_SHOOTING_FRAMES; }
int Threepeater::shootingSpriteWidth() const { return THREEPEATER_SHOOTING_WIDTH; }
int Threepeater::shootingSpriteHeight() const { return THREEPEATER_SHOOTING_HEIGHT; }
