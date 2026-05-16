#include <Applications/PlantsVsZombies/Jalapeno.h>
#include <Applications/PlantsVsZombies/sprites/plants/jalapeno_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/jalapeno_attacking_sprite.h>
#include <Applications/PlantsVsZombies/sprites/objects/jalapeno_fire_sprite.h>
#include <vga/vga.h>

extern volatile int compt;

static unsigned int jal_rng = 31337;
static unsigned int jal_lcg() {
    jal_rng = jal_rng * 1103515245 + 12345;
    return (jal_rng >> 16) & 0x7FFF;
}

Jalapeno::Jalapeno(int x, int y)
    : Plant(x, y, HP), attacking(false), onFire(false), fireDone(false),
      attackStartTick(0), attackFrame(0), attackAnimTick(0),
      fireStartTick(0), fireFrame(0), fireAnimTick(0)
{}

void Jalapeno::update() {
    if (state == DYING) { state = DEAD; return; }
    if (state == DEAD)  return;

    if (onFire) {
        if (compt - fireStartTick >= FIRE_DURATION) {
            fireDone = true;
            state = DYING;
            return;
        }
        if (++fireAnimTick >= FIRE_ANIM_SPEED) {
            fireAnimTick = 0;
            fireFrame = (fireFrame + 1) % JALAPENO_FIRE_FRAMES;
        }
    } else if (attacking) {
        if (compt - attackStartTick >= ATTACK_DURATION) {
            attacking = false;
            onFire = true;
            fireStartTick = compt;
            fireFrame = 0;
            fireAnimTick = 0;
            return;
        }
        if (++attackAnimTick >= FIRE_ANIM_SPEED) {
            attackAnimTick = 0;
            attackFrame = (attackFrame + 1) % JALAPENO_ATTACKING_FRAMES;
        }
    } else {
        if (++animTick >= ANIM_SPEED) {
            animTick = 0;
            frame = (frame + 1) % JALAPENO_FRAMES;
        }
    }
}

void Jalapeno::render() {
    if (state == DEAD) return;
    if (onFire) {
        draw_sprite(jalapeno_fire_frames[fireFrame],
                    JALAPENO_FIRE_WIDTH, JALAPENO_FIRE_HEIGHT,
                    x, y + JALAPENO_HEIGHT - JALAPENO_FIRE_HEIGHT);
    } else if (attacking) {
        draw_sprite(jalapeno_attacking_frames[attackFrame],
                    JALAPENO_ATTACKING_WIDTH, JALAPENO_ATTACKING_HEIGHT,
                    x, y + JALAPENO_HEIGHT - JALAPENO_ATTACKING_HEIGHT);
    } else {
        draw_sprite(jalapeno_frames[frame],
                    JALAPENO_WIDTH, JALAPENO_HEIGHT, x, y);
    }
    renderHpBar(JALAPENO_WIDTH / 2, JALAPENO_HEIGHT);
}

void Jalapeno::ignite() {
    if (!attacking && !onFire && state != DEAD && state != DYING) {
        attacking = true;
        attackStartTick = compt;
        attackFrame = 0;
        attackAnimTick = 0;
    }
}

int Jalapeno::getFireDamage() {
    jal_rng ^= (unsigned int)compt;
    return FIRE_DMG_MIN + (int)(jal_lcg() % (FIRE_DMG_MAX - FIRE_DMG_MIN + 1));
}

int Jalapeno::getFireEffectDuration() {
    jal_rng ^= (unsigned int)compt;
    return FIRE_EFFECT_MIN + (int)(jal_lcg() % (FIRE_EFFECT_MAX - FIRE_EFFECT_MIN + 1));
}

PlantType Jalapeno::getPlantType() const { return PLANT_JALAPENO; }

const unsigned char* Jalapeno::idleFrame(int f) const { return jalapeno_frames[f]; }
int Jalapeno::idleFrameCount() const { return JALAPENO_FRAMES; }
int Jalapeno::spriteWidth() const { return JALAPENO_WIDTH; }
int Jalapeno::spriteHeight() const { return JALAPENO_HEIGHT; }
