#include <Applications/PlantsVsZombies/PotatoMine.h>
#include <Applications/PlantsVsZombies/sprites/plants/potato_mine_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/potato_mine_popping_sprite.h>
#include <Applications/PlantsVsZombies/sprites/objects/potato_mine_explosion_sprite.h>
#include <vga/vga.h>

extern volatile int compt;

PotatoMine::PotatoMine(int x, int y)
    : Plant(x, y, HP), armed(false), popping(false),
      poppingStartTick(compt), poppingFrame(0), poppingAnimTick(0),
      exploding(false), explosionDone(false),
      explosionStartTick(0), explosionFrame(0), explosionAnimTick(0)
{}

void PotatoMine::update() {
    if (state == DYING) { state = DEAD; return; }
    if (state == DEAD)  return;

    if (exploding) {
        if (compt - explosionStartTick >= EXPLOSION_DURATION) {
            explosionDone = true;
            state = DYING;
            return;
        }
        if (++explosionAnimTick >= EXPLOSION_ANIM_SPEED) {
            explosionAnimTick = 0;
            if (explosionFrame < POTATO_MINE_EXPLOSION_FRAMES - 1)
                explosionFrame++;
        }
        return;
    }

    if (!armed) {
        if (!popping && compt - poppingStartTick >= POPPING_DURATION) {
            popping = true;
            poppingFrame = 0;
            poppingAnimTick = 0;
        }
        if (popping) {
            if (++poppingAnimTick >= POPPING_ANIM_SPEED) {
                poppingAnimTick = 0;
                poppingFrame++;
                if (poppingFrame >= POTATO_MINE_POPPING_FRAMES) {
                    armed = true;
                    popping = false;
                    frame = 0;
                }
            }
        }
        return;
    }

    if (++animTick >= ANIM_SPEED) {
        animTick = 0;
        frame = (frame + 1) % POTATO_MINE_FRAMES;
    }
}

void PotatoMine::render() {
    if (state == DEAD) return;
    if (exploding) {
        int ex = x - (POTATO_MINE_EXPLOSION_WIDTH  - POTATO_MINE_WIDTH)  / 2;
        int ey = y - (POTATO_MINE_EXPLOSION_HEIGHT - POTATO_MINE_HEIGHT) / 2;
        draw_sprite(potato_mine_explosion_frames[explosionFrame],
                    POTATO_MINE_EXPLOSION_WIDTH, POTATO_MINE_EXPLOSION_HEIGHT,
                    ex, ey);
    } else if (popping) {
        int f = poppingFrame < POTATO_MINE_POPPING_FRAMES ? poppingFrame : POTATO_MINE_POPPING_FRAMES - 1;
        /* Align popping sprite bottom with idle sprite bottom. */
        int py = y + POTATO_MINE_HEIGHT - POTATO_MINE_POPPING_HEIGHT;
        int px = x + (POTATO_MINE_WIDTH - POTATO_MINE_POPPING_WIDTH) / 2;
        draw_sprite(potato_mine_popping_frames[f],
                    POTATO_MINE_POPPING_WIDTH, POTATO_MINE_POPPING_HEIGHT, px, py);
    } else {
        draw_sprite(potato_mine_frames[frame],
                    POTATO_MINE_WIDTH, POTATO_MINE_HEIGHT, x, y);
        if (armed) renderHpBar(POTATO_MINE_WIDTH / 2, POTATO_MINE_HEIGHT);
    }
}

void PotatoMine::explode() {
    if (!exploding && armed && state != DEAD && state != DYING) {
        exploding = true;
        explosionStartTick = compt;
        explosionFrame = 0;
        explosionAnimTick = 0;
    }
}

PlantType PotatoMine::getPlantType() const { return PLANT_POTATO_MINE; }

const unsigned char* PotatoMine::idleFrame(int f) const { return potato_mine_frames[f]; }
int PotatoMine::idleFrameCount() const { return POTATO_MINE_FRAMES; }
int PotatoMine::spriteWidth() const { return POTATO_MINE_WIDTH; }
int PotatoMine::spriteHeight() const { return POTATO_MINE_HEIGHT; }
