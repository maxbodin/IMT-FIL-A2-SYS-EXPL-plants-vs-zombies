#include <Applications/PlantsVsZombies/CobCannon.h>
#include <Applications/PlantsVsZombies/Grid.h>
#include <Applications/PlantsVsZombies/sprites/plants/cobcannon/cobcannon_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/cobcannon/cobcannon_loading_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/cobcannon/cobcannon_ready_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/cobcannon/cobcannon_rising_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/cobcannon/cobcannon_firing_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/cobcannon/cobcannon_used_sprite.h>
#include <vga/vga.h>

CobCannon::CobCannon(int x, int y)
    : Plant(x, y, HP), cannonState(LOADING), stateFrame(0), stateAnimTick(0) {}

void CobCannon::update() {
    if (state == DYING) { state = DEAD; return; }
    if (state == DEAD) return;

    stateAnimTick++;
    switch (cannonState) {
    case LOADING:
        if (stateAnimTick >= ANIM_SPEED_LOADING) {
            stateAnimTick = 0;
            stateFrame++;
            if (stateFrame >= COBCANNON_LOADING_FRAMES) {
                cannonState = READY;
                stateFrame = 0;
            }
        }
        break;
    case READY:
        if (stateAnimTick >= ANIM_SPEED_READY) {
            stateAnimTick = 0;
            stateFrame = (stateFrame + 1) % COBCANNON_READY_FRAMES;
        }
        break;
    case RISING:
        if (stateAnimTick >= ANIM_SPEED_RISING) {
            stateAnimTick = 0;
            stateFrame++;
            if (stateFrame >= COBCANNON_RISING_FRAMES) {
                cannonState = FIRING;
                stateFrame = 0;
            }
        }
        break;
    case FIRING:
        if (stateAnimTick >= ANIM_SPEED_FIRING) {
            stateAnimTick = 0;
            stateFrame++;
            if (stateFrame >= COBCANNON_FIRING_FRAMES) {
                cannonState = USED;
                stateFrame = 0;
            }
        }
        break;
    case USED:
        if (stateAnimTick >= ANIM_SPEED_USED) {
            stateAnimTick = 0;
            stateFrame++;
            if (stateFrame >= COBCANNON_USED_FRAMES) {
                cannonState = LOADING;
                stateFrame = 0;
            }
        }
        break;
    }
}

void CobCannon::render() {
    if (state == DEAD) return;

    const unsigned char* sprite = 0;
    int w = COBCANNON_WIDTH;
    int h = COBCANNON_HEIGHT;

    switch (cannonState) {
    case LOADING:
        sprite = cobcannon_loading_frames[stateFrame];
        w = COBCANNON_LOADING_WIDTH;
        h = COBCANNON_LOADING_HEIGHT;
        break;
    case READY:
        sprite = cobcannon_ready_frames[stateFrame];
        w = COBCANNON_READY_WIDTH;
        h = COBCANNON_READY_HEIGHT;
        break;
    case RISING:
        sprite = cobcannon_rising_frames[stateFrame];
        w = COBCANNON_RISING_WIDTH;
        h = COBCANNON_RISING_HEIGHT;
        break;
    case FIRING:
        sprite = cobcannon_firing_frames[stateFrame];
        w = COBCANNON_FIRING_WIDTH;
        h = COBCANNON_FIRING_HEIGHT;
        break;
    case USED:
        sprite = cobcannon_used_frames[stateFrame];
        w = COBCANNON_USED_WIDTH;
        h = COBCANNON_USED_HEIGHT;
        break;
    }

    // Center across 2 tiles, bottom-aligned
    int cx = x + Grid::TILE_SIZE;  // center of 2-tile span
    int by = y + Grid::TILE_SIZE;
    int drawX = cx - w / 2;
    int drawY = by - h;
    draw_sprite(sprite, w, h, drawX, drawY);
    renderHpBar(Grid::TILE_SIZE, Grid::TILE_SIZE);
}

PlantType CobCannon::getPlantType() const { return PLANT_COBCANNON; }

bool CobCannon::canShoot() const {
    return state == ALIVE && cannonState == READY;
}

void CobCannon::resetCooldown() {
    cannonState = RISING;
    stateFrame = 0;
    stateAnimTick = 0;
}

BulletType CobCannon::getBulletType() const { return BULLET_COB; }

const unsigned char* CobCannon::idleFrame(int f) const { return cobcannon_frames[f]; }
int CobCannon::idleFrameCount() const { return COBCANNON_FRAMES; }
int CobCannon::spriteWidth() const { return COBCANNON_WIDTH; }
int CobCannon::spriteHeight() const { return COBCANNON_HEIGHT; }
