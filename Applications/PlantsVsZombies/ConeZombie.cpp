#include <Applications/PlantsVsZombies/ConeZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/cone_zombie_walk_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/cone_zombie_walk_damaged_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/cone_zombie_walk_no_cone_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/cone_zombie_fight_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/cone_zombie_fight_damaged_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/cone_zombie_fight_no_cone_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/cone_zombie_death_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/cone_zombie_death_damaged_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/cone_zombie_death_no_cone_sprite.h>
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
            int maxY = Grid::OFFSET_Y + (Grid::ROWS - 1) * Grid::TILE_SIZE;
            if (newY >= minY && newY <= maxY) {
                y = newY;
            }
        }
    }
}

int ConeZombie::getWidth()  const { return CONE_ZOMBIE_WALK_FULL_WIDTH; }
int ConeZombie::getHeight() const { return CONE_ZOMBIE_WALK_FULL_HEIGHT; }

const unsigned char* ConeZombie::currentWalkFrame(int f) const {
    switch (getDamageStage()) {
        case 1:  return cone_zombie_walk_damaged_frames[f];
        case 2:  return cone_zombie_walk_no_cone_frames[f];
        default: return cone_zombie_walk_full_frames[f];
    }
}

const unsigned char* ConeZombie::currentFightFrame(int f) const {
    switch (getDamageStage()) {
        case 1:  return cone_zombie_fight_damaged_frames[f];
        case 2:  return cone_zombie_fight_no_cone_frames[f];
        default: return cone_zombie_fight_full_frames[f];
    }
}

int ConeZombie::currentWalkFrameCount() const { return CONE_ZOMBIE_WALK_FULL_FRAMES; }
int ConeZombie::currentFightFrameCount() const { return CONE_ZOMBIE_FIGHT_FULL_FRAMES; }
int ConeZombie::currentWalkWidth() const { return CONE_ZOMBIE_WALK_FULL_WIDTH; }
int ConeZombie::currentWalkHeight() const { return CONE_ZOMBIE_WALK_FULL_HEIGHT; }
int ConeZombie::currentFightWidth() const { return CONE_ZOMBIE_FIGHT_FULL_WIDTH; }
int ConeZombie::currentFightHeight() const { return CONE_ZOMBIE_FIGHT_FULL_HEIGHT; }

const unsigned char* ConeZombie::currentDeathFrame(int f) const {
    switch (getDamageStage()) {
        case 1:  return cone_zombie_death_damaged_frames[f];
        case 2:  return cone_zombie_death_no_cone_frames[f];
        default: return cone_zombie_death_full_frames[f];
    }
}

int ConeZombie::currentDeathFrameCount() const { return CONE_ZOMBIE_DEATH_FULL_FRAMES; }
int ConeZombie::currentDeathWidth() const { return CONE_ZOMBIE_DEATH_FULL_WIDTH; }
int ConeZombie::currentDeathHeight() const { return CONE_ZOMBIE_DEATH_FULL_HEIGHT; }
bool ConeZombie::hasDeathAnimation() const { return true; }
