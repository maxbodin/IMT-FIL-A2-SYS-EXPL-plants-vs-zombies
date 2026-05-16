#ifndef FOOTBALL_ZOMBIE_H
#define FOOTBALL_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class FootballZombie : public Zombie {
public:
    static const int FOOTBALL_HP    = 500;
    static const int SPEED_BONUS    = 3; // inherently faster than other zombies

    FootballZombie(int x, int y, int speedBonus = 0);

protected:
    const unsigned char* walkFrame(int f) const override;
    const unsigned char* fightFrame(int f) const override;
    int walkFrameCount() const override;
    int fightFrameCount() const override;
};

#endif
