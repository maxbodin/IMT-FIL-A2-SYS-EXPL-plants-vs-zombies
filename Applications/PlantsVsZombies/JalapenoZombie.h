#ifndef JALAPENO_ZOMBIE_H
#define JALAPENO_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class JalapenoZombie : public Zombie {
public:
    static const int JALAPENO_ZOMBIE_HP = 200;
    static const int SPEED_BONUS = 1;

    JalapenoZombie(int x, int y, int speedBonus = 0);

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
};

#endif
