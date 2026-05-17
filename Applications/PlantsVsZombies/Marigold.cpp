#include <Applications/PlantsVsZombies/Marigold.h>
#include <Applications/PlantsVsZombies/sprites/plants/marigold/marigold_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/marigold/marigold_attacking_sprite.h>
#include <vga/vga.h>

extern volatile int compt;

static unsigned int mg_rng = 99887;
static unsigned int mg_lcg() {
    mg_rng = mg_rng * 1103515245 + 12345;
    return (mg_rng >> 16) & 0x7FFF;
}

Marigold::Marigold(int x, int y)
    : Plant(x, y, HP), nextSunTick(0),
      producing(false), producingTick(0), producingFrame(0), producingAnimTick(0)
{
    mg_rng ^= (unsigned int)compt;
    nextSunTick = compt + randomSunDelay();
}

int Marigold::randomSunDelay() {
    mg_rng ^= (unsigned int)compt;
    return SUN_MIN_DELAY + (int)(mg_lcg() % (SUN_MAX_DELAY - SUN_MIN_DELAY + 1));
}

void Marigold::update() {
    if (state == DYING) { state = DEAD; return; }
    if (state == DEAD) return;

    if (producing) {
        if (compt - producingTick >= PRODUCING_DURATION) {
            producing = false;
        } else if (++producingAnimTick >= PRODUCING_ANIM_SPEED) {
            producingAnimTick = 0;
            producingFrame = (producingFrame + 1) % MARIGOLD_ATTACKING_FRAMES;
        }
    } else {
        if (++animTick >= ANIM_SPEED) {
            animTick = 0;
            frame = (frame + 1) % MARIGOLD_FRAMES;
        }
    }
}

void Marigold::render() {
    if (state == DEAD) return;

    if (producing) {
        draw_sprite(marigold_attacking_frames[producingFrame],
                    MARIGOLD_ATTACKING_WIDTH, MARIGOLD_ATTACKING_HEIGHT, x, y);
        renderHpBar(MARIGOLD_ATTACKING_WIDTH / 2, MARIGOLD_ATTACKING_HEIGHT);
    } else {
        draw_sprite(marigold_frames[frame], MARIGOLD_WIDTH, MARIGOLD_HEIGHT, x, y);
        renderHpBar(MARIGOLD_WIDTH / 2, MARIGOLD_HEIGHT);
    }
}

PlantType Marigold::getPlantType() const { return PLANT_MARIGOLD; }

bool Marigold::hasSunReady() const {
    return state == ALIVE && compt >= nextSunTick;
}

void Marigold::resetSunTimer() {
    nextSunTick = compt + randomSunDelay();
    producing = true;
    producingTick = compt;
    producingFrame = 0;
    producingAnimTick = 0;
}

const unsigned char* Marigold::idleFrame(int f) const { return marigold_frames[f]; }
int Marigold::idleFrameCount() const { return MARIGOLD_FRAMES; }
int Marigold::spriteWidth() const { return MARIGOLD_WIDTH; }
int Marigold::spriteHeight() const { return MARIGOLD_HEIGHT; }
