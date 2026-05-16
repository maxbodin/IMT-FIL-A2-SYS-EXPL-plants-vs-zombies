#ifndef PLANT_H
#define PLANT_H

#include <Applications/PlantsVsZombies/Entity.h>
#include <Applications/PlantsVsZombies/Bullet.h>
#include <Applications/PlantsVsZombies/PlantType.h>

class Plant : public Entity {
public:
    Plant(int x, int y, int hp);

    virtual void update() override;
    virtual void render() override;

    virtual PlantType getPlantType() const = 0;

    virtual bool canShoot() const;
    virtual BulletType getBulletType() const;
    virtual void resetCooldown();

    virtual bool hasSunReady() const;
    virtual void resetSunTimer();

    virtual int getWidth() const override;
    virtual int getHeight() const override;

protected:
    int frame;
    int animTick;
    static const int ANIM_SPEED = 8;

    virtual const unsigned char* idleFrame(int f) const;
    virtual int idleFrameCount() const;
    virtual int spriteWidth() const;
    virtual int spriteHeight() const;
};

#endif
