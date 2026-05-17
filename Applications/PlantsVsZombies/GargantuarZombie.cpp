#include <Applications/PlantsVsZombies/GargantuarZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/gargantuar_zombie_walk_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/gargantuar_zombie_fight_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/gargantuar_zombie_death_sprite.h>

GargantuarZombie::GargantuarZombie(int x, int y, int speedBonus)
    : Zombie(x, y, GARGANTUAR_HP, speedBonus), crushCooldown(0) {}

void GargantuarZombie::onUpdate() {
    if (crushCooldown > 0) --crushCooldown;
}

bool GargantuarZombie::canCrush() const { return crushCooldown <= 0; }
void GargantuarZombie::resetCrushCooldown() { crushCooldown = CRUSH_COOLDOWN; }

int GargantuarZombie::getWidth()  const { return GARGANTUAR_ZOMBIE_WALK_WIDTH; }
int GargantuarZombie::getHeight() const { return GARGANTUAR_ZOMBIE_WALK_HEIGHT; }

const unsigned char* GargantuarZombie::currentWalkFrame(int f) const {
    return gargantuar_zombie_walk_frames[f];
}

const unsigned char* GargantuarZombie::currentFightFrame(int f) const {
    return gargantuar_zombie_fight_frames[f];
}

int GargantuarZombie::currentWalkFrameCount() const { return GARGANTUAR_ZOMBIE_WALK_FRAMES; }
int GargantuarZombie::currentFightFrameCount() const { return GARGANTUAR_ZOMBIE_FIGHT_FRAMES; }

int GargantuarZombie::currentWalkWidth() const { return GARGANTUAR_ZOMBIE_WALK_WIDTH; }
int GargantuarZombie::currentWalkHeight() const { return GARGANTUAR_ZOMBIE_WALK_HEIGHT; }
int GargantuarZombie::currentFightWidth() const { return GARGANTUAR_ZOMBIE_FIGHT_WIDTH; }
int GargantuarZombie::currentFightHeight() const { return GARGANTUAR_ZOMBIE_FIGHT_HEIGHT; }

const unsigned char* GargantuarZombie::currentDeathFrame(int f) const { return gargantuar_zombie_death_frames[f]; }
int GargantuarZombie::currentDeathFrameCount() const { return GARGANTUAR_ZOMBIE_DEATH_FRAMES; }
int GargantuarZombie::currentDeathWidth() const { return GARGANTUAR_ZOMBIE_DEATH_WIDTH; }
int GargantuarZombie::currentDeathHeight() const { return GARGANTUAR_ZOMBIE_DEATH_HEIGHT; }
bool GargantuarZombie::hasDeathAnimation() const { return true; }
