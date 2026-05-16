#include <Applications/PlantsVsZombies/Repeater.h>
#include <Applications/PlantsVsZombies/sprites/plants/reapeater_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/reapeater_shooting_sprite.h>

static const int SECOND_SHOT_GAP = 12;

Repeater::Repeater(int x, int y)
    : ShooterPlant(x, y, HP, SHOOT_DELAY), secondShotDelay(0) {}

void Repeater::update() {
    ShooterPlant::update();
    if (secondShotDelay > 0) secondShotDelay--;
}

BulletType Repeater::getBulletType() const { return BULLET_PEASHOOTER; }
PlantType Repeater::getPlantType() const { return PLANT_REPEATER; }

bool Repeater::canShoot() const {
    if (state != ALIVE) return false;
    return cooldown == 0 || secondShotDelay == 1;
}

void Repeater::resetCooldown() {
    if (secondShotDelay > 0) {
        secondShotDelay = 0;
        cooldown = shootDelay;
        shooting = true;
        shootIdx = 0;
        shootAnimTick = 0;
        shootHoldTicks = shootHold;
    } else {
        secondShotDelay = SECOND_SHOT_GAP;
    }
}

const unsigned char* Repeater::idleFrame(int f) const { return reapeater_frames[f]; }
int Repeater::idleFrameCount() const { return REAPEATER_FRAMES; }
int Repeater::spriteWidth() const { return REAPEATER_WIDTH; }
int Repeater::spriteHeight() const { return REAPEATER_HEIGHT; }

const unsigned char* Repeater::shootingSprite(int f) const { return reapeater_shooting_frames[f]; }
int Repeater::shootingSpriteCount() const { return REAPEATER_SHOOTING_FRAMES; }
int Repeater::shootingSpriteWidth() const { return REAPEATER_SHOOTING_WIDTH; }
int Repeater::shootingSpriteHeight() const { return REAPEATER_SHOOTING_HEIGHT; }
