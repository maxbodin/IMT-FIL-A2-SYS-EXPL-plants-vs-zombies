#ifndef NEWSPAPER_ZOMBIE_H
#define NEWSPAPER_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class NewspaperZombie : public Zombie {
public:
    static const int NEWSPAPER_HP       = 200;
    static const int ANGRY_THRESHOLD    = 100; // loses newspaper, gets angry
    static const int ANGRY_SPEED_BONUS  = 2;

    NewspaperZombie(int x, int y, int speedBonus = 0);

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
    bool angry;
    int lastDamageStage;
    int getDamageStage() const;
};

#endif
