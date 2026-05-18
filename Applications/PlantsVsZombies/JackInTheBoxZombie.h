#ifndef JACKINTHEBOX_ZOMBIE_H
#define JACKINTHEBOX_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class JackInTheBoxZombie : public Zombie {
public:
    static const int JACK_HP     = 400;
    static const int SPEED_BONUS = 2;
    static const int SPAWN_COUNT = 4; // zombies spawned on explosion

    JackInTheBoxZombie(int x, int y, int speedBonus = 0);

    int getWidth()  const override;
    int getHeight() const override;

    void onPlantContact() override;
    bool hasPendingExplosion() const override;
    void consumeExplosion() override;
    int explosionSpawnCount() const override;
    int deathSpawnCount() const override;

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
    bool explodeTriggered;
    bool explodePending;
};

#endif
