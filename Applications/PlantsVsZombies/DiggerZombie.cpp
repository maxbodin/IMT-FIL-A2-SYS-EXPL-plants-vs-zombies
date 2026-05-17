#include <Applications/PlantsVsZombies/DiggerZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/digger_zombie_dig_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/digger_zombie_walk_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/digger_zombie_fight_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/digger_zombie_death_sprite.h>
#include <vga/vga.h>

DiggerZombie::DiggerZombie(int x, int y, int speedBonus)
    : Zombie(x, y, DIGGER_HP, speedBonus), digging(true),
      digFrame(0), digAnimTick(0) {}

void DiggerZombie::update() {
    if (state == DEAD) return;

    if (digging) {
        if (++digAnimTick >= DIG_ANIM_SPEED) {
            digAnimTick = 0;
            digFrame = (digFrame + 1) % DIGGER_ZOMBIE_DIG_FRAMES;
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
        draw_sprite(digger_zombie_dig_frames[digFrame],
                    DIGGER_ZOMBIE_DIG_WIDTH, DIGGER_ZOMBIE_DIG_HEIGHT, x, y);
        return;
    }
    Zombie::render();
}

bool DiggerZombie::canBeBlocked() const {
    return !digging;
}

bool DiggerZombie::isDigging() const { return digging; }

int DiggerZombie::getWidth()  const {
    if (digging) return DIGGER_ZOMBIE_DIG_WIDTH;
    return DIGGER_ZOMBIE_WALK_WIDTH;
}
int DiggerZombie::getHeight() const {
    if (digging) return DIGGER_ZOMBIE_DIG_HEIGHT;
    return DIGGER_ZOMBIE_WALK_HEIGHT;
}

const unsigned char* DiggerZombie::currentWalkFrame(int f)  const { return digger_zombie_walk_frames[f]; }
const unsigned char* DiggerZombie::currentFightFrame(int f) const { return digger_zombie_fight_frames[f]; }

int DiggerZombie::currentWalkFrameCount()  const { return DIGGER_ZOMBIE_WALK_FRAMES; }
int DiggerZombie::currentFightFrameCount() const { return DIGGER_ZOMBIE_FIGHT_FRAMES; }
int DiggerZombie::currentWalkWidth()  const { return DIGGER_ZOMBIE_WALK_WIDTH; }
int DiggerZombie::currentWalkHeight() const { return DIGGER_ZOMBIE_WALK_HEIGHT; }
int DiggerZombie::currentFightWidth()  const { return DIGGER_ZOMBIE_FIGHT_WIDTH; }
int DiggerZombie::currentFightHeight() const { return DIGGER_ZOMBIE_FIGHT_HEIGHT; }

const unsigned char* DiggerZombie::currentDeathFrame(int f) const { return digger_zombie_death_frames[f]; }
int DiggerZombie::currentDeathFrameCount() const { return DIGGER_ZOMBIE_DEATH_FRAMES; }
int DiggerZombie::currentDeathWidth()  const { return DIGGER_ZOMBIE_DEATH_WIDTH; }
int DiggerZombie::currentDeathHeight() const { return DIGGER_ZOMBIE_DEATH_HEIGHT; }
bool DiggerZombie::hasDeathAnimation() const { return true; }
