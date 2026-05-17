#include <Applications/PlantsVsZombies/Threepeater.h>
#include <Applications/PlantsVsZombies/sprites/plants/threepeater/threepeater_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/threepeater/threepeater_shooting_sprite.h>

Threepeater::Threepeater(int x, int y)
    : ShooterPlant(x, y, HP, SHOOT_DELAY) {}

BulletType Threepeater::getBulletType() const { return BULLET_PEASHOOTER; }
PlantType Threepeater::getPlantType() const { return PLANT_THREEPEATER; }

const unsigned char* Threepeater::idleFrame(int f) const { return threepeater_frames[f]; }
int Threepeater::idleFrameCount() const { return THREEPEATER_FRAMES; }
int Threepeater::spriteWidth() const { return THREEPEATER_WIDTH; }
int Threepeater::spriteHeight() const { return THREEPEATER_HEIGHT; }

const unsigned char* Threepeater::shootingSprite(int f) const { return threepeater_shooting_frames[f]; }
int Threepeater::shootingSpriteCount() const { return THREEPEATER_SHOOTING_FRAMES; }
int Threepeater::shootingSpriteWidth() const { return THREEPEATER_SHOOTING_WIDTH; }
int Threepeater::shootingSpriteHeight() const { return THREEPEATER_SHOOTING_HEIGHT; }
