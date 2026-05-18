#ifndef WALLNUT_H
#define WALLNUT_H

#include <Applications/PlantsVsZombies/Plant.h>

class WallNut : public Plant {
public:
    static const int COST       = 50;
    static const int WALLNUT_HP = 800;

    WallNut(int x, int y);

    void render() override;
    PlantType getPlantType() const override;

protected:
    const unsigned char* idleFrame(int f) const override;
    int idleFrameCount() const override;
    int spriteWidth() const override;
    int spriteHeight() const override;
};

#endif
