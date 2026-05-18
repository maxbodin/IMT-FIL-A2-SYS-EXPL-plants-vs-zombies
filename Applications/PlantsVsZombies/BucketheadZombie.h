#ifndef BUCKETHEAD_ZOMBIE_H
#define BUCKETHEAD_ZOMBIE_H

#include <Applications/PlantsVsZombies/Zombie.h>

class BucketheadZombie : public Zombie {
public:
    static const int BUCKET_HP = 650;
    static const int BUCKET_DAMAGED_THRESHOLD = 430;
    static const int BUCKET_LOST_THRESHOLD    = 200;

    BucketheadZombie(int x, int y, int speedBonus = 0);

    int getWidth()  const override;
    int getHeight() const override;

protected:
    int lastDamageStage;
    int getDamageStage() const;

    void onUpdate() override;

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
};

#endif
