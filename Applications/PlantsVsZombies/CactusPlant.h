#ifndef CACTUS_PLANT_H
#define CACTUS_PLANT_H

#include <Applications/PlantsVsZombies/ShooterPlant.h>

class CactusPlant : public ShooterPlant {
public:
    static const int COST            = 125;
    static const int HP              = 300;
    static const int SHOOT_DELAY     = 120;
    static const int GROW_ANIM_SPEED = 8;

    CactusPlant(int x, int y);

    void update() override;
    void render() override;

    BulletType getBulletType() const override;
    PlantType getPlantType() const override;

    /* When extended, the cactus can hit zombies that jump or fly. */
    bool isExtended() const { return extended; }
    void setZombieInRange(bool v) { zombieInRange = v; }

protected:
    const unsigned char* idleFrame(int f) const override;
    int idleFrameCount() const override;
    int spriteWidth() const override;
    int spriteHeight() const override;

    const unsigned char* shootingSprite(int f) const override;
    int shootingSpriteCount() const override;
    int shootingSpriteWidth() const override;
    int shootingSpriteHeight() const override;

private:
    bool extended;
    bool growing;
    bool shrinking;
    bool zombieInRange;
    int growFrame;
    int growAnimTick;
};

#endif
