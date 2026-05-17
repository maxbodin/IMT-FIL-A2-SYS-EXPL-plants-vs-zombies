#ifndef POLEVAULTING_ZOMBIE_H
#define POLEVAULTING_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class PoleVaultingZombie : public Zombie {
public:
    static const int VAULT_HP    = 300;
    static const int SPEED_BONUS = 3;   // fast runner
    static const int VAULT_ANIM_SPEED = 6;

    PoleVaultingZombie(int x, int y, int speedBonus = 0);

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
    bool hasVaulted;
    bool vaulting;
    int vaultFrame;
    int vaultAnimTick;
};

#endif
