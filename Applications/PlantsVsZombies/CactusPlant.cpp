#include <Applications/PlantsVsZombies/CactusPlant.h>
#include <Applications/PlantsVsZombies/sprites/plants/cactus/cactus_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/cactus/cactus_attacking_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/cactus/cactus_growing_sprite.h>
#include <vga/vga.h>

CactusPlant::CactusPlant(int x, int y)
    : ShooterPlant(x, y, HP, SHOOT_DELAY),
      extended(false), growing(false), shrinking(false),
      zombieInRange(false), growFrame(0), growAnimTick(0) {}

void CactusPlant::update() {
    if (state == DYING) { state = DEAD; return; }
    if (state == DEAD) return;

    /* Grow/shrink based on zombie presence. */
    if (zombieInRange && !extended && !growing) {
        growing = true;
        shrinking = false;
        growFrame = 0;
        growAnimTick = 0;
    } else if (!zombieInRange && extended && !shrinking) {
        shrinking = true;
        growing = false;
        growFrame = CACTUS_GROWING_FRAMES - 1;
        growAnimTick = 0;
    }

    if (growing) {
        if (++growAnimTick >= GROW_ANIM_SPEED) {
            growAnimTick = 0;
            growFrame++;
            if (growFrame >= CACTUS_GROWING_FRAMES) {
                growing = false;
                extended = true;
                growFrame = CACTUS_GROWING_FRAMES - 1;
            }
        }
    } else if (shrinking) {
        if (++growAnimTick >= GROW_ANIM_SPEED) {
            growAnimTick = 0;
            growFrame--;
            if (growFrame < 0) {
                shrinking = false;
                extended = false;
                growFrame = 0;
            }
        }
    }

    /* Reset flag each tick; PlantsVsZombies.cpp sets it if a zombie is in lane. */
    zombieInRange = false;

    ShooterPlant::update();
}

void CactusPlant::render() {
    if (state == DEAD) return;

    if (growing || shrinking) {
        int f = growFrame < CACTUS_GROWING_FRAMES ? growFrame : CACTUS_GROWING_FRAMES - 1;
        if (f < 0) f = 0;
        /* Align bottom of growing sprite with bottom of idle sprite. */
        int gy = y + CACTUS_HEIGHT - CACTUS_GROWING_HEIGHT;
        draw_sprite(cactus_growing_frames[f],
                    CACTUS_GROWING_WIDTH, CACTUS_GROWING_HEIGHT, x, gy);
        renderHpBar(CACTUS_GROWING_WIDTH / 2, CACTUS_GROWING_HEIGHT + (y - gy));
    } else if (extended) {
        /* While extended, show the last growing frame (fully tall). */
        int gy = y + CACTUS_HEIGHT - CACTUS_GROWING_HEIGHT;
        int f = CACTUS_GROWING_FRAMES - 1;
        draw_sprite(cactus_growing_frames[f],
                    CACTUS_GROWING_WIDTH, CACTUS_GROWING_HEIGHT, x, gy);
        renderHpBar(CACTUS_GROWING_WIDTH / 2, CACTUS_GROWING_HEIGHT + (y - gy));
    } else {
        ShooterPlant::render();
    }
}

BulletType CactusPlant::getBulletType() const { return BULLET_THORN; }
PlantType CactusPlant::getPlantType() const { return PLANT_CACTUS; }

const unsigned char* CactusPlant::idleFrame(int f) const { return cactus_frames[f]; }
int CactusPlant::idleFrameCount() const { return CACTUS_FRAMES; }
int CactusPlant::spriteWidth() const { return CACTUS_WIDTH; }
int CactusPlant::spriteHeight() const { return CACTUS_HEIGHT; }

const unsigned char* CactusPlant::shootingSprite(int f) const { return cactus_attacking_frames[f]; }
int CactusPlant::shootingSpriteCount() const { return CACTUS_ATTACKING_FRAMES; }
int CactusPlant::shootingSpriteWidth() const { return CACTUS_ATTACKING_WIDTH; }
int CactusPlant::shootingSpriteHeight() const { return CACTUS_ATTACKING_HEIGHT; }
