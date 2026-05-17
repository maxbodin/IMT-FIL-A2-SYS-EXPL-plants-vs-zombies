#include <Applications/PlantsVsZombies/DiggerZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/digger/digger_zombie_digging_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/digger/digger_zombie_walk_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/digger/digger_zombie_fight_full_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/digger/digger_zombie_death_no_arm_sprite.h>
#include <vga/vga.h>

DiggerZombie::DiggerZombie(int x, int y, int speedBonus)
    : Zombie(x, y, DIGGER_HP, speedBonus), digging(true),
      digFrame(0), digAnimTick(0) {}

void DiggerZombie::update() {
    if (state == DEAD) return;

    if (digging) {
        if (++digAnimTick >= DIG_ANIM_SPEED) {
            digAnimTick = 0;
            digFrame = (digFrame + 1) % DIGGER_ZOMBIE_DIGGING_FRAMES;
            x -= Zombie::WALK_STEP;
        }
        if (x <= EMERGE_X) {
            digging = false;
        }
        return;
    }
    Zombie::update();
}

void DiggerZombie::render() {
    if (state == DEAD) return;

    if (digging) {
        draw_sprite(digger_zombie_digging_frames[digFrame],
                    DIGGER_ZOMBIE_DIGGING_WIDTH, DIGGER_ZOMBIE_DIGGING_HEIGHT, x, y);
        return;
    }
    Zombie::render();
}

bool DiggerZombie::canBeBlocked() const {
    return !digging;
}

bool DiggerZombie::isDigging() const { return digging; }

int DiggerZombie::getWidth()  const {
    if (digging) return DIGGER_ZOMBIE_DIGGING_WIDTH;
    return DIGGER_ZOMBIE_WALK_FULL_WIDTH;
}
int DiggerZombie::getHeight() const {
    if (digging) return DIGGER_ZOMBIE_DIGGING_HEIGHT;
    return DIGGER_ZOMBIE_WALK_FULL_HEIGHT;
}

const unsigned char* DiggerZombie::currentWalkFrame(int f)  const { return digger_zombie_walk_full_frames[f]; }
const unsigned char* DiggerZombie::currentFightFrame(int f) const { return digger_zombie_fight_full_frames[f]; }

int DiggerZombie::currentWalkFrameCount()  const { return DIGGER_ZOMBIE_WALK_FULL_FRAMES; }
int DiggerZombie::currentFightFrameCount() const { return DIGGER_ZOMBIE_FIGHT_FULL_FRAMES; }
int DiggerZombie::currentWalkWidth()  const { return DIGGER_ZOMBIE_WALK_FULL_WIDTH; }
int DiggerZombie::currentWalkHeight() const { return DIGGER_ZOMBIE_WALK_FULL_HEIGHT; }
int DiggerZombie::currentFightWidth()  const { return DIGGER_ZOMBIE_FIGHT_FULL_WIDTH; }
int DiggerZombie::currentFightHeight() const { return DIGGER_ZOMBIE_FIGHT_FULL_HEIGHT; }

const unsigned char* DiggerZombie::currentDeathFrame(int f) const { return digger_zombie_death_no_arm_frames[f]; }
int DiggerZombie::currentDeathFrameCount() const { return DIGGER_ZOMBIE_DEATH_NO_ARM_FRAMES; }
int DiggerZombie::currentDeathWidth()  const { return DIGGER_ZOMBIE_DEATH_NO_ARM_WIDTH; }
int DiggerZombie::currentDeathHeight() const { return DIGGER_ZOMBIE_DEATH_NO_ARM_HEIGHT; }
bool DiggerZombie::hasDeathAnimation() const { return true; }
