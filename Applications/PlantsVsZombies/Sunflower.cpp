#include <Applications/PlantsVsZombies/Sunflower.h>
#include <Applications/PlantsVsZombies/sprites/plants/sunflower/sunflower_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/sunflower/sunflower_producing_sprite.h>
#include <vga/vga.h>

extern volatile int compt;

static unsigned int sf_rng = 77777;
static unsigned int sf_lcg() {
    sf_rng = sf_rng * 1103515245 + 12345;
    return (sf_rng >> 16) & 0x7FFF;
}

Sunflower::Sunflower(int x, int y)
    : Plant(x, y, 200), nextSunTick(0),
      producing(false), producingStartTick(0), producingFrame(0), producingAnimTick(0)
{
    sf_rng ^= (unsigned int)compt;
    nextSunTick = compt + randomSunDelay();
}

int Sunflower::randomSunDelay() {
    sf_rng ^= (unsigned int)compt;
    return SUN_MIN_DELAY + (int)(sf_lcg() % (SUN_MAX_DELAY - SUN_MIN_DELAY + 1));
}

void Sunflower::update() {
    if (state == DYING) { state = DEAD; return; }
    if (state == DEAD) return;

    if (producing) {
        if (compt - producingStartTick >= PRODUCING_DURATION) {
            producing = false;
        } else {
            if (++producingAnimTick >= PRODUCING_ANIM_SPEED) {
                producingAnimTick = 0;
                producingFrame = (producingFrame + 1) % SUNFLOWER_PRODUCING_FRAMES;
            }
        }
    } else {
        if (++animTick >= ANIM_SPEED) {
            animTick = 0;
            frame = (frame + 1) % SUNFLOWER_FRAMES;
        }
        if (compt >= nextSunTick - PRODUCING_DURATION && !producing && compt < nextSunTick) {
            producing = true;
            producingStartTick = compt;
            producingFrame = 0;
            producingAnimTick = 0;
        }
    }
}

void Sunflower::render() {
    if (state == DEAD) return;
    if (producing) {
        int px = x + (SUNFLOWER_WIDTH - SUNFLOWER_PRODUCING_WIDTH) / 2;
        int py = y + SUNFLOWER_HEIGHT - SUNFLOWER_PRODUCING_HEIGHT;
        draw_sprite(sunflower_producing_frames[producingFrame],
                    SUNFLOWER_PRODUCING_WIDTH, SUNFLOWER_PRODUCING_HEIGHT, px, py);
    } else {
        draw_sprite(sunflower_frames[frame], SUNFLOWER_WIDTH, SUNFLOWER_HEIGHT, x, y);
        renderHpBar(SUNFLOWER_WIDTH / 2, SUNFLOWER_HEIGHT);
    }
}

PlantType Sunflower::getPlantType() const { return PLANT_SUNFLOWER; }

bool Sunflower::hasSunReady() const {
    return state != DYING && state != DEAD && compt >= nextSunTick;
}

void Sunflower::resetSunTimer() {
    nextSunTick = compt + randomSunDelay();
    producing = false;
}

const unsigned char* Sunflower::idleFrame(int f) const { return sunflower_frames[f]; }
int Sunflower::idleFrameCount() const { return SUNFLOWER_FRAMES; }
int Sunflower::spriteWidth() const { return SUNFLOWER_WIDTH; }
int Sunflower::spriteHeight() const { return SUNFLOWER_HEIGHT; }
