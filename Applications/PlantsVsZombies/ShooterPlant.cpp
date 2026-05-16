#include <Applications/PlantsVsZombies/ShooterPlant.h>
#include <vga/vga.h>

ShooterPlant::ShooterPlant(int x, int y, int hp, int shootDelay, int shootHold)
    : Plant(x, y, hp), cooldown(shootDelay), shootDelay(shootDelay),
      shooting(false), shootIdx(0), shootAnimTick(0),
      shootHoldTicks(0), shootHold(shootHold) {}

void ShooterPlant::update() {
    if (state == DYING) { state = DEAD; return; }
    if (state == DEAD) return;

    if (cooldown > 0) cooldown--;

    if (shooting) {
        int sc = shootingSpriteCount();
        if (sc > 0) {
            if (++shootAnimTick >= SHOOT_ANIM_SPEED) {
                shootAnimTick = 0;
                shootIdx = (shootIdx + 1) % sc;
            }
        }
        shootHoldTicks--;
        if (shootHoldTicks <= 0) {
            shooting = false;
            frame = 0;
            animTick = 0;
        }
    } else {
        if (++animTick >= ANIM_SPEED) {
            animTick = 0;
            frame = (frame + 1) % idleFrameCount();
        }
    }
}

void ShooterPlant::render() {
    if (state == DEAD) return;

    if (shooting && shootingSpriteCount() > 0) {
        int w = shootingSpriteWidth();
        int h = shootingSpriteHeight();
        draw_sprite(shootingSprite(shootIdx), w, h, x, y);
        renderHpBar(w / 2, h);
    } else {
        int w = spriteWidth();
        int h = spriteHeight();
        draw_sprite(idleFrame(frame), w, h, x, y);
        renderHpBar(w / 2, h);
    }
}

bool ShooterPlant::canShoot() const {
    return state == ALIVE && cooldown == 0;
}

void ShooterPlant::resetCooldown() {
    cooldown = shootDelay;
    shooting = true;
    shootIdx = 0;
    shootAnimTick = 0;
    shootHoldTicks = shootHold;
}

const unsigned char* ShooterPlant::shootingSprite(int) const { return 0; }
int ShooterPlant::shootingSpriteCount() const { return 0; }
int ShooterPlant::shootingSpriteWidth() const { return spriteWidth(); }
int ShooterPlant::shootingSpriteHeight() const { return spriteHeight(); }
