#include <Applications/PlantsVsZombies/WallNut.h>
#include <Applications/PlantsVsZombies/sprites/plants/wallnut/wallnut_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/wallnut/wallnut_damaged_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/wallnut/wallnut_very_damaged_sprite.h>
#include <vga/vga.h>

WallNut::WallNut(int x, int y) : Plant(x, y, WALLNUT_HP) {}

void WallNut::render() {
    if (state == DEAD) return;

    /* Pick sprite set based on HP percentage. */
    int hpPct = (hp * 100) / maxHp;
    const unsigned char* spriteData;

    if (hpPct > 66) {
        spriteData = wallnut_frames[frame % WALLNUT_FRAMES];
    } else if (hpPct > 33) {
        spriteData = wallnut_damaged_frames[frame % WALLNUT_DAMAGED_FRAMES];
    } else {
        spriteData = wallnut_very_damaged_frames[frame % WALLNUT_VERY_DAMAGED_FRAMES];
    }

    draw_sprite(spriteData, WALLNUT_WIDTH, WALLNUT_HEIGHT, x, y);
    renderHpBar(WALLNUT_WIDTH / 2, WALLNUT_HEIGHT);
}

PlantType WallNut::getPlantType() const { return PLANT_WALLNUT; }

const unsigned char* WallNut::idleFrame(int f) const { return wallnut_frames[f]; }
int WallNut::idleFrameCount() const { return WALLNUT_FRAMES; }
int WallNut::spriteWidth() const { return WALLNUT_WIDTH; }
int WallNut::spriteHeight() const { return WALLNUT_HEIGHT; }
