#include <Applications/PlantsVsZombies/Bullet.h>
#include <Applications/PlantsVsZombies/Zombie.h>
#include <Applications/PlantsVsZombies/sprites/objects/peabullet_sprite.h>
#include <Applications/PlantsVsZombies/sprites/objects/snowpeabullet_sprite.h>
#include <Applications/PlantsVsZombies/sprites/objects/peabullet_impact_sprite.h>
#include <Applications/PlantsVsZombies/sprites/objects/snowpea_impact_sprite.h>
#include <Applications/PlantsVsZombies/sprites/objects/cabbage_sprite.h>
#include <Applications/PlantsVsZombies/sprites/objects/thorn_sprite.h>
#include <Applications/PlantsVsZombies/sprites/objects/kernel_sprite.h>
#include <Applications/PlantsVsZombies/sprites/objects/butter_sprite.h>
#include <Applications/PlantsVsZombies/sprites/objects/cobcannon_impact_sprite.h>
#include <Applications/PlantsVsZombies/sprites/objects/butter_impact_sprite.h>
#include <Applications/PlantsVsZombies/sprites/objects/melon_sprite.h>
#include <Applications/PlantsVsZombies/sprites/objects/melon_impact_sprite.h>
#include <Applications/PlantsVsZombies/sprites/objects/wintermelon_projectile_sprite.h>
#include <Applications/PlantsVsZombies/sprites/objects/wintermelon_impact_sprite.h>
#include <Applications/PlantsVsZombies/sprites/objects/fire_pea_sprite.h>
#include <vga/vga.h>

/* Damage values per bullet type. */
static const int DAMAGE_PEASHOOTER = 20;
static const int DAMAGE_FIRE_PEA   = 40;
static const int DAMAGE_SNOW_PEA   = 15;
static const int DAMAGE_CABBAGE    = 40;
static const int DAMAGE_THORN      = 20;
static const int DAMAGE_KERNEL     = 15;
static const int DAMAGE_BUTTER     = 40;
static const int DAMAGE_COB        = 300;
static const int DAMAGE_MELON      = 80;
static const int DAMAGE_WINTER_MELON = 80;
static const int PROJECTILE_ANIM_SPEED = 5;

Bullet::Bullet() : x(0), y(0), spawnX(0), active(false),
                   type(BULLET_PEASHOOTER), damage(0),
                   impacting(false), impactFrame(0), impactAnimTick(0), impactHoldTicks(0),
                   projectileFrame(0), projectileAnimTick(0),
                   direction(1), torchUpgraded(false) {}

void Bullet::init(int x, int y, BulletType type) {
    this->x      = x;
    this->y      = y;
    this->spawnX = x;
    this->active = true;
    this->type   = type;
    switch (type) {
        case BULLET_SNOW_PEA: this->damage = DAMAGE_SNOW_PEA; break;
        case BULLET_FIRE_PEA: this->damage = DAMAGE_FIRE_PEA; break;
        case BULLET_CABBAGE:  this->damage = DAMAGE_CABBAGE;  break;
        case BULLET_THORN:    this->damage = DAMAGE_THORN;    break;
        case BULLET_KERNEL:   this->damage = DAMAGE_KERNEL;   break;
        case BULLET_BUTTER:   this->damage = DAMAGE_BUTTER;   break;
        case BULLET_COB:      this->damage = DAMAGE_COB;       break;
        case BULLET_MELON:    this->damage = DAMAGE_MELON;      break;
        case BULLET_WINTER_MELON: this->damage = DAMAGE_WINTER_MELON; break;
        default:              this->damage = DAMAGE_PEASHOOTER; break;
    }
    this->impacting = false;
    this->impactFrame = 0;
    this->impactAnimTick = 0;
    this->impactHoldTicks = 0;
    this->projectileFrame = 0;
    this->projectileAnimTick = 0;
    this->direction = 1;
    this->torchUpgraded = false;
}

