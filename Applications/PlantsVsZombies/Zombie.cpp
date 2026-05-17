#include <Applications/PlantsVsZombies/Zombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/basic/basic_zombie_walk_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/basic/basic_zombie_walk_no_arm_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/basic/basic_zombie_walk_no_head_no_arm_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/basic/basic_zombie_fight_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/basic/basic_zombie_fight_no_arm_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/basic/basic_zombie_death_sprite.h>
#include <Applications/PlantsVsZombies/sprites/objects/zombie_fire/zombie_fire_sprite.h>
#include <Applications/PlantsVsZombies/sprites/objects/snowpea/snowpea_impact_sprite.h>
#include <vga/vga.h>

extern volatile int compt;

static int clampSpeed(int bonus) {
    int s = Zombie::BASE_ANIM_SPEED - bonus;
    return s < Zombie::MIN_ANIM_SPEED ? Zombie::MIN_ANIM_SPEED : s;
}

Zombie::Zombie(int x, int y, int speedBonus)
    : Entity(x, y, HP), animSpeed(clampSpeed(speedBonus)),
      cooldown(HIT_DELAY), frame(0), animTick(0), slowTicks(0),
      fireDamage(0), fireEndTick(0), fireNextDmgTick(0), fireFrame(0), fireAnimTick(0), deathFrame(0), deathAnimTick(0) {}

Zombie::Zombie(int x, int y, int customHp, int speedBonus)
    : Entity(x, y, customHp), animSpeed(clampSpeed(speedBonus)),
      cooldown(HIT_DELAY), frame(0), animTick(0), slowTicks(0),
      fireDamage(0), fireEndTick(0), fireNextDmgTick(0), fireFrame(0), fireAnimTick(0), deathFrame(0), deathAnimTick(0) {}

void Zombie::update() {
    if (state == DYING) {
        if (hasDeathAnimation()) {
            if (++deathAnimTick >= DEATH_ANIM_SPEED) {
                deathAnimTick = 0;
                deathFrame++;
                if (deathFrame >= currentDeathFrameCount()) {
                    state = DEAD;
                }
            }
        } else {
            state = DEAD;
        }
        return;
    }

    if (state == DEAD)  return;

    if (cooldown > 0)
        cooldown--;
    if (slowTicks > 0)
        slowTicks--;

    /* Fire damage over time */
    if (fireEndTick > 0 && compt < fireEndTick) {
        if (compt >= fireNextDmgTick) {
            takeDamage(fireDamage);
            fireNextDmgTick = compt + FIRE_TICK_INTERVAL;
        }
        if (++fireAnimTick >= FIRE_ANIM_SPEED) {
            fireAnimTick = 0;
            fireFrame = (fireFrame + 1) % ZOMBIE_FIRE_FRAMES;
        }
    }

    int speed = (slowTicks > 0) ? animSpeed * 2 : animSpeed;

    onUpdate();

    if (state == BLOCKED) {
        if (++animTick >= speed) {
            animTick = 0;
            frame = (frame + 1) % currentFightFrameCount();
        }
    } else if (++animTick >= speed) {
        animTick = 0;
        frame = (frame + 1) % currentWalkFrameCount();
        if (frame == 1 || frame == 5) {
            x -= WALK_STEP; 
        }
    }
}

void Zombie::render() {
    if (state == DEAD) return;

    /* Death animation rendering */
    if (state == DYING && hasDeathAnimation()) {
        int df = deathFrame < currentDeathFrameCount() ? deathFrame : currentDeathFrameCount() - 1;
        draw_sprite(currentDeathFrame(df),
                    currentDeathWidth(), currentDeathHeight(), x, y);
        return;
    }

    int w = getWidth();
    int h = getHeight();
    if (state == BLOCKED) {
        draw_sprite(currentFightFrame(frame), currentFightWidth(), currentFightHeight(), x, y);
    } else {
        draw_sprite(currentWalkFrame(frame), currentWalkWidth(), currentWalkHeight(), x, y);
    }
    renderHpBar(w / 2, h);

    /* Fire overlay */
    if (fireEndTick > 0 && compt < fireEndTick) {
        draw_sprite(zombie_fire_frames[fireFrame],
                    ZOMBIE_FIRE_WIDTH, ZOMBIE_FIRE_HEIGHT,
                    x + (w - ZOMBIE_FIRE_WIDTH) / 2, y);
    }

    /* Snow/slow overlay */
    if (slowTicks > 0) {
        draw_sprite(snowpea_impact_frames[SNOWPEA_IMPACT_FRAMES - 1],
                    SNOWPEA_IMPACT_WIDTH, SNOWPEA_IMPACT_HEIGHT,
                    x + (w - SNOWPEA_IMPACT_WIDTH) / 2,
                    y + h - SNOWPEA_IMPACT_HEIGHT);
    }
}

