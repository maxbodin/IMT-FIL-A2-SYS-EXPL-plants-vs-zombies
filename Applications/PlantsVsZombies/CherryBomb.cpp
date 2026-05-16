#include <Applications/PlantsVsZombies/CherryBomb.h>
#include <Applications/PlantsVsZombies/sprites/plants/cherrybomb_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/cherrybomb_attacking_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/cherrybomb_explosion_sprite.h>
#include <vga/vga.h>

extern volatile int compt;

CherryBomb::CherryBomb(int x, int y)
    : Plant(x, y, HP),
      attacking(false), attackingStartTick(0), attackingFrame(0), attackingAnimTick(0),
      exploding(false), explosionStartTick(0), explosionFrame(0), explosionAnimTick(0) {}

void CherryBomb::update() {
    if (state == DYING) { state = DEAD; return; }
    if (state == DEAD) return;

    if (exploding) {
        if (compt - explosionStartTick >= EXPLOSION_DURATION) {
            state = DYING;
            return;
        }
        if (++explosionAnimTick >= EXPLOSION_ANIM_SPEED) {
            explosionAnimTick = 0;
            if (explosionFrame < CHERRYBOMB_EXPLOSION_FRAMES - 1)
                explosionFrame++;
        }
    } else if (attacking) {
        if (compt - attackingStartTick >= ATTACKING_DURATION) {
            exploding = true;
            attacking = false;
            explosionStartTick = compt;
            explosionFrame = 0;
            explosionAnimTick = 0;
            return;
        }
        if (++attackingAnimTick >= ATTACKING_ANIM_SPEED) {
            attackingAnimTick = 0;
            attackingFrame = (attackingFrame + 1) % CHERRYBOMB_ATTACKING_FRAMES;
        }
    } else {
        if (++animTick >= ANIM_SPEED) {
            animTick = 0;
            frame = (frame + 1) % CHERRYBOMB_FRAMES;
        }
    }
}

void CherryBomb::render() {
    if (state == DEAD) return;
    if (exploding) {
        int ex = x - (CHERRYBOMB_EXPLOSION_WIDTH - CHERRYBOMB_WIDTH) / 2;
        int ey = y - (CHERRYBOMB_EXPLOSION_HEIGHT - CHERRYBOMB_HEIGHT) / 2;
        draw_sprite(cherrybomb_explosion_frames[explosionFrame],
                    CHERRYBOMB_EXPLOSION_WIDTH, CHERRYBOMB_EXPLOSION_HEIGHT, ex, ey);
    } else if (attacking) {
        int ax = x + (CHERRYBOMB_WIDTH - CHERRYBOMB_ATTACKING_WIDTH) / 2;
        int ay = y + CHERRYBOMB_HEIGHT - CHERRYBOMB_ATTACKING_HEIGHT;
        draw_sprite(cherrybomb_attacking_frames[attackingFrame],
                    CHERRYBOMB_ATTACKING_WIDTH, CHERRYBOMB_ATTACKING_HEIGHT, ax, ay);
    } else {
        draw_sprite(cherrybomb_frames[frame], CHERRYBOMB_WIDTH, CHERRYBOMB_HEIGHT, x, y);
        renderHpBar(CHERRYBOMB_WIDTH / 2, CHERRYBOMB_HEIGHT);
    }
}

PlantType CherryBomb::getPlantType() const { return PLANT_CHERRYBOMB; }

void CherryBomb::explode() {
    if (!exploding && !attacking) {
        attacking = true;
        attackingStartTick = compt;
        attackingFrame = 0;
        attackingAnimTick = 0;
    }
}

bool CherryBomb::isExploding() const { return exploding; }
bool CherryBomb::isAttacking() const { return attacking; }
int CherryBomb::getExplosionDamage() const { return EXPLOSION_DAMAGE; }

const unsigned char* CherryBomb::idleFrame(int f) const { return cherrybomb_frames[f]; }
int CherryBomb::idleFrameCount() const { return CHERRYBOMB_FRAMES; }
int CherryBomb::spriteWidth() const { return CHERRYBOMB_WIDTH; }
int CherryBomb::spriteHeight() const { return CHERRYBOMB_HEIGHT; }
