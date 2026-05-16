#ifndef DISCO_ZOMBIE_H
#define DISCO_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class DiscoZombie : public Zombie {
public:
    static const int DISCO_HP       = 300;
    static const int SUMMON_X       = 200; // X threshold to trigger summon
    static const int SUMMON_COUNT   = 4;   // backup dancers to spawn

    DiscoZombie(int x, int y, int speedBonus = 0);

    bool hasPendingSummon() const override;
    void consumeSummon() override;

protected:
    const unsigned char* walkFrame(int f) const override;
    const unsigned char* fightFrame(int f) const override;
    int walkFrameCount() const override;
    int fightFrameCount() const override;
    void onUpdate() override;

private:
    bool summonTriggered;
    bool summonPending;
};

#endif
