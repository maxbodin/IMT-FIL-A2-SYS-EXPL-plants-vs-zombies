#ifndef FOOTBALL_ZOMBIE_H
#define FOOTBALL_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class FootballZombie : public Zombie {
public:
    static const int FOOTBALL_HP    = 500;
    static const int SPEED_BONUS    = 10;   // very fast
    static const int DAMAGE1_THRESHOLD = 375; // below: damage1
    static const int DAMAGE2_THRESHOLD = 250; // below: damage2
    static const int NO_HELMET_THRESHOLD = 125; // below: no helmet
    static const int HEADLESS_THRESHOLD = 60;  // below: headless
    static const int ARMLESS_THRESHOLD = 30;   // below: armless

    FootballZombie(int x, int y, int speedBonus = 0);

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
    int getDamageStage() const;
};

#endif
