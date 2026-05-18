#ifndef CONE_ZOMBIE_H
#define CONE_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class ConeZombie : public Zombie {
public:
    static const int CONE_HP = 370;
    static const int CONE_DAMAGED_THRESHOLD = 245; // below this: damaged cone
    static const int CONE_LOST_THRESHOLD    = 120; // below this: no cone
    static const int LANE_SWITCH_CHANCE = 200;

    ConeZombie(int x, int y, int speedBonus = 0);

    int getWidth()  const override;
    int getHeight() const override;

protected:
    const unsigned char* currentWalkFrame(int f) const override;
    const unsigned char* currentFightFrame(int f) const override;
    int currentWalkFrameCount() const override;
    int currentFightFrameCount() const override;
    int currentWalkWidth() const override;
    int currentWalkHeight() const override;
    int currentFightWidth() const override;
    int currentFightHeight() const override;

    const unsigned char* currentDeathFrame(int f) const override;
    int currentDeathFrameCount() const override;
    int currentDeathWidth() const override;
    int currentDeathHeight() const override;
    bool hasDeathAnimation() const override;

    void onUpdate() override;

private:
    unsigned int rng;
    int lastDamageStage; // preserved at death for death animation
    int getDamageStage() const;
};

#endif
