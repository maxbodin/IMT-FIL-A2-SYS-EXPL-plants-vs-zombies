#include <Applications/PlantsVsZombies/TwinSunflower.h>
#include <Applications/PlantsVsZombies/sprites/plants/twinsunflower_sprite.h>

extern volatile int compt;

static unsigned int tsf_rng = 55555;
static unsigned int tsf_lcg() {
    tsf_rng = tsf_rng * 1103515245 + 12345;
    return (tsf_rng >> 16) & 0x7FFF;
}

TwinSunflower::TwinSunflower(int x, int y)
    : Plant(x, y, 300), nextSunTick(0)
{
    tsf_rng ^= (unsigned int)compt;
    nextSunTick = compt + randomSunDelay();
}

int TwinSunflower::randomSunDelay() {
    tsf_rng ^= (unsigned int)compt;
    return SUN_MIN_DELAY + (int)(tsf_lcg() % (SUN_MAX_DELAY - SUN_MIN_DELAY + 1));
}

PlantType TwinSunflower::getPlantType() const { return PLANT_TWINSUNFLOWER; }

bool TwinSunflower::hasSunReady() const {
    return state != DYING && state != DEAD && compt >= nextSunTick;
}

void TwinSunflower::resetSunTimer() {
    nextSunTick = compt + randomSunDelay();
}

const unsigned char* TwinSunflower::idleFrame(int f) const { return twinsunflower_frames[f]; }
int TwinSunflower::idleFrameCount() const { return TWINSUNFLOWER_FRAMES; }
int TwinSunflower::spriteWidth() const { return TWINSUNFLOWER_WIDTH; }
int TwinSunflower::spriteHeight() const { return TWINSUNFLOWER_HEIGHT; }
