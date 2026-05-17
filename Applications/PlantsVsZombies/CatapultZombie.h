#ifndef CATAPULT_ZOMBIE_H
#define CATAPULT_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class CatapultZombie : public Zombie {
public:
    static const int CATAPULT_HP    = 400;
    static const int SPEED_BONUS    = 2;
    static const int CRUSH_DAMAGE   = 300;
    static const int CRUSH_COOLDOWN = 200;

    CatapultZombie(int x, int y, int speedBonus = 0);

    int getWidth()  const override;
    int getHeight() const override;

    bool canCrush() const;
    void resetCrushCooldown();

protected:
    int crushCooldown;

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
};

#endif
