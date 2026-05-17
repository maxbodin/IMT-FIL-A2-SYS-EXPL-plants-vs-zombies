#include <Applications/PlantsVsZombies/JalapenoZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/jalapeno_zombie_walk_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/jalapeno_zombie_fight_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/jalapeno_zombie_death_sprite.h>

JalapenoZombie::JalapenoZombie(int x, int y, int speedBonus)
    : Zombie(x, y, JALAPENO_ZOMBIE_HP, speedBonus + SPEED_BONUS) {}

int JalapenoZombie::getWidth()  const { return JALAPENO_ZOMBIE_WALK_WIDTH; }
int JalapenoZombie::getHeight() const { return JALAPENO_ZOMBIE_WALK_HEIGHT; }

const unsigned char* JalapenoZombie::currentWalkFrame(int f) const {
    return jalapeno_zombie_walk_frames[f];
}

const unsigned char* JalapenoZombie::currentFightFrame(int f) const {
    return jalapeno_zombie_fight_frames[f];
}

int JalapenoZombie::currentWalkFrameCount() const { return JALAPENO_ZOMBIE_WALK_FRAMES; }
int JalapenoZombie::currentFightFrameCount() const { return JALAPENO_ZOMBIE_FIGHT_FRAMES; }

int JalapenoZombie::currentWalkWidth() const { return JALAPENO_ZOMBIE_WALK_WIDTH; }
int JalapenoZombie::currentWalkHeight() const { return JALAPENO_ZOMBIE_WALK_HEIGHT; }
int JalapenoZombie::currentFightWidth() const { return JALAPENO_ZOMBIE_FIGHT_WIDTH; }
int JalapenoZombie::currentFightHeight() const { return JALAPENO_ZOMBIE_FIGHT_HEIGHT; }

const unsigned char* JalapenoZombie::currentDeathFrame(int f) const { return jalapeno_zombie_death_frames[f]; }
int JalapenoZombie::currentDeathFrameCount() const { return JALAPENO_ZOMBIE_DEATH_FRAMES; }
int JalapenoZombie::currentDeathWidth() const { return JALAPENO_ZOMBIE_DEATH_WIDTH; }
int JalapenoZombie::currentDeathHeight() const { return JALAPENO_ZOMBIE_DEATH_HEIGHT; }
bool JalapenoZombie::hasDeathAnimation() const { return true; }
