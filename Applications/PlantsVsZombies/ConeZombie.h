#ifndef CONE_ZOMBIE_H
#define CONE_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class ConeZombie : public Zombie {
public:
    static const int CONE_HP = 370;
    static const int LANE_SWITCH_CHANCE = 200; // 1 in N per update tick

    ConeZombie(int x, int y, int speedBonus = 0);

    int getWidth()  const override;
    int getHeight() const override;

protected:
    const unsigned char* walkFrame(int f) const override;
    const unsigned char* fightFrame(int f) const override;
    int walkFrameCount() const override;
    int fightFrameCount() const override;
    void onUpdate() override;

private:
    unsigned int rng;
};

#endif
