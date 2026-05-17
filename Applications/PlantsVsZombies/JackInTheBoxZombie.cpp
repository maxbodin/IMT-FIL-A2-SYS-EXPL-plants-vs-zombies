#include <Applications/PlantsVsZombies/JackInTheBoxZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/jackinthebox_zombie_walk_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/jackinthebox_zombie_fight_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/jackinthebox_zombie_death_sprite.h>

JackInTheBoxZombie::JackInTheBoxZombie(int x, int y, int speedBonus)
    : Zombie(x, y, JACK_HP, speedBonus + SPEED_BONUS),
      explodeTriggered(false), explodePending(false) {}

void JackInTheBoxZombie::onUpdate() {
    if (!explodeTriggered && x <= EXPLODE_X) {
        explodeTriggered = true;
        explodePending   = true;
    }
}

bool JackInTheBoxZombie::hasPendingExplosion() const { return explodePending; }
void JackInTheBoxZombie::consumeExplosion() { explodePending = false; }

int JackInTheBoxZombie::getWidth()  const { return JACKINTHEBOX_ZOMBIE_WALK_WIDTH; }
int JackInTheBoxZombie::getHeight() const { return JACKINTHEBOX_ZOMBIE_WALK_HEIGHT; }

const unsigned char* JackInTheBoxZombie::currentWalkFrame(int f) const {
    return jackinthebox_zombie_walk_frames[f];
}

const unsigned char* JackInTheBoxZombie::currentFightFrame(int f) const {
    return jackinthebox_zombie_fight_frames[f];
}

int JackInTheBoxZombie::currentWalkFrameCount() const { return JACKINTHEBOX_ZOMBIE_WALK_FRAMES; }
int JackInTheBoxZombie::currentFightFrameCount() const { return JACKINTHEBOX_ZOMBIE_FIGHT_FRAMES; }

int JackInTheBoxZombie::currentWalkWidth() const { return JACKINTHEBOX_ZOMBIE_WALK_WIDTH; }
int JackInTheBoxZombie::currentWalkHeight() const { return JACKINTHEBOX_ZOMBIE_WALK_HEIGHT; }
int JackInTheBoxZombie::currentFightWidth() const { return JACKINTHEBOX_ZOMBIE_FIGHT_WIDTH; }
int JackInTheBoxZombie::currentFightHeight() const { return JACKINTHEBOX_ZOMBIE_FIGHT_HEIGHT; }

const unsigned char* JackInTheBoxZombie::currentDeathFrame(int f) const { return jackinthebox_zombie_death_frames[f]; }
int JackInTheBoxZombie::currentDeathFrameCount() const { return JACKINTHEBOX_ZOMBIE_DEATH_FRAMES; }
int JackInTheBoxZombie::currentDeathWidth() const { return JACKINTHEBOX_ZOMBIE_DEATH_WIDTH; }
int JackInTheBoxZombie::currentDeathHeight() const { return JACKINTHEBOX_ZOMBIE_DEATH_HEIGHT; }
bool JackInTheBoxZombie::hasDeathAnimation() const { return true; }
