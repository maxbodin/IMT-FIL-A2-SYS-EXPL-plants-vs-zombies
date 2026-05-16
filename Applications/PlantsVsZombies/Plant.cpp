#include <Applications/PlantsVsZombies/Plant.h>
#include <vga/vga.h>

Plant::Plant(int x, int y, int hp)
    : Entity(x, y, hp), frame(0), animTick(0) {}

void Plant::update() {
    if (state == DYING) { state = DEAD; return; }
    if (state == DEAD) return;

    if (++animTick >= ANIM_SPEED) {
        animTick = 0;
        frame = (frame + 1) % idleFrameCount();
    }
}

void Plant::render() {
    if (state == DEAD) return;
    int w = spriteWidth();
    int h = spriteHeight();
    draw_sprite(idleFrame(frame), w, h, x, y);
    renderHpBar(w / 2, h);
}

bool Plant::canShoot() const { return false; }
BulletType Plant::getBulletType() const { return BULLET_PEASHOOTER; }
void Plant::resetCooldown() {}
bool Plant::hasSunReady() const { return false; }
void Plant::resetSunTimer() {}

int Plant::getWidth() const { return spriteWidth(); }
int Plant::getHeight() const { return spriteHeight(); }

const unsigned char* Plant::idleFrame(int) const { return 0; }
int Plant::idleFrameCount() const { return 1; }
int Plant::spriteWidth() const { return 0; }
int Plant::spriteHeight() const { return 0; }
