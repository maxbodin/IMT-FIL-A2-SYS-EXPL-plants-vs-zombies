#include <Applications/PlantsVsZombies/ConeZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/cone_zombie_walk_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/cone_zombie_fight_sprite.h>
#include <Applications/PlantsVsZombies/Grid.h>

extern volatile int compt;

ConeZombie::ConeZombie(int x, int y, int speedBonus)
    : Zombie(x, y, CONE_HP, speedBonus), rng((unsigned int)(x * 31 + y * 17)) {}

void ConeZombie::onUpdate() {
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

int ConeZombie::getWidth()  const { return CONE_ZOMBIE_WALK_WIDTH; }
int ConeZombie::getHeight() const { return CONE_ZOMBIE_WALK_HEIGHT; }

const unsigned char* ConeZombie::walkFrame(int f)  const { return cone_zombie_walk_frames[f]; }
const unsigned char* ConeZombie::fightFrame(int f) const { return cone_zombie_fight_frames[f]; }
int ConeZombie::walkFrameCount()  const { return CONE_ZOMBIE_WALK_FRAMES; }
int ConeZombie::fightFrameCount() const { return CONE_ZOMBIE_FIGHT_FRAMES; }
