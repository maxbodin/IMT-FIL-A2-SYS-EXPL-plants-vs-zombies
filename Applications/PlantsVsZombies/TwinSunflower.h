#ifndef TWINSUNFLOWER_H
#define TWINSUNFLOWER_H

#include <Applications/PlantsVsZombies/Plant.h>

class TwinSunflower : public Plant {
public:
    static const int COST          = 150;
    static const int SUN_MIN_DELAY = 5000;
    static const int SUN_MAX_DELAY = 9000;

    TwinSunflower(int x, int y);

    PlantType getPlantType() const override;

    bool hasSunReady() const override;
    void resetSunTimer() override;

    int sunCount() const { return 2; }

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
