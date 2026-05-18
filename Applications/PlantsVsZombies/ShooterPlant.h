#ifndef SHOOTER_PLANT_H
#define SHOOTER_PLANT_H

#include <Applications/PlantsVsZombies/Plant.h>

class ShooterPlant : public Plant {
public:
    ShooterPlant(int x, int y, int hp, int shootDelay, int shootHold = 18);

    void update() override;
    void render() override;
    bool canShoot() const override;
    void resetCooldown() override;

protected:
    int cooldown;
    int shootDelay;
    bool shooting;
    int shootIdx;
    int shootAnimTick;
    int shootHoldTicks;
    int shootHold;

    static const int SHOOT_ANIM_SPEED = 6;

    virtual const unsigned char* shootingSprite(int f) const;
    virtual int shootingSpriteCount() const;
    virtual int shootingSpriteWidth() const;
    virtual int shootingSpriteHeight() const;
};

#endif
