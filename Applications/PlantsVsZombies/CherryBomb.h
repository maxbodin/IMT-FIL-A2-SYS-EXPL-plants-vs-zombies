#ifndef CHERRYBOMB_H
#define CHERRYBOMB_H

#include <Applications/PlantsVsZombies/Plant.h>

class CherryBomb : public Plant {
public:
    static const int COST                 = 150;
    static const int HP                   = 100;
    static const int EXPLOSION_DAMAGE     = 300;
    static const int EXPLOSION_DURATION   = 300;
    static const int EXPLOSION_ANIM_SPEED = 6;
    static const int ATTACKING_ANIM_SPEED = 8;
    static const int ATTACKING_DURATION   = 120;

    CherryBomb(int x, int y);

    void update() override;
    void render() override;
    PlantType getPlantType() const override;

    void explode();
    bool isExploding() const;
    bool isAttacking() const;
    int getExplosionDamage() const;

protected:
    const unsigned char* idleFrame(int f) const override;
    int idleFrameCount() const override;
    int spriteWidth() const override;
    int spriteHeight() const override;

private:
    bool attacking;
    int attackingStartTick;
    int attackingFrame;
    int attackingAnimTick;

    bool exploding;
    int explosionStartTick;
    int explosionFrame;
    int explosionAnimTick;
};

#endif
