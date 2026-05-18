#include <Applications/PlantsVsZombies/Blover.h>
#include <Applications/PlantsVsZombies/Grid.h>
#include <Applications/PlantsVsZombies/sprites/plants/blover/blover_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/blover/blover_attacking_sprite.h>
#include <vga/vga.h>

extern volatile int compt;

Blover::Blover(int x, int y)
    : Plant(x, y, HP), blowFrame(0), blowAnimTick(0), pushCooldown(PUSH_INTERVAL) {}

void Blover::update() {
    if (state == DYING) { state = DEAD; return; }
    if (state == DEAD) return;

    if (pushCooldown > 0) pushCooldown--;

    if (++blowAnimTick >= ANIM_SPEED) {
        blowAnimTick = 0;
        blowFrame = (blowFrame + 1) % BLOVER_ATTACKING_FRAMES;
    }
}

void Blover::render() {
    if (state == DEAD) return;

    int w = BLOVER_ATTACKING_WIDTH;
    int h = BLOVER_ATTACKING_HEIGHT;
    int rx = x + (Grid::TILE_SIZE - w) / 2;
    int ry = y + Grid::TILE_SIZE - h;
    draw_sprite(blover_attacking_frames[blowFrame], w, h, rx, ry);
    renderHpBar(w / 2, h);
}

PlantType Blover::getPlantType() const { return PLANT_BLOVER; }
bool Blover::isBlowing() const { return state == ALIVE; }
bool Blover::shouldPush() const { return state == ALIVE && pushCooldown == 0; }
void Blover::resetPushCooldown() { pushCooldown = PUSH_INTERVAL; }
int Blover::getLane() const {
    return (y - Grid::OFFSET_Y + Grid::TILE_SIZE / 2) / Grid::TILE_SIZE;
}

const unsigned char* Blover::idleFrame(int f) const { return blover_attacking_frames[f]; }
int Blover::idleFrameCount() const { return BLOVER_ATTACKING_FRAMES; }
int Blover::spriteWidth() const { return BLOVER_ATTACKING_WIDTH; }
int Blover::spriteHeight() const { return BLOVER_ATTACKING_HEIGHT; }
