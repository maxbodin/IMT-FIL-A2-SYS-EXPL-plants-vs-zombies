#include <Applications/PlantsVsZombies/CabbagePult.h>
#include <Applications/PlantsVsZombies/sprites/plants/cabbagepult/cabbagepult_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/cabbagepult/cabbagepult_attacking_sprite.h>

CabbagePult::CabbagePult(int x, int y)
    : ShooterPlant(x, y, HP, SHOOT_DELAY) {}

BulletType CabbagePult::getBulletType() const { return BULLET_CABBAGE; }
PlantType CabbagePult::getPlantType() const { return PLANT_CABBAGEPULT; }

const unsigned char* CabbagePult::idleFrame(int f) const { return cabbagepult_frames[f]; }
int CabbagePult::idleFrameCount() const { return CABBAGEPULT_FRAMES; }
int CabbagePult::spriteWidth() const { return CABBAGEPULT_WIDTH; }
int CabbagePult::spriteHeight() const { return CABBAGEPULT_HEIGHT; }

const unsigned char* CabbagePult::shootingSprite(int f) const { return cabbagepult_attacking_frames[f]; }
int CabbagePult::shootingSpriteCount() const { return CABBAGEPULT_ATTACKING_FRAMES; }
int CabbagePult::shootingSpriteWidth() const { return CABBAGEPULT_ATTACKING_WIDTH; }
int CabbagePult::shootingSpriteHeight() const { return CABBAGEPULT_ATTACKING_HEIGHT; }
