#include <Applications/PlantsVsZombies/CatapultZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/catapult/catapult_zombie_walk_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/catapult/catapult_zombie_launch_sequence_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/basic/basic_zombie_death_sprite.h>
#include <Applications/PlantsVsZombies/Grid.h>

CatapultZombie::CatapultZombie(int x, int y, int speedBonus)
    : Zombie(x, y, CATAPULT_HP, speedBonus + SPEED_BONUS),
      crushCooldown(0), throwCooldown(THROW_COOLDOWN / 2),
      throwing(false), throwFrame(0), throwAnimTick(0),
      ballReady(false), ballTargetX(0) {}

void CatapultZombie::onUpdate() {
    if (crushCooldown > 0) crushCooldown--;
    if (throwCooldown > 0) throwCooldown--;

    if (throwing) {
        if (++throwAnimTick >= THROW_ANIM_SPEED) {
            throwAnimTick = 0;
            throwFrame++;
            if (throwFrame == CATAPULT_ZOMBIE_LAUNCH_SEQUENCE_FRAMES / 2) {
                ballReady = true;
            }
            if (throwFrame >= CATAPULT_ZOMBIE_LAUNCH_SEQUENCE_FRAMES) {
                throwing = false;
                throwFrame = 0;
                throwCooldown = THROW_COOLDOWN;
            }
        }
    }
}

bool CatapultZombie::canCrush() const {
    return state != DYING && state != DEAD && crushCooldown == 0;
}

void CatapultZombie::resetCrushCooldown() {
    crushCooldown = CRUSH_COOLDOWN;
}

bool CatapultZombie::canThrow() const {
    return state != DYING && state != DEAD && !throwing && throwCooldown == 0;
}

void CatapultZombie::startThrow() {
    throwing = true;
    throwFrame = 0;
    throwAnimTick = 0;
    ballReady = false;
    ballTargetX = x - (Grid::TILE_SIZE * 2 + Grid::TILE_SIZE / 2);
    if (ballTargetX < Grid::OFFSET_X) ballTargetX = Grid::OFFSET_X;
}

bool CatapultZombie::hasBallReady() const {
    return ballReady;
}

void CatapultZombie::consumeBall() {
    ballReady = false;
}

int CatapultZombie::getBallTargetY() const {
    return y + getHeight() / 2;
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
