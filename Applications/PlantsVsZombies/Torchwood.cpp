#include <Applications/PlantsVsZombies/Torchwood.h>
#include <Applications/PlantsVsZombies/sprites/plants/torchwood_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/torchwood_fire_sprite.h>
#include <vga/vga.h>

Torchwood::Torchwood(int x, int y)
    : Plant(x, y, HP), fireFrame(0), fireAnimTick(0), fireTicks(0) {}

void Torchwood::update() {
    Plant::update();
    if (fireTicks <= 0) return;

    fireTicks--;
    if (++fireAnimTick >= FIRE_ANIM_SPEED) {
        fireAnimTick = 0;
        fireFrame = (fireFrame + 1) % TORCHWOOD_FIRE_FRAMES;
    }
}

void Torchwood::render() {
    if (state == DEAD) return;
    draw_sprite(torchwood_frames[frame], TORCHWOOD_WIDTH, TORCHWOOD_HEIGHT, x, y);
    if (fireTicks > 0) {
        draw_sprite(torchwood_fire_frames[fireFrame], TORCHWOOD_FIRE_WIDTH, TORCHWOOD_FIRE_HEIGHT,
                    x + (TORCHWOOD_WIDTH - TORCHWOOD_FIRE_WIDTH) / 2, y + 10);
    }
    renderHpBar(TORCHWOOD_WIDTH / 2, TORCHWOOD_HEIGHT);
}

PlantType Torchwood::getPlantType() const { return PLANT_TORCHWOOD; }

void Torchwood::ignite() {
    fireTicks = FIRE_DURATION;
    fireFrame = 0;
    fireAnimTick = 0;
}

const unsigned char* Torchwood::idleFrame(int f) const { return torchwood_frames[f]; }
int Torchwood::idleFrameCount() const { return TORCHWOOD_FRAMES; }
int Torchwood::spriteWidth() const { return TORCHWOOD_WIDTH; }
int Torchwood::spriteHeight() const { return TORCHWOOD_HEIGHT; }
