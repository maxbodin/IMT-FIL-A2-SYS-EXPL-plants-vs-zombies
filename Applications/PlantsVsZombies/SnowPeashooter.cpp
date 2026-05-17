#include <Applications/PlantsVsZombies/SnowPeashooter.h>
#include <Applications/PlantsVsZombies/sprites/plants/snow_peashooter/snow_peashooter_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/snow_peashooter/snow_peashooter_shooting_sprite.h>

SnowPeashooter::SnowPeashooter(int x, int y)
    : ShooterPlant(x, y, HP, SHOOT_DELAY) {}

BulletType SnowPeashooter::getBulletType() const { return BULLET_SNOW_PEA; }
PlantType SnowPeashooter::getPlantType() const { return PLANT_SNOW_PEASHOOTER; }

const unsigned char* SnowPeashooter::idleFrame(int f) const { return snow_peashooter_frames[f]; }
int SnowPeashooter::idleFrameCount() const { return SNOW_PEASHOOTER_FRAMES; }
int SnowPeashooter::spriteWidth() const { return SNOW_PEASHOOTER_WIDTH; }
int SnowPeashooter::spriteHeight() const { return SNOW_PEASHOOTER_HEIGHT; }

const unsigned char* SnowPeashooter::shootingSprite(int f) const { return snow_peashooter_shooting_frames[f]; }
int SnowPeashooter::shootingSpriteCount() const { return SNOW_PEASHOOTER_SHOOTING_FRAMES; }
int SnowPeashooter::shootingSpriteWidth() const { return SNOW_PEASHOOTER_SHOOTING_WIDTH; }
int SnowPeashooter::shootingSpriteHeight() const { return SNOW_PEASHOOTER_SHOOTING_HEIGHT; }
