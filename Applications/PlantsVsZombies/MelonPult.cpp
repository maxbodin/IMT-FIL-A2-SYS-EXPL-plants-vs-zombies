#include <Applications/PlantsVsZombies/MelonPult.h>
#include <Applications/PlantsVsZombies/sprites/plants/melonpult/melonpult_sprite.h>

MelonPult::MelonPult(int x, int y)
    : ShooterPlant(x, y, HP, SHOOT_DELAY) {}

BulletType MelonPult::getBulletType() const { return BULLET_MELON; }
PlantType MelonPult::getPlantType() const { return PLANT_MELONPULT; }

const unsigned char* MelonPult::idleFrame(int f) const { return melonpult_frames[f]; }
int MelonPult::idleFrameCount() const { return MELONPULT_FRAMES; }
int MelonPult::spriteWidth() const { return MELONPULT_WIDTH; }
int MelonPult::spriteHeight() const { return MELONPULT_HEIGHT; }
