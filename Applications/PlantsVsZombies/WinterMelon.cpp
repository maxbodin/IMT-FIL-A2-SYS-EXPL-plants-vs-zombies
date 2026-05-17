#include <Applications/PlantsVsZombies/WinterMelon.h>
#include <Applications/PlantsVsZombies/sprites/plants/wintermelon/wintermelon_sprite.h>

WinterMelon::WinterMelon(int x, int y)
    : ShooterPlant(x, y, HP, SHOOT_DELAY) {}

BulletType WinterMelon::getBulletType() const { return BULLET_WINTER_MELON; }
PlantType WinterMelon::getPlantType() const { return PLANT_WINTERMELON; }

const unsigned char* WinterMelon::idleFrame(int f) const { return wintermelon_frames[f]; }
int WinterMelon::idleFrameCount() const { return WINTERMELON_FRAMES; }
int WinterMelon::spriteWidth() const { return WINTERMELON_WIDTH; }
int WinterMelon::spriteHeight() const { return WINTERMELON_HEIGHT; }
