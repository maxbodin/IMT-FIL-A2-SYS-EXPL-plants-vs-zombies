#include <Applications/PlantsVsZombies/DiscoZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/disco_zombie_walk_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/disco_zombie_fight_sprite.h>

DiscoZombie::DiscoZombie(int x, int y, int speedBonus)
    : Zombie(x, y, DISCO_HP, speedBonus), summonTriggered(false), summonPending(false) {}

void DiscoZombie::onUpdate() {
    if (!summonTriggered && x <= SUMMON_X) {
        summonTriggered = true;
        summonPending   = true;
    }
}

const unsigned char* DiscoZombie::walkFrame(int f)  const { return disco_zombie_walk_frames[f]; }
const unsigned char* DiscoZombie::fightFrame(int f) const { return disco_zombie_fight_frames[f]; }
int DiscoZombie::walkFrameCount()  const { return DISCO_ZOMBIE_WALK_FRAMES; }
int DiscoZombie::fightFrameCount() const { return DISCO_ZOMBIE_FIGHT_FRAMES; }

bool DiscoZombie::hasPendingSummon() const { return summonPending; }
void DiscoZombie::consumeSummon() { summonPending = false; }