bool Zombie::canHit() const {
    return state != DYING && state != DEAD && cooldown == 0;
}

int Zombie::getWidth()  const { return BASIC_ZOMBIE_WALK_FULL_WIDTH; }
int Zombie::getHeight() const { return BASIC_ZOMBIE_WALK_FULL_HEIGHT; }

void Zombie::resetCooldown() {
    cooldown = HIT_DELAY;
}

void Zombie::block() { 
    if (state == ALIVE) state = BLOCKED;
}

void Zombie::unblock() { 
    if (state == BLOCKED) state = ALIVE; 
}

bool Zombie::isBlocked() const { 
    return state == BLOCKED; 
}

void Zombie::applySlow(int duration) {
    slowTicks = duration;
}

bool Zombie::isSlowed() const {
    return slowTicks > 0;
}

void Zombie::applyFire(int damage, int duration) {
    fireDamage     = damage;
    fireEndTick    = compt + duration;
    fireNextDmgTick = compt + 100;
    fireFrame      = 0;
    fireAnimTick   = 0;
}

bool Zombie::isOnFire() const {
    return fireEndTick > 0 && compt < fireEndTick;
}

bool Zombie::hasPendingSummon() const { return false; }
void Zombie::consumeSummon() {}
bool Zombie::canBeBlocked() const { return true; }
bool Zombie::isFlying() const { return false; }

const unsigned char* Zombie::currentWalkFrame(int f)  const {
    if (hp > 100) return basic_zombie_walk_full_frames[f];
    if (hp > 50)  return basic_zombie_walk_no_arm_frames[f];
    if (hp > 25)  return basic_zombie_walk_no_arm_frames[f];
    return basic_zombie_walk_no_head_no_arm_frames[f % BASIC_ZOMBIE_WALK_NO_HEAD_NO_ARM_FRAMES];
}
const unsigned char* Zombie::currentFightFrame(int f) const {
    if (hp > 100) return basic_zombie_fight_full_frames[f];
    if (hp > 50)  return basic_zombie_fight_no_arm_frames[f];
    return basic_zombie_fight_no_arm_frames[f];
}
int Zombie::currentWalkFrameCount()  const { return BASIC_ZOMBIE_WALK_FULL_FRAMES; }
int Zombie::currentFightFrameCount() const { return BASIC_ZOMBIE_FIGHT_FULL_FRAMES; }
int Zombie::currentWalkWidth()  const {
    if (hp <= 25) return BASIC_ZOMBIE_WALK_NO_HEAD_NO_ARM_WIDTH;
    return BASIC_ZOMBIE_WALK_FULL_WIDTH;
}
int Zombie::currentWalkHeight() const {
    if (hp <= 25) return BASIC_ZOMBIE_WALK_NO_HEAD_NO_ARM_HEIGHT;
    return BASIC_ZOMBIE_WALK_FULL_HEIGHT;
}
int Zombie::currentFightWidth()  const { return BASIC_ZOMBIE_FIGHT_FULL_WIDTH; }
int Zombie::currentFightHeight() const { return BASIC_ZOMBIE_FIGHT_FULL_HEIGHT; }

const unsigned char* Zombie::currentDeathFrame(int f) const {
    if (hp <= 100) return basic_zombie_death_frames[f % BASIC_ZOMBIE_DEATH_FRAMES];
    return basic_zombie_death_frames[f];
}
int Zombie::currentDeathFrameCount() const {
    if (hp <= 100) return BASIC_ZOMBIE_DEATH_FRAMES;
    return BASIC_ZOMBIE_DEATH_FRAMES;
}
int Zombie::currentDeathWidth() const {
    if (hp <= 100) return BASIC_ZOMBIE_DEATH_WIDTH;
    return BASIC_ZOMBIE_DEATH_WIDTH;
}
int Zombie::currentDeathHeight() const {
    if (hp <= 100) return BASIC_ZOMBIE_DEATH_HEIGHT;
    return BASIC_ZOMBIE_DEATH_HEIGHT;
}
bool Zombie::hasDeathAnimation() const { return true; }

void Zombie::onUpdate() {}
