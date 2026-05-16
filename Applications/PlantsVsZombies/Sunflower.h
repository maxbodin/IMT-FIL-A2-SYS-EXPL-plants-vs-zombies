#ifndef SUNFLOWER_H
#define SUNFLOWER_H

#include <Applications/PlantsVsZombies/Plant.h>

class Sunflower : public Plant {
public:
    static const int COST        = 50;
    static const int SUN_MIN_DELAY = 5000;
    static const int SUN_MAX_DELAY = 9000;

    Sunflower(int x, int y);

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

    int randomSunDelay();
};

#endif
