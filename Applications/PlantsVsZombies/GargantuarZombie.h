#ifndef GARGANTUAR_ZOMBIE_H
#define GARGANTUAR_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class GargantuarZombie : public Zombie {
public:
    static const int GARGANTUAR_HP = 3000;
    static const int CRUSH_DAMAGE  = 300;
    static const int CRUSH_COOLDOWN = 200;

    GargantuarZombie(int x, int y, int speedBonus = 0);

    int getWidth()  const override;
    int getHeight() const override;

    bool canCrush() const;
    void resetCrushCooldown();

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
    int crushCooldown;
};

#endif
