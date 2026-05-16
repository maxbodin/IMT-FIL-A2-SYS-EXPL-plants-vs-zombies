#ifndef MARIGOLD_H
#define MARIGOLD_H

#include <Applications/PlantsVsZombies/Plant.h>

class Marigold : public Plant {
public:
    static const int COST          = 50;
    static const int HP            = 200;
    static const int SUN_MIN_DELAY = 6000;
    static const int SUN_MAX_DELAY = 10000;
    static const int PRODUCING_DURATION = 60;
    static const int PRODUCING_ANIM_SPEED = 3;

    Marigold(int x, int y);

    void update() override;
    void render() override;
    PlantType getPlantType() const override;
    bool hasSunReady() const override;
    void resetSunTimer() override;

protected:
    const unsigned char* idleFrame(int f) const override;
    int idleFrameCount() const override;
    int spriteWidth() const override;
    int spriteHeight() const override;

private:
    int nextSunTick;
    bool producing;
    int producingTick;
    int producingFrame;
    int producingAnimTick;
    int randomSunDelay();
};

#endif
