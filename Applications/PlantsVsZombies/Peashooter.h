#ifndef PEASHOOTER_H
#define PEASHOOTER_H

#include <Applications/PlantsVsZombies/ShooterPlant.h>

class Peashooter : public ShooterPlant {
public:
    static const int HP          = 300;
    static const int SHOOT_DELAY = 150; // ticks entre chaque tir
    static const int COST        = 100; // soleils nécessaires pour placer

    Peashooter(int x, int y);

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
