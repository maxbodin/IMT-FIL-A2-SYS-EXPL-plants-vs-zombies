#include <Applications/PlantsVsZombies/JackInTheBoxZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/jackinthebox/jackinthebox_zombie_walk_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/jackinthebox/jackinthebox_zombie_fight_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/jackinthebox/jackinthebox_zombie_death_headless_sprite.h>

JackInTheBoxZombie::JackInTheBoxZombie(int x, int y, int speedBonus)
    : Zombie(x, y, JACK_HP, speedBonus + SPEED_BONUS),
      explodeTriggered(false), explodePending(false) {}

void JackInTheBoxZombie::onUpdate() {
    // Explosion is triggered by plant contact via onPlantContact()
}

void JackInTheBoxZombie::onPlantContact() {
    if (!explodeTriggered) {
        explodeTriggered = true;
        explodePending = true;
    }
}

bool JackInTheBoxZombie::hasPendingExplosion() const { return explodePending; }
void JackInTheBoxZombie::consumeExplosion() { explodePending = false; }
int JackInTheBoxZombie::explosionSpawnCount() const { return SPAWN_COUNT; }
int JackInTheBoxZombie::deathSpawnCount() const { return 1; }

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

const unsigned char* JackInTheBoxZombie::currentDeathFrame(int f) const { return jackinthebox_zombie_death_headless_frames[f]; }
int JackInTheBoxZombie::currentDeathFrameCount() const { return JACKINTHEBOX_ZOMBIE_DEATH_HEADLESS_FRAMES; }
int JackInTheBoxZombie::currentDeathWidth() const { return JACKINTHEBOX_ZOMBIE_DEATH_HEADLESS_WIDTH; }
int JackInTheBoxZombie::currentDeathHeight() const { return JACKINTHEBOX_ZOMBIE_DEATH_HEADLESS_HEIGHT; }
bool JackInTheBoxZombie::hasDeathAnimation() const { return true; }
