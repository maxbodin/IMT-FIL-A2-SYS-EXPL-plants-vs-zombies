#include <Applications/PlantsVsZombies/NewspaperZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/newspaper_zombie_walk_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/newspaper_zombie_walk_angry_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/newspaper_zombie_fight_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/newspaper_zombie_fight_angry_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/newspaper_zombie_death_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/newspaper_zombie_death_angry_sprite.h>

NewspaperZombie::NewspaperZombie(int x, int y, int speedBonus)
    : Zombie(x, y, NEWSPAPER_HP, speedBonus), angry(false), lastDamageStage(0) {}

int NewspaperZombie::getDamageStage() const {
    if (state == DYING) return lastDamageStage;
    if (hp > ANGRY_THRESHOLD) return 0; // full (with newspaper)
    return 1; // angry (no newspaper)
}

void NewspaperZombie::onUpdate() {
    if (hp > ANGRY_THRESHOLD) {
        lastDamageStage = 0;
    } else {
        lastDamageStage = 1;
        if (!angry) {
            angry = true;
            animSpeed -= ANGRY_SPEED_BONUS;
            if (animSpeed < MIN_ANIM_SPEED)
                animSpeed = MIN_ANIM_SPEED;
        }
    }
}

int NewspaperZombie::getWidth()  const { return NEWSPAPER_ZOMBIE_WALK_FULL_WIDTH; }
int NewspaperZombie::getHeight() const { return NEWSPAPER_ZOMBIE_WALK_FULL_HEIGHT; }

const unsigned char* NewspaperZombie::currentWalkFrame(int f) const {
    if (getDamageStage() == 1)
        return newspaper_zombie_walk_angry_frames[f % NEWSPAPER_ZOMBIE_WALK_ANGRY_FRAMES];
    return newspaper_zombie_walk_full_frames[f];
}

const unsigned char* NewspaperZombie::currentFightFrame(int f) const {
    if (getDamageStage() == 1)
        return newspaper_zombie_fight_angry_frames[f % NEWSPAPER_ZOMBIE_FIGHT_ANGRY_FRAMES];
    return newspaper_zombie_fight_full_frames[f];
}

int NewspaperZombie::currentWalkFrameCount() const { return NEWSPAPER_ZOMBIE_WALK_FULL_FRAMES; }
int NewspaperZombie::currentFightFrameCount() const { return NEWSPAPER_ZOMBIE_FIGHT_FULL_FRAMES; }

int NewspaperZombie::currentWalkWidth() const {
    if (getDamageStage() == 1) return NEWSPAPER_ZOMBIE_WALK_ANGRY_WIDTH;
    return NEWSPAPER_ZOMBIE_WALK_FULL_WIDTH;
}

int NewspaperZombie::currentWalkHeight() const {
    if (getDamageStage() == 1) return NEWSPAPER_ZOMBIE_WALK_ANGRY_HEIGHT;
    return NEWSPAPER_ZOMBIE_WALK_FULL_HEIGHT;
}

int NewspaperZombie::currentFightWidth() const {
    if (getDamageStage() == 1) return NEWSPAPER_ZOMBIE_FIGHT_ANGRY_WIDTH;
    return NEWSPAPER_ZOMBIE_FIGHT_FULL_WIDTH;
}

int NewspaperZombie::currentFightHeight() const {
    if (getDamageStage() == 1) return NEWSPAPER_ZOMBIE_FIGHT_ANGRY_HEIGHT;
    return NEWSPAPER_ZOMBIE_FIGHT_FULL_HEIGHT;
}

const unsigned char* NewspaperZombie::currentDeathFrame(int f) const {
    if (getDamageStage() == 1)
        return newspaper_zombie_death_angry_frames[f];
    return newspaper_zombie_death_full_frames[f];
}

int NewspaperZombie::currentDeathFrameCount() const {
    if (getDamageStage() == 1) return NEWSPAPER_ZOMBIE_DEATH_ANGRY_FRAMES;
    return NEWSPAPER_ZOMBIE_DEATH_FULL_FRAMES;
}

int NewspaperZombie::currentDeathWidth() const {
    if (getDamageStage() == 1) return NEWSPAPER_ZOMBIE_DEATH_ANGRY_WIDTH;
    return NEWSPAPER_ZOMBIE_DEATH_FULL_WIDTH;
}

int NewspaperZombie::currentDeathHeight() const {
    if (getDamageStage() == 1) return NEWSPAPER_ZOMBIE_DEATH_ANGRY_HEIGHT;
    return NEWSPAPER_ZOMBIE_DEATH_FULL_HEIGHT;
}

bool NewspaperZombie::hasDeathAnimation() const { return true; }
