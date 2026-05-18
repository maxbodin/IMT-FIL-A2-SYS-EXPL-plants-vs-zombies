#include <Applications/PlantsVsZombies/GatlingPea.h>
#include <Applications/PlantsVsZombies/sprites/plants/gatlingpea/gatlingpea_idle_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/gatlingpea/gatlingpea_shooting_sprite.h>

GatlingPea::GatlingPea(int x, int y)
    : ShooterPlant(x, y, HP, SHOOT_DELAY, 24) {}

BulletType GatlingPea::getBulletType() const { return BULLET_PEASHOOTER; }
PlantType GatlingPea::getPlantType() const { return PLANT_GATLING_PEA; }

const unsigned char* GatlingPea::idleFrame(int f) const { return gatlingpea_idle_frames[f]; }
int GatlingPea::idleFrameCount() const { return GATLINGPEA_IDLE_FRAMES; }
int GatlingPea::spriteWidth() const { return GATLINGPEA_IDLE_WIDTH; }
int GatlingPea::spriteHeight() const { return GATLINGPEA_IDLE_HEIGHT; }

const unsigned char* GatlingPea::shootingSprite(int f) const { return gatlingpea_shooting_frames[f]; }
int GatlingPea::shootingSpriteCount() const { return GATLINGPEA_SHOOTING_FRAMES; }
int GatlingPea::shootingSpriteWidth() const { return GATLINGPEA_SHOOTING_WIDTH; }
int GatlingPea::shootingSpriteHeight() const { return GATLINGPEA_SHOOTING_HEIGHT; }
