#include <Applications/PlantsVsZombies/Garlic.h>
#include <Applications/PlantsVsZombies/Grid.h>
#include <Applications/PlantsVsZombies/sprites/plants/garlic/garlic_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/garlic/garlic_damaged_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/garlic/garlic_very_damaged_sprite.h>
#include <vga/vga.h>

Garlic::Garlic(int x, int y) : Plant(x, y, GARLIC_HP) {}

void Garlic::render() {
    if (state == DEAD) return;

    int hpPct = (hp * 100) / maxHp;
    const unsigned char* spriteData;

    if (hpPct > 66) {
        spriteData = garlic_frames[frame % GARLIC_FRAMES];
    } else if (hpPct > 33) {
        spriteData = garlic_damaged_frames[frame % GARLIC_DAMAGED_FRAMES];
    } else {
        spriteData = garlic_very_damaged_frames[frame % GARLIC_VERY_DAMAGED_FRAMES];
    }

    int rx = x + (Grid::TILE_SIZE - GARLIC_WIDTH) / 2;
    int ry = y + Grid::TILE_SIZE - GARLIC_HEIGHT;
    draw_sprite(spriteData, GARLIC_WIDTH, GARLIC_HEIGHT, rx, ry);
    renderHpBar(GARLIC_WIDTH / 2, GARLIC_HEIGHT);
}

PlantType Garlic::getPlantType() const { return PLANT_GARLIC; }

const unsigned char* Garlic::idleFrame(int f) const { return garlic_frames[f]; }
int Garlic::idleFrameCount() const { return GARLIC_FRAMES; }
int Garlic::spriteWidth() const { return GARLIC_WIDTH; }
int Garlic::spriteHeight() const { return GARLIC_HEIGHT; }
