#include <Applications/PlantsVsZombies/ConeZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/cone/cone_zombie_walk_cone_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/cone/cone_zombie_walk_cone_damaged_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/basic/basic_zombie_walk_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/cone/cone_zombie_fight_cone_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/cone/cone_zombie_fight_cone_damaged_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/basic/basic_zombie_fight_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/basic/basic_zombie_death_sprite.h>
#include <Applications/PlantsVsZombies/Grid.h>

extern volatile int compt;

ConeZombie::ConeZombie(int x, int y, int speedBonus)
    : Zombie(x, y, CONE_HP, speedBonus), rng((unsigned int)(x * 31 + y * 17)), lastDamageStage(0) {}

int ConeZombie::getDamageStage() const {
    if (state == DYING) return lastDamageStage;
    if (hp > CONE_DAMAGED_THRESHOLD) return 0;
    if (hp > CONE_LOST_THRESHOLD)    return 1;
    return 2;
}

void ConeZombie::onUpdate() {
    // Save damage stage while alive (before potential death)
    if (hp > CONE_DAMAGED_THRESHOLD) lastDamageStage = 0;
    else if (hp > CONE_LOST_THRESHOLD) lastDamageStage = 1;
    else lastDamageStage = 2;

    if (state != BLOCKED) {
        rng = rng * 1103515245 + 12345;
        int roll = (int)((rng >> 16) & 0x7FFF);
        if (roll % LANE_SWITCH_CHANCE == 0) {
            int dir = (roll / LANE_SWITCH_CHANCE) % 2 == 0 ? -1 : 1;
            int newY = y + dir * Grid::TILE_SIZE;
            int minY = Grid::OFFSET_Y;
            int maxY = Grid::OFFSET_Y + (Grid::ROWS - 2) * Grid::TILE_SIZE;
            if (newY >= minY && newY <= maxY) {
                y = newY;
            }
        }
    }
}

int ConeZombie::getWidth()  const { return CONE_ZOMBIE_WALK_CONE_FULL_WIDTH; }
int ConeZombie::getHeight() const { return CONE_ZOMBIE_WALK_CONE_FULL_HEIGHT; }

const unsigned char* ConeZombie::currentWalkFrame(int f) const {
    switch (getDamageStage()) {
        case 1:  return cone_zombie_walk_cone_damaged_frames[f];
        case 2:  return basic_zombie_walk_full_frames[f];
        default: return cone_zombie_walk_cone_full_frames[f];
    }
}

const unsigned char* ConeZombie::currentFightFrame(int f) const {
    switch (getDamageStage()) {
        case 1:  return cone_zombie_fight_cone_damaged_frames[f];
        case 2:  return basic_zombie_fight_full_frames[f];
        default: return cone_zombie_fight_cone_full_frames[f];
    }
}

int ConeZombie::currentWalkFrameCount() const {
    if (getDamageStage() == 2) return BASIC_ZOMBIE_WALK_FULL_FRAMES;
    return CONE_ZOMBIE_WALK_CONE_FULL_FRAMES;
}
int ConeZombie::currentFightFrameCount() const {
    if (getDamageStage() == 2) return BASIC_ZOMBIE_FIGHT_FULL_FRAMES;
    return CONE_ZOMBIE_FIGHT_CONE_FULL_FRAMES;
}
int ConeZombie::currentWalkWidth() const {
    if (getDamageStage() == 2) return BASIC_ZOMBIE_WALK_FULL_WIDTH;
    return CONE_ZOMBIE_WALK_CONE_FULL_WIDTH;
}
int ConeZombie::currentWalkHeight() const {
    if (getDamageStage() == 2) return BASIC_ZOMBIE_WALK_FULL_HEIGHT;
    return CONE_ZOMBIE_WALK_CONE_FULL_HEIGHT;
}
int ConeZombie::currentFightWidth() const {
    if (getDamageStage() == 2) return BASIC_ZOMBIE_FIGHT_FULL_WIDTH;
    return CONE_ZOMBIE_FIGHT_CONE_FULL_WIDTH;
}
int ConeZombie::currentFightHeight() const {
    if (getDamageStage() == 2) return BASIC_ZOMBIE_FIGHT_FULL_HEIGHT;
    return CONE_ZOMBIE_FIGHT_CONE_FULL_HEIGHT;
}

const unsigned char* ConeZombie::currentDeathFrame(int f) const {
    switch (getDamageStage()) {
        case 1:  return basic_zombie_death_frames[f];
        case 2:  return basic_zombie_death_frames[f];
        default: return basic_zombie_death_frames[f];
    }
}

int ConeZombie::currentDeathFrameCount() const { return BASIC_ZOMBIE_DEATH_FRAMES; }
int ConeZombie::currentDeathWidth() const { return BASIC_ZOMBIE_DEATH_WIDTH; }
int ConeZombie::currentDeathHeight() const { return BASIC_ZOMBIE_DEATH_HEIGHT; }
bool ConeZombie::hasDeathAnimation() const { return true; }
