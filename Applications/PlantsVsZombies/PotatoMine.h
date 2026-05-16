#ifndef POTATO_MINE_H
#define POTATO_MINE_H

#include <Applications/PlantsVsZombies/Plant.h>

class PotatoMine : public Plant {
public:
    static const int COST               = 25;
    static const int EXPLOSION_DAMAGE   = 200;
    static const int EXPLOSION_DURATION = 400;  // ticks for explosion animation

    PotatoMine(int x, int y);

    void update() override;
    void render() override;
    PlantType getPlantType() const override;

    /* Trigger explosion (called when zombie steps on mine). */
    void explode();
    bool isExploding()    const { return exploding; }
    bool isExplosionDone() const { return explosionDone; }
    int  getExplosionDamage() const { return EXPLOSION_DAMAGE; }

protected:
    const unsigned char* idleFrame(int f) const override;
    int idleFrameCount() const override;
    int spriteWidth() const override;
    int spriteHeight() const override;

private:
    bool exploding;
    bool explosionDone;
    int  explosionStartTick;
    int  explosionFrame;
    int  explosionAnimTick;
};

#endif
