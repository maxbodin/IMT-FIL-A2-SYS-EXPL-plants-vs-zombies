#ifndef JACKINTHEBOX_ZOMBIE_H
#define JACKINTHEBOX_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class JackInTheBoxZombie : public Zombie {
public:
    static const int JACK_HP     = 200;
    static const int SPEED_BONUS = 2;
    static const int EXPLODE_X   = 80; // explodes when reaching this x

    JackInTheBoxZombie(int x, int y, int speedBonus = 0);

    int getWidth()  const override;
    int getHeight() const override;

    bool hasPendingExplosion() const;
    void consumeExplosion();

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
    bool explodeTriggered;
    bool explodePending;
};

#endif
