#ifndef BALLOON_ZOMBIE_H
#define BALLOON_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class BalloonZombie : public Zombie {
public:
    static const int BALLOON_TOTAL_HP  = 200;
    static const int BALLOON_HP        = 80;  // damage to pop balloon
    static const int NO_ARM_THRESHOLD  = 50;  // damage stage: no arm
    static const int POP_ANIM_SPEED    = 6;

    BalloonZombie(int x, int y, int speedBonus = 0);

    bool canBeBlocked() const override;
    bool isFlying() const override;

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
    int balloonHp;
    bool balloonPopped;
    bool popping;       // currently playing pop animation
    int popFrame;
    int popAnimTick;
    int lastDamageStage;
    int getDamageStage() const;
};

#endif
