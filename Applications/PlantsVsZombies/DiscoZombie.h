#ifndef DISCO_ZOMBIE_H
#define DISCO_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class DiscoZombie : public Zombie {
public:
    static const int DISCO_HP       = 300;
    static const int SPEED_BONUS    = 3;
    static const int SUMMON_X       = 200;
    static const int SUMMON_COUNT   = 4;
    static const int MAX_SUMMONS    = 4;
    static const int DANCE_SPEED    = 7; // faster animation when dancing
    static const int HEADLESS_THRESHOLD = 200;
    static const int ARMLESS_THRESHOLD  = 100;
    static const int ARMLESS_HEADLESS_THRESHOLD = 50;

    DiscoZombie(int x, int y, int speedBonus = 0);

    bool hasPendingSummon() const override;
    void consumeSummon() override;

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
    int summonCount;
    bool summonPending;
    bool dancing; // dance walk mode before summoning
    int lastDamageStage;
    int getDamageStage() const;
};

#endif
