#ifndef SCREENDOOR_ZOMBIE_H
#define SCREENDOOR_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class ScreenDoorZombie : public Zombie {
public:
    static const int SCREENDOOR_HP       = 400;
    static const int DAMAGED_THRESHOLD   = 260;
    static const int NO_DOOR_THRESHOLD   = 130;
    static const int RESTORED_HP         = 300;  // HP after screen breaks

    ScreenDoorZombie(int x, int y, int speedBonus = 0);

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
    int lastDamageStage;
    bool doorBroken;
    int getDamageStage() const;
};

#endif
