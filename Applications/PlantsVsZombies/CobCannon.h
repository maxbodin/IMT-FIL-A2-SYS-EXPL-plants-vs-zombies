#ifndef COBCANNON_H
#define COBCANNON_H

#include <Applications/PlantsVsZombies/Plant.h>

class CobCannon : public Plant {
public:
    static const int COST = 500;
    static const int HP   = 400;

    static const int ANIM_SPEED_LOADING = 30;
    static const int ANIM_SPEED_READY   = 12;
    static const int ANIM_SPEED_RISING  = 8;
    static const int ANIM_SPEED_FIRING  = 6;
    static const int ANIM_SPEED_USED    = 10;

    enum CannonState { LOADING, READY, RISING, FIRING, USED };

    CobCannon(int x, int y);

    void update() override;
    void render() override;
    PlantType getPlantType() const override;

    bool canShoot() const override;
    void resetCooldown() override;
    BulletType getBulletType() const override;

protected:
    const unsigned char* idleFrame(int f) const override;
    int idleFrameCount() const override;
    int spriteWidth() const override;
    int spriteHeight() const override;

private:
    CannonState cannonState;
    int stateFrame;
    int stateAnimTick;
};

#endif
