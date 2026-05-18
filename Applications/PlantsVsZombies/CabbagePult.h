#ifndef CABBAGEPULT_H
#define CABBAGEPULT_H

#include <Applications/PlantsVsZombies/ShooterPlant.h>

class CabbagePult : public ShooterPlant {
public:
    static const int COST        = 100;
    static const int HP          = 300;
    static const int SHOOT_DELAY = 200;

    CabbagePult(int x, int y);

    void render() override;
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
