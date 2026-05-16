#include <Applications/PlantsVsZombies/PogoZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/pogo_zombie_pogo_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/pogo_zombie_walk_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/pogo_zombie_fight_sprite.h>

PogoZombie::PogoZombie(int x, int y, int speedBonus)
    : Zombie(x, y, POGO_TOTAL_HP, speedBonus + POGO_SPEED), pogoHp(POGO_HP),
      pogoDamageAccum(0) {}

void PogoZombie::onUpdate() {
    /* Check if pogo should break based on cumulative damage taken. */
    if (pogoHp > 0) {
        int damageTaken = POGO_TOTAL_HP - hp;
        if (damageTaken >= POGO_HP) {
            pogoHp = 0;
        }
    }
}

bool PogoZombie::canBeBlocked() const {
    return pogoHp <= 0; // can't be blocked while on pogo.
}

const unsigned char* PogoZombie::walkFrame(int f) const {
    if (pogoHp > 0) return pogo_zombie_pogo_frames[f];
    return pogo_zombie_walk_frames[f];
}

const unsigned char* PogoZombie::fightFrame(int f) const {
    return pogo_zombie_fight_frames[f];
}

int PogoZombie::walkFrameCount() const {
    if (pogoHp > 0) return POGO_ZOMBIE_POGO_FRAMES;
    return POGO_ZOMBIE_WALK_FRAMES;
}

int PogoZombie::fightFrameCount() const {
    return POGO_ZOMBIE_FIGHT_FRAMES;
}
