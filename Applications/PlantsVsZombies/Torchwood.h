#ifndef TORCHWOOD_H
#define TORCHWOOD_H

#include <Applications/PlantsVsZombies/Plant.h>

class Torchwood : public Plant {
public:
    static const int COST = 175;
    static const int HP   = 300;

    Torchwood(int x, int y);

    void update() override;
    void render() override;
    PlantType getPlantType() const override;
    void ignite();

protected:
    const unsigned char* idleFrame(int f) const override;
    int idleFrameCount() const override;
    int spriteWidth() const override;
    int spriteHeight() const override;

private:
    static const int FIRE_DURATION = 24;
    static const int FIRE_ANIM_SPEED = 4;

    int fireFrame;
    int fireAnimTick;
    int fireTicks;
};

#endif
