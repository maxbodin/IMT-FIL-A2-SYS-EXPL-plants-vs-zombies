#include <Applications/PlantsVsZombies/SplitPea.h>
#include <Applications/PlantsVsZombies/sprites/plants/splitpea_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/splitpea_shooting_both_sprite.h>

SplitPea::SplitPea(int x, int y)
    : ShooterPlant(x, y, HP, SHOOT_DELAY) {}

BulletType SplitPea::getBulletType() const { return BULLET_PEASHOOTER; }
PlantType SplitPea::getPlantType() const { return PLANT_SPLITPEA; }

const unsigned char* SplitPea::idleFrame(int f) const { return splitpea_frames[f]; }
int SplitPea::idleFrameCount() const { return SPLITPEA_FRAMES; }
int SplitPea::spriteWidth() const { return SPLITPEA_WIDTH; }
int SplitPea::spriteHeight() const { return SPLITPEA_HEIGHT; }

const unsigned char* SplitPea::shootingSprite(int f) const { return splitpea_shooting_both_frames[f]; }
int SplitPea::shootingSpriteCount() const { return SPLITPEA_SHOOTING_BOTH_FRAMES; }
int SplitPea::shootingSpriteWidth() const { return SPLITPEA_SHOOTING_BOTH_WIDTH; }
int SplitPea::shootingSpriteHeight() const { return SPLITPEA_SHOOTING_BOTH_HEIGHT; }
