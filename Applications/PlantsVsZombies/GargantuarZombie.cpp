#include <Applications/PlantsVsZombies/GargantuarZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/gargantuar/gargantuar_zombie_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/gargantuar/gargantuar_zombie_minus_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/gargantuar/gargantuar_zombie_burnt_sprite.h>

GargantuarZombie::GargantuarZombie(int x, int y, int speedBonus)
    : Zombie(x, y, GARGANTUAR_HP, speedBonus), crushCooldown(0) {}

void GargantuarZombie::onUpdate() {
    if (crushCooldown > 0) --crushCooldown;
}

bool GargantuarZombie::canCrush() const { return crushCooldown <= 0; }
void GargantuarZombie::resetCrushCooldown() { crushCooldown = CRUSH_COOLDOWN; }

int GargantuarZombie::getWidth()  const { return GARGANTUAR_ZOMBIE_FULL_WIDTH; }
int GargantuarZombie::getHeight() const { return GARGANTUAR_ZOMBIE_FULL_HEIGHT; }

const unsigned char* GargantuarZombie::currentWalkFrame(int f) const {
    return gargantuar_zombie_full_frames[f];
}

const unsigned char* GargantuarZombie::currentFightFrame(int f) const {
    return gargantuar_zombie_minus_frames[f];
}

int GargantuarZombie::currentWalkFrameCount() const { return GARGANTUAR_ZOMBIE_FULL_FRAMES; }
int GargantuarZombie::currentFightFrameCount() const { return GARGANTUAR_ZOMBIE_MINUS_FRAMES; }

int GargantuarZombie::currentWalkWidth() const { return GARGANTUAR_ZOMBIE_FULL_WIDTH; }
int GargantuarZombie::currentWalkHeight() const { return GARGANTUAR_ZOMBIE_FULL_HEIGHT; }
int GargantuarZombie::currentFightWidth() const { return GARGANTUAR_ZOMBIE_MINUS_WIDTH; }
int GargantuarZombie::currentFightHeight() const { return GARGANTUAR_ZOMBIE_MINUS_HEIGHT; }

const unsigned char* GargantuarZombie::currentDeathFrame(int f) const { return gargantuar_zombie_burnt_frames[f]; }
int GargantuarZombie::currentDeathFrameCount() const { return GARGANTUAR_ZOMBIE_BURNT_FRAMES; }
int GargantuarZombie::currentDeathWidth() const { return GARGANTUAR_ZOMBIE_BURNT_WIDTH; }
int GargantuarZombie::currentDeathHeight() const { return GARGANTUAR_ZOMBIE_BURNT_HEIGHT; }
bool GargantuarZombie::hasDeathAnimation() const { return true; }
