#include <Applications/PlantsVsZombies/Squash.h>
#include <Applications/PlantsVsZombies/sprites/plants/squash/squash_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/squash/squash_attacking_sprite.h>
#include <vga/vga.h>

static const int LANDED_DURATION = 30;

Squash::Squash(int x, int y)
    : Plant(x, y, HP), jumping(false), landed(false),
      targetX(0), targetY(0), attackFrame(0), attackAnimTick(0), landedTicks(0) {}

void Squash::update() {
    if (state == DYING) { state = DEAD; return; }
    if (state == DEAD) return;

    if (landed) {
        landedTicks--;
        if (landedTicks <= 0) {
            state = DYING;
        }
        return;
    }

    if (jumping) {
        int dx = targetX - x;
        int dy = targetY - y;
        int dist2 = dx * dx + dy * dy;
        if (dist2 <= JUMP_SPEED * JUMP_SPEED + 4) {
            x = targetX;
            y = targetY;
            landed = true;
            landedTicks = LANDED_DURATION;
        } else {
            if (dx > 0) x += JUMP_SPEED;
            else if (dx < 0) x -= JUMP_SPEED;
            if (dy > 0) y += JUMP_SPEED;
            else if (dy < 0) y -= JUMP_SPEED;
        }
        if (++attackAnimTick >= ATTACK_ANIM_SPEED) {
            attackAnimTick = 0;
            attackFrame = (attackFrame + 1) % SQUASH_ATTACKING_FRAMES;
        }
    } else {
        if (++animTick >= ANIM_SPEED) {
            animTick = 0;
            frame = (frame + 1) % SQUASH_FRAMES;
        }
    }
}

void Squash::render() {
    if (state == DEAD) return;

    if (jumping || landed) {
        int f = attackFrame < SQUASH_ATTACKING_FRAMES ? attackFrame : SQUASH_ATTACKING_FRAMES - 1;
        draw_sprite(squash_attacking_frames[f],
                    SQUASH_ATTACKING_WIDTH, SQUASH_ATTACKING_HEIGHT, x, y);
    } else {
        draw_sprite(squash_frames[frame], SQUASH_WIDTH, SQUASH_HEIGHT, x, y);
        renderHpBar(SQUASH_WIDTH / 2, SQUASH_HEIGHT);
    }
}

void Squash::setTarget(int tx, int ty) {
    targetX = tx;
    targetY = ty;
    jumping = true;
    attackFrame = 0;
    attackAnimTick = 0;
}

PlantType Squash::getPlantType() const { return PLANT_SQUASH; }

const unsigned char* Squash::idleFrame(int f) const { return squash_frames[f]; }
int Squash::idleFrameCount() const { return SQUASH_FRAMES; }
int Squash::spriteWidth() const { return SQUASH_WIDTH; }
int Squash::spriteHeight() const { return SQUASH_HEIGHT; }
