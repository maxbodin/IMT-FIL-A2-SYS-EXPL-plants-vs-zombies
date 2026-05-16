#ifndef SUNFLOWER_H
#define SUNFLOWER_H

#include <Applications/PlantsVsZombies/Plant.h>

class Sunflower : public Plant {
public:
    static const int COST             = 50;
    static const int SUN_MIN_DELAY    = 5000;
    static const int SUN_MAX_DELAY    = 9000;
    static const int PRODUCING_DURATION = 120;
    static const int PRODUCING_ANIM_SPEED = 6;

    Sunflower(int x, int y);

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
    int producingStartTick;
    int producingFrame;
    int producingAnimTick;

    int randomSunDelay();
};

#endif
