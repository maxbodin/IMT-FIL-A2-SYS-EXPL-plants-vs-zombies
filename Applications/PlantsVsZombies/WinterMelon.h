#ifndef WINTERMELON_H
#define WINTERMELON_H

#include <Applications/PlantsVsZombies/ShooterPlant.h>

class WinterMelon : public ShooterPlant {
public:
    static const int COST        = 500;
    static const int HP          = 300;
    static const int SHOOT_DELAY = 250;

    WinterMelon(int x, int y);

    BulletType getBulletType() const override;
    PlantType getPlantType() const override;

protected:
    const unsigned char* idleFrame(int f) const override;
    int idleFrameCount() const override;
    int spriteWidth() const override;
    int spriteHeight() const override;
};

#endif
