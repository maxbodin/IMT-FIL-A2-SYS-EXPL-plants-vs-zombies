#include <Applications/PlantsVsZombies/Zombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/zombie_walk_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/zombie_fight_sprite.h>
#include <Applications/PlantsVsZombies/sprites/objects/zombie_fire_sprite.h>
#include <Applications/PlantsVsZombies/sprites/objects/snowpea_impact_sprite.h>
#include <vga/vga.h>

extern volatile int compt;

static int clampSpeed(int bonus) {
    int s = Zombie::BASE_ANIM_SPEED - bonus;
    return s < Zombie::MIN_ANIM_SPEED ? Zombie::MIN_ANIM_SPEED : s;
}

Zombie::Zombie(int x, int y, int speedBonus)
    : Entity(x, y, HP), animSpeed(clampSpeed(speedBonus)),
      cooldown(HIT_DELAY), frame(0), animTick(0), slowTicks(0),
      fireDamage(0), fireEndTick(0), fireNextDmgTick(0), fireFrame(0), fireAnimTick(0) {}

Zombie::Zombie(int x, int y, int customHp, int speedBonus)
    : Entity(x, y, customHp), animSpeed(clampSpeed(speedBonus)),
      cooldown(HIT_DELAY), frame(0), animTick(0), slowTicks(0),
      fireDamage(0), fireEndTick(0), fireNextDmgTick(0), fireFrame(0), fireAnimTick(0) {}

void Zombie::update() {
    if (state == DYING) {
        state = DEAD;
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
            fireNextDmgTick = compt + 100; // damage tick every 100 ticks
        }
        if (++fireAnimTick >= 8) {
            fireAnimTick = 0;
            fireFrame = (fireFrame + 1) % ZOMBIE_FIRE_FRAMES;
        }
    }

    int speed = (slowTicks > 0) ? animSpeed * 2 : animSpeed;

    onUpdate();

    if (state == BLOCKED) {
        if (++animTick >= speed) {
            animTick = 0;
            frame = (frame + 1) % fightFrameCount();
        }
    } else if (++animTick >= speed) {
        animTick = 0;
        frame = (frame + 1) % walkFrameCount();
        if (frame == 1 || frame == 5) {
            x -= 5; 
        }
    }
}

void Zombie::render() {
    if (state == DEAD) return;
    int w = getWidth();
    int h = getHeight();
    if (state == BLOCKED) {
        draw_sprite(fightFrame(frame), ZOMBIE_FIGHT_WIDTH, ZOMBIE_FIGHT_HEIGHT, x, y);
    } else {
        draw_sprite(walkFrame(frame), ZOMBIE_WALK_WIDTH, ZOMBIE_WALK_HEIGHT, x, y);
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

int Zombie::getWidth()  const { return ZOMBIE_WALK_WIDTH; }
int Zombie::getHeight() const { return ZOMBIE_WALK_HEIGHT; }

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

const unsigned char* Zombie::walkFrame(int f)  const { return zombie_walk_frames[f]; }
const unsigned char* Zombie::fightFrame(int f) const { return zombie_fight_frames[f]; }
int Zombie::walkFrameCount()  const { return ZOMBIE_WALK_FRAMES; }
int Zombie::fightFrameCount() const { return ZOMBIE_FIGHT_FRAMES; }
void Zombie::onUpdate() {}
