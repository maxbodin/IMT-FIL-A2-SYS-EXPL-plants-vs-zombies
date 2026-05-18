#ifndef MELONPULT_H
#define MELONPULT_H

#include <Applications/PlantsVsZombies/ShooterPlant.h>

class MelonPult : public ShooterPlant {
public:
    static const int COST        = 300;
    static const int HP          = 300;
    static const int SHOOT_DELAY = 250;

    MelonPult(int x, int y);

    BulletType getBulletType() const override;
    PlantType getPlantType() const override;

protected:
    const unsigned char* idleFrame(int f) const override;
    int idleFrameCount() const override;
    int spriteWidth() const override;
    int spriteHeight() const override;
};

#endif