void Bullet::update() {
    if (!active) return;

    if (impacting) {
        impactHoldTicks--;
        if (++impactAnimTick >= IMPACT_ANIM_SPEED) {
            impactAnimTick = 0;
            impactFrame++;
        }
        if (impactHoldTicks <= 0) {
            active = false;
            impacting = false;
        }
        return;
    }

    x += SPEED * direction;
    if (type == BULLET_FIRE_PEA && ++projectileAnimTick >= PROJECTILE_ANIM_SPEED) {
        projectileAnimTick = 0;
        projectileFrame = (projectileFrame + 1) % FIRE_PEA_FRAMES;
    }
    if (x >= 320 || x < 0)
        active = false;
}

void Bullet::render() {
    if (!active) return;

    if (impacting) {
        if (type == BULLET_COB) {
            int f = impactFrame < COBCANNON_IMPACT_FRAMES ? impactFrame : COBCANNON_IMPACT_FRAMES - 1;
            draw_sprite(cobcannon_impact_frames[f],
                        COBCANNON_IMPACT_WIDTH, COBCANNON_IMPACT_HEIGHT,
                        x - COBCANNON_IMPACT_WIDTH / 2, y - COBCANNON_IMPACT_HEIGHT / 2);
        } else if (type == BULLET_BUTTER) {
            int f = impactFrame < BUTTER_IMPACT_FRAMES ? impactFrame : BUTTER_IMPACT_FRAMES - 1;
            draw_sprite(butter_impact_frames[f],
                        BUTTER_IMPACT_WIDTH, BUTTER_IMPACT_HEIGHT, x, y);
        } else if (type == BULLET_MELON) {
            int f = impactFrame < MELON_IMPACT_FRAMES ? impactFrame : MELON_IMPACT_FRAMES - 1;
            draw_sprite(melon_impact_frames[f],
                        MELON_IMPACT_WIDTH, MELON_IMPACT_HEIGHT,
                        x - MELON_IMPACT_WIDTH / 2, y - MELON_IMPACT_HEIGHT / 2);
        } else if (type == BULLET_WINTER_MELON) {
            int f = impactFrame < WINTERMELON_IMPACT_FRAMES ? impactFrame : WINTERMELON_IMPACT_FRAMES - 1;
            draw_sprite(wintermelon_impact_frames[f],
                        WINTERMELON_IMPACT_WIDTH, WINTERMELON_IMPACT_HEIGHT,
                        x - WINTERMELON_IMPACT_WIDTH / 2, y - WINTERMELON_IMPACT_HEIGHT / 2);
        } else if (type == BULLET_SNOW_PEA) {
            int f = impactFrame < SNOWPEA_IMPACT_FRAMES ? impactFrame : SNOWPEA_IMPACT_FRAMES - 1;
            draw_sprite(snowpea_impact_frames[f],
                        SNOWPEA_IMPACT_WIDTH, SNOWPEA_IMPACT_HEIGHT, x, y);
        } else {
            int f = impactFrame < PEABULLET_IMPACT_FRAMES ? impactFrame : PEABULLET_IMPACT_FRAMES - 1;
            draw_sprite(peabullet_impact_frames[f],
                        PEABULLET_IMPACT_WIDTH, PEABULLET_IMPACT_HEIGHT, x, y);
        }
        return;
    }

    if (type == BULLET_SNOW_PEA) {
        draw_sprite(snowpeabullet_sprite_data,
                    SNOWPEABULLET_WIDTH, SNOWPEABULLET_HEIGHT,
                    x, y);
    } else if (type == BULLET_FIRE_PEA) {
        draw_sprite(fire_pea_frames[projectileFrame],
                    FIRE_PEA_WIDTH, FIRE_PEA_HEIGHT,
                    x, y);
    } else if (type == BULLET_CABBAGE) {
        draw_sprite(cabbage_sprite_data,
                    CABBAGE_WIDTH, CABBAGE_HEIGHT,
                    x, y);
    } else if (type == BULLET_THORN) {
        draw_sprite(thorn_sprite_data,
                    THORN_WIDTH, THORN_HEIGHT,
                    x, y);
    } else if (type == BULLET_KERNEL) {
        draw_sprite(kernel_sprite_data,
                    KERNEL_WIDTH, KERNEL_HEIGHT,
                    x, y);
    } else if (type == BULLET_BUTTER) {
        draw_sprite(butter_sprite_data,
                    BUTTER_WIDTH, BUTTER_HEIGHT,
                    x, y);
    } else if (type == BULLET_COB) {
        draw_sprite(cabbage_sprite_data,
                    CABBAGE_WIDTH, CABBAGE_HEIGHT,
                    x, y);
    } else if (type == BULLET_MELON) {
        draw_sprite(melon_frames[0],
                    MELON_WIDTH, MELON_HEIGHT,
                    x, y);
    } else if (type == BULLET_WINTER_MELON) {
        draw_sprite(wintermelon_projectile_frames[0],
                    WINTERMELON_PROJECTILE_WIDTH, WINTERMELON_PROJECTILE_HEIGHT,
                    x, y);
    } else {
        draw_sprite(peabullet_sprite_data,
                    PEABULLET_WIDTH, PEABULLET_HEIGHT,
                    x, y);
    }
}

