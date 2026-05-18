#ifndef BASEBALL_ZOMBIE_H
#define BASEBALL_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class BaseballZombie : public Zombie {
public:
    static const int BASEBALL_HP    = 250;
    static const int SPEED_BONUS    = 2;
    static const int BAT_DAMAGE     = 40;
    static const int DAMAGED_THRESHOLD = 165;
    static const int NO_BAT_THRESHOLD  = 80;
    static const int HEADLESS_THRESHOLD = 40;

    BaseballZombie(int x, int y, int speedBonus = 0);

    int getWidth()  const override;
    int getHeight() const override;

protected:
    int lastDamageStage;
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
