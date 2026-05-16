#include <Applications/PlantsVsZombies/Garlic.h>
#include <Applications/PlantsVsZombies/sprites/plants/garlic_sprite.h>

Garlic::Garlic(int x, int y) : Plant(x, y, GARLIC_HP) {}

PlantType Garlic::getPlantType() const { return PLANT_GARLIC; }

const unsigned char* Garlic::idleFrame(int f) const { return garlic_frames[f]; }
int Garlic::idleFrameCount() const { return GARLIC_FRAMES; }
int Garlic::spriteWidth() const { return GARLIC_WIDTH; }
int Garlic::spriteHeight() const { return GARLIC_HEIGHT; }
