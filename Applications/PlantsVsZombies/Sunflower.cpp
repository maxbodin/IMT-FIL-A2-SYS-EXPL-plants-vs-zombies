#include <Applications/PlantsVsZombies/Sunflower.h>
#include <Applications/PlantsVsZombies/sprites/plants/sunflower_sprite.h>
#include <vga/vga.h>

extern volatile int compt;

static unsigned int sf_rng = 77777;
static unsigned int sf_lcg() {
    sf_rng = sf_rng * 1103515245 + 12345;
    return (sf_rng >> 16) & 0x7FFF;
}

Sunflower::Sunflower(int x, int y)
    : Plant(x, y, 200), nextSunTick(0)
{
    sf_rng ^= (unsigned int)compt;
    nextSunTick = compt + randomSunDelay();
}

int Sunflower::randomSunDelay() {
    sf_rng ^= (unsigned int)compt;
    return SUN_MIN_DELAY + (int)(sf_lcg() % (SUN_MAX_DELAY - SUN_MIN_DELAY + 1));
}

PlantType Sunflower::getPlantType() const { return PLANT_SUNFLOWER; }

bool Sunflower::hasSunReady() const {
    return state != DYING && state != DEAD && compt >= nextSunTick;
}

void Sunflower::resetSunTimer() {
    nextSunTick = compt + randomSunDelay();
}

const unsigned char* Sunflower::idleFrame(int f) const { return sunflower_frames[f]; }
int Sunflower::idleFrameCount() const { return SUNFLOWER_FRAMES; }
int Sunflower::spriteWidth() const { return SUNFLOWER_WIDTH; }
int Sunflower::spriteHeight() const { return SUNFLOWER_HEIGHT; }
