#include <Applications/PlantsVsZombies/CatapultZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/catapult/catapult_zombie_walk_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/basic/basic_zombie_death_sprite.h>

CatapultZombie::CatapultZombie(int x, int y, int speedBonus)
    : Zombie(x, y, CATAPULT_HP, speedBonus + SPEED_BONUS), crushCooldown(0) {}

void CatapultZombie::onUpdate() {
    if (crushCooldown > 0) crushCooldown--;
}

bool CatapultZombie::canCrush() const {
    return state != DYING && state != DEAD && crushCooldown == 0;
}

void CatapultZombie::resetCrushCooldown() {
    crushCooldown = CRUSH_COOLDOWN;
}

int CatapultZombie::getWidth()  const { return CATAPULT_ZOMBIE_WALK_WIDTH; }
int CatapultZombie::getHeight() const { return CATAPULT_ZOMBIE_WALK_HEIGHT; }

const unsigned char* CatapultZombie::currentWalkFrame(int f)  const { return catapult_zombie_walk_frames[f]; }
const unsigned char* CatapultZombie::currentFightFrame(int f) const { return catapult_zombie_walk_frames[f]; }

int CatapultZombie::currentWalkFrameCount()  const { return CATAPULT_ZOMBIE_WALK_FRAMES; }
int CatapultZombie::currentFightFrameCount() const { return CATAPULT_ZOMBIE_WALK_FRAMES; }
int CatapultZombie::currentWalkWidth()  const { return CATAPULT_ZOMBIE_WALK_WIDTH; }
int CatapultZombie::currentWalkHeight() const { return CATAPULT_ZOMBIE_WALK_HEIGHT; }
int CatapultZombie::currentFightWidth()  const { return CATAPULT_ZOMBIE_WALK_WIDTH; }
int CatapultZombie::currentFightHeight() const { return CATAPULT_ZOMBIE_WALK_HEIGHT; }

const unsigned char* CatapultZombie::currentDeathFrame(int f) const { return basic_zombie_death_frames[f]; }
int CatapultZombie::currentDeathFrameCount() const { return BASIC_ZOMBIE_DEATH_FRAMES; }
int CatapultZombie::currentDeathWidth()  const { return BASIC_ZOMBIE_DEATH_WIDTH; }
int CatapultZombie::currentDeathHeight() const { return BASIC_ZOMBIE_DEATH_HEIGHT; }
bool CatapultZombie::hasDeathAnimation() const { return true; }
