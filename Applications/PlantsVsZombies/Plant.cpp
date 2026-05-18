#include <Applications/PlantsVsZombies/Plant.h>
#include <Applications/PlantsVsZombies/Grid.h>
#include <Applications/PlantsVsZombies/sprites/objects/zombie_fire/zombie_fire_sprite.h>
#include <vga/vga.h>

Plant::Plant(int x, int y, int hp)
    : Entity(x, y, hp), frame(0), animTick(0),
      plantFireDmg(0), plantFireTicks(0), plantFireNextTick(0),
      plantFireFrame(0), plantFireAnimTick(0) {}

void Plant::update() {
    if (state == DYING) { state = DEAD; return; }
    if (state == DEAD) return;

    /* Fire DoT */
    if (plantFireTicks > 0) {
        plantFireTicks--;
        if (--plantFireNextTick <= 0) {
            takeDamage(plantFireDmg);
            plantFireNextTick = FIRE_TICK_INTERVAL;
        }
        if (++plantFireAnimTick >= FIRE_ANIM_SPEED) {
            plantFireAnimTick = 0;
            plantFireFrame = (plantFireFrame + 1) % ZOMBIE_FIRE_FRAMES;
        }
    }

    if (++animTick >= ANIM_SPEED) {
        animTick = 0;
        frame = (frame + 1) % idleFrameCount();
    }
}

void Plant::render() {
    if (state == DEAD) return;
    int w = spriteWidth();
    int h = spriteHeight();
    int rx = x + (Grid::TILE_SIZE - w) / 2;
    int ry = y + Grid::TILE_SIZE - h;
    draw_sprite(idleFrame(frame), w, h, rx, ry);
    renderHpBar(w / 2, h);

    /* Fire overlay */
    if (plantFireTicks > 0) {
        draw_sprite(zombie_fire_frames[plantFireFrame],
                    ZOMBIE_FIRE_WIDTH, ZOMBIE_FIRE_HEIGHT,
                    rx + (w - ZOMBIE_FIRE_WIDTH) / 2, ry);
    }
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

void Plant::applyFire(int dmgPerTick, int duration) {
    plantFireDmg = dmgPerTick;
    plantFireTicks = duration;
    plantFireNextTick = FIRE_TICK_INTERVAL;
    plantFireFrame = 0;
    plantFireAnimTick = 0;
}

bool Plant::isOnFireEffect() const {
    return plantFireTicks > 0;
}

int Plant::blockChance() const { return 100; }
