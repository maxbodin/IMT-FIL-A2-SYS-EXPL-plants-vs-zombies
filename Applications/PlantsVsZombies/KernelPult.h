#ifndef KERNELPULT_H
#define KERNELPULT_H

#include <Applications/PlantsVsZombies/ShooterPlant.h>

class KernelPult : public ShooterPlant {
public:
    static const int COST        = 100;
    static const int HP          = 300;
    static const int SHOOT_DELAY = 200;

    KernelPult(int x, int y);

    BulletType getBulletType() const override;
    PlantType getPlantType() const override;

protected:
    const unsigned char* idleFrame(int f) const override;
    int idleFrameCount() const override;
    int spriteWidth() const override;
    int spriteHeight() const override;

    const unsigned char* shootingSprite(int f) const override;
    int shootingSpriteCount() const override;
    int shootingSpriteWidth() const override;
    int shootingSpriteHeight() const override;
};

#endif
