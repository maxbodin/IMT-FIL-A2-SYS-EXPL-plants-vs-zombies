#include <Applications/PlantsVsZombies/FootballZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/football_zombie_walk_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/football_zombie_fight_sprite.h>

FootballZombie::FootballZombie(int x, int y, int speedBonus)
    : Zombie(x, y, FOOTBALL_HP, speedBonus + SPEED_BONUS) {}

const unsigned char* FootballZombie::walkFrame(int f)  const { return football_zombie_walk_frames[f]; }
const unsigned char* FootballZombie::fightFrame(int f) const { return football_zombie_fight_frames[f]; }
int FootballZombie::walkFrameCount()  const { return FOOTBALL_ZOMBIE_WALK_FRAMES; }
int FootballZombie::fightFrameCount() const { return FOOTBALL_ZOMBIE_FIGHT_FRAMES; }
