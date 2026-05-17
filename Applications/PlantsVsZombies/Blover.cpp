#include <Applications/PlantsVsZombies/Blover.h>
#include <Applications/PlantsVsZombies/sprites/plants/blover/blover_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/blover/blover_attacking_sprite.h>
#include <vga/vga.h>

extern volatile int compt;

Blover::Blover(int x, int y)
    : Plant(x, y, HP), blowing(true), blowStartTick(compt),
      blowFrame(0), blowAnimTick(0) {}

void Blover::update() {
    if (state == DYING) { state = DEAD; return; }
    if (state == DEAD) return;

    if (blowing && compt - blowStartTick >= DURATION) {
        state = DYING;
        return;
    }

    if (blowing) {
        if (++blowAnimTick >= ANIM_SPEED) {
            blowAnimTick = 0;
            blowFrame = (blowFrame + 1) % BLOVER_ATTACKING_FRAMES;
        }
    } else {
        if (++animTick >= ANIM_SPEED) {
            animTick = 0;
            frame = (frame + 1) % BLOVER_FRAMES;
        }
    }
}

void Blover::render() {
    if (state == DEAD) return;

    if (blowing) {
        draw_sprite(blover_attacking_frames[blowFrame],
                    BLOVER_ATTACKING_WIDTH, BLOVER_ATTACKING_HEIGHT, x, y);
        renderHpBar(BLOVER_ATTACKING_WIDTH / 2, BLOVER_ATTACKING_HEIGHT);
    } else {
        draw_sprite(blover_frames[frame], BLOVER_WIDTH, BLOVER_HEIGHT, x, y);
        renderHpBar(BLOVER_WIDTH / 2, BLOVER_HEIGHT);
    }
}

PlantType Blover::getPlantType() const { return PLANT_BLOVER; }
bool Blover::isBlowing() const { return blowing && state == ALIVE; }
int Blover::getBlowRange() const { return BLOW_RANGE; }

const unsigned char* Blover::idleFrame(int f) const { return blover_frames[f]; }
int Blover::idleFrameCount() const { return BLOVER_FRAMES; }
int Blover::spriteWidth() const { return BLOVER_WIDTH; }
int Blover::spriteHeight() const { return BLOVER_HEIGHT; }
