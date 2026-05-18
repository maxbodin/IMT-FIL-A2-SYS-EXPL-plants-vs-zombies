#include <Applications/PlantsVsZombies/CabbagePult.h>
#include <Applications/PlantsVsZombies/Grid.h>
#include <Applications/PlantsVsZombies/sprites/plants/cabbagepult/cabbagepult_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/cabbagepult/cabbagepult_attacking_sprite.h>
#include <vga/vga.h>

CabbagePult::CabbagePult(int x, int y)
    : ShooterPlant(x, y, HP, SHOOT_DELAY) {}

void CabbagePult::render() {
    if (state == DEAD) return;

    // Anchor both idle and shooting sprites to the same bottom-left position
    int iw = CABBAGEPULT_WIDTH;
    int ih = CABBAGEPULT_HEIGHT;
    int rx = x + (Grid::TILE_SIZE - iw) / 2;
    int by = y + Grid::TILE_SIZE;

    if (shooting && shootingSpriteCount() > 0) {
        int sw = CABBAGEPULT_ATTACKING_WIDTH;
        int sh = CABBAGEPULT_ATTACKING_HEIGHT;
        // Left-align with idle sprite, extend upward
        draw_sprite(shootingSprite(shootIdx), sw, sh, rx, by - sh);
        renderHpBar(sw / 2, sh);
    } else {
        draw_sprite(idleFrame(frame), iw, ih, rx, by - ih);
        renderHpBar(iw / 2, ih);
    }
}

BulletType CabbagePult::getBulletType() const { return BULLET_CABBAGE; }
PlantType CabbagePult::getPlantType() const { return PLANT_CABBAGEPULT; }

const unsigned char* CabbagePult::idleFrame(int f) const { return cabbagepult_frames[f]; }
int CabbagePult::idleFrameCount() const { return CABBAGEPULT_FRAMES; }
int CabbagePult::spriteWidth() const { return CABBAGEPULT_WIDTH; }
int CabbagePult::spriteHeight() const { return CABBAGEPULT_HEIGHT; }

const unsigned char* CabbagePult::shootingSprite(int f) const { return cabbagepult_attacking_frames[f]; }
int CabbagePult::shootingSpriteCount() const { return CABBAGEPULT_ATTACKING_FRAMES; }
int CabbagePult::shootingSpriteWidth() const { return CABBAGEPULT_ATTACKING_WIDTH; }
int CabbagePult::shootingSpriteHeight() const { return CABBAGEPULT_ATTACKING_HEIGHT; }
