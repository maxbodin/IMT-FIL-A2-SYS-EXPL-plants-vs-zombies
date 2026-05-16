#ifndef SPLITPEA_H
#define SPLITPEA_H

#include <Applications/PlantsVsZombies/ShooterPlant.h>

class SplitPea : public ShooterPlant {
public:
    static const int COST        = 125;
    static const int HP          = 300;
    static const int SHOOT_DELAY = 150;

    SplitPea(int x, int y);

    BulletType getBulletType() const override;
    PlantType getPlantType() const override;

    bool shootsBackward() const { return true; }

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
