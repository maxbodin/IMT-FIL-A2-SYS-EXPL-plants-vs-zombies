#include <Applications/PlantsVsZombies/Peashooter.h>
#include <Applications/PlantsVsZombies/sprites/plants/peashooter_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/peashooter_shooting_sprite.h>

Peashooter::Peashooter(int x, int y)
    : ShooterPlant(x, y, HP, SHOOT_DELAY) {}

BulletType Peashooter::getBulletType() const { return BULLET_PEASHOOTER; }
PlantType Peashooter::getPlantType() const { return PLANT_PEASHOOTER; }

const unsigned char* Peashooter::idleFrame(int f) const { return peashooter_frames[f]; }
int Peashooter::idleFrameCount() const { return PEASHOOTER_FRAMES; }
int Peashooter::spriteWidth() const { return PEASHOOTER_WIDTH; }
int Peashooter::spriteHeight() const { return PEASHOOTER_HEIGHT; }

const unsigned char* Peashooter::shootingSprite(int f) const { return peashooter_shooting_frames[f]; }
int Peashooter::shootingSpriteCount() const { return PEASHOOTER_SHOOTING_FRAMES; }
int Peashooter::shootingSpriteWidth() const { return PEASHOOTER_SHOOTING_WIDTH; }
int Peashooter::shootingSpriteHeight() const { return PEASHOOTER_SHOOTING_HEIGHT; }
