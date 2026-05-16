#ifndef POGO_ZOMBIE_H
#define POGO_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class PogoZombie : public Zombie {
public:
    static const int POGO_TOTAL_HP = 200;
    static const int POGO_HP       = 100; // damage absorbed before pogo breaks
    static const int POGO_SPEED    = 5;   // inherently very fast

    PogoZombie(int x, int y, int speedBonus = 0);

    bool canBeBlocked() const override;
    bool isOnPogo() const { return pogoHp > 0; }

protected:
    const unsigned char* walkFrame(int f) const override;
    const unsigned char* fightFrame(int f) const override;
    int walkFrameCount() const override;
    int fightFrameCount() const override;
    void onUpdate() override;

private:
    int pogoHp;
    int pogoDamageAccum; // tracks damage taken to decide when pogo breaks
};

#endif
