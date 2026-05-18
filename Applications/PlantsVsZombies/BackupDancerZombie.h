#ifndef BACKUPDANCER_ZOMBIE_H
#define BACKUPDANCER_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class BackupDancerZombie : public Zombie {
public:
    static const int DANCER_HP = 150;
    static const int DAMAGED_THRESHOLD = 100;
    static const int NO_ARM_THRESHOLD  = 50;
    static const int RISING_ANIM_SPEED = 8;

    BackupDancerZombie(int x, int y, int speedBonus = 0);

    void update() override;
    void render() override;

    int getWidth()  const override;
    int getHeight() const override;

    bool isRising() const;

protected:
    int lastDamageStage;
    bool rising;
    int risingFrame;
    int risingAnimTick;

    int getDamageStage() const;
    void onUpdate() override;

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
