#ifndef DIGGER_ZOMBIE_H
#define DIGGER_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class DiggerZombie : public Zombie {
public:
    static const int DIGGER_HP      = 300;
    static const int EMERGE_X       = 40;
    static const int DIG_ANIM_SPEED = 6;

    DiggerZombie(int x, int y, int speedBonus = 0);

    void update() override;
    void render() override;

    bool canBeBlocked() const override;

    int getWidth()  const override;
    int getHeight() const override;

    bool isDigging() const;

protected:
    bool digging;
    int digFrame;
    int digAnimTick;

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