void Bullet::onHit(Zombie& target) {
    target.takeDamage(damage);
    if (type == BULLET_SNOW_PEA)
        target.applySlow(SLOW_DURATION);
    if (type == BULLET_WINTER_MELON)
        target.applySlow(SLOW_DURATION);
    if (type == BULLET_BUTTER)
        target.applySlow(SLOW_DURATION * 2);
}

void Bullet::startImpact() {
    impacting = true;
    impactFrame = 0;
    impactAnimTick = 0;
    if (type == BULLET_COB)
        impactHoldTicks = COBCANNON_IMPACT_FRAMES * IMPACT_ANIM_SPEED;
    else if (type == BULLET_MELON)
        impactHoldTicks = MELON_IMPACT_FRAMES * IMPACT_ANIM_SPEED;
    else if (type == BULLET_WINTER_MELON)
        impactHoldTicks = WINTERMELON_IMPACT_FRAMES * IMPACT_ANIM_SPEED;
    else
        impactHoldTicks = IMPACT_HOLD;
}

bool Bullet::isActive()    const { return active; }
bool Bullet::isImpacting() const { return impacting; }
void Bullet::deactivate()        { active = false; impacting = false; }
int  Bullet::getX()       const { return x; }
int  Bullet::getY()       const { return y; }
int  Bullet::getSpawnX()  const { return spawnX; }
int  Bullet::getWidth()   const {
    switch (type) {
        case BULLET_CABBAGE: return CABBAGE_WIDTH;
        case BULLET_COB:     return CABBAGE_WIDTH;
        case BULLET_MELON:   return MELON_WIDTH;
        case BULLET_WINTER_MELON: return WINTERMELON_PROJECTILE_WIDTH;
        case BULLET_FIRE_PEA: return FIRE_PEA_WIDTH;
        case BULLET_THORN:   return THORN_WIDTH;
        case BULLET_KERNEL:  return KERNEL_WIDTH;
        case BULLET_BUTTER:  return BUTTER_WIDTH;
        case BULLET_SNOW_PEA: return SNOWPEABULLET_WIDTH;
        default: return PEABULLET_WIDTH;
    }
}
int  Bullet::getHeight()  const {
    switch (type) {
        case BULLET_CABBAGE: return CABBAGE_HEIGHT;
        case BULLET_COB:     return CABBAGE_HEIGHT;
        case BULLET_MELON:   return MELON_HEIGHT;
        case BULLET_WINTER_MELON: return WINTERMELON_PROJECTILE_HEIGHT;
        case BULLET_FIRE_PEA: return FIRE_PEA_HEIGHT;
        case BULLET_THORN:   return THORN_HEIGHT;
        case BULLET_KERNEL:  return KERNEL_HEIGHT;
        case BULLET_BUTTER:  return BUTTER_HEIGHT;
        case BULLET_SNOW_PEA: return SNOWPEABULLET_HEIGHT;
        default: return PEABULLET_HEIGHT;
    }
}
int  Bullet::getDamage()  const { return damage; }
BulletType Bullet::getType() const { return type; }
int  Bullet::getDirection() const { return direction; }
void Bullet::setDirection(int dir) { direction = dir; }
void Bullet::doubleDamage() {
    if (!torchUpgraded) {
        damage *= 2;
        torchUpgraded = true;
    }
}

void Bullet::igniteByTorchwood() {
    if (torchUpgraded) return;
    type = BULLET_FIRE_PEA;
    damage = DAMAGE_FIRE_PEA;
    projectileFrame = 0;
    projectileAnimTick = 0;
    torchUpgraded = true;
}
