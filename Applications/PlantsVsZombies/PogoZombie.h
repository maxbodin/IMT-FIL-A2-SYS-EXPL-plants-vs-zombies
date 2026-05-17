#ifndef POGO_ZOMBIE_H
#define POGO_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class PogoZombie : public Zombie {
public:
    static const int POGO_TOTAL_HP      = 300;
    static const int POGO_HP            = 150; // damage to break the pogo stick
    static const int POGO_SPEED         = 3;   // extra speed while on pogo
    static const int HEADLESS_THRESHOLD = 80;
    static const int ARMLESS_THRESHOLD  = 40;

    PogoZombie(int x, int y, int speedBonus = 0);

    bool canBeBlocked() const override;

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
    int pogoHp;
    int lastDamageStage;
    int getDamageStage() const;
};

#endif
