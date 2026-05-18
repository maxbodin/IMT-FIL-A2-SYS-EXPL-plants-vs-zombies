#ifndef YETI_ZOMBIE_H
#define YETI_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class YetiZombie : public Zombie {
public:
    static const int YETI_HP            = 2500;
    static const int DAMAGED_THRESHOLD  = 1800;
    static const int HEADLESS_THRESHOLD = 1000;
    static const int NO_ARM_THRESHOLD   = 400;
    static const int ATTACK_MULTIPLIER  = 3;

    YetiZombie(int x, int y, int speedBonus = 0);

    int getWidth()  const override;
    int getHeight() const override;
    int getAttackDamage() const override;

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
    int lastDamageStage;
    int getDamageStage() const;
};

#endif
