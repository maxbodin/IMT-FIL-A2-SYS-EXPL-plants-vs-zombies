#ifndef GARLIC_H
#define GARLIC_H

#include <Applications/PlantsVsZombies/Plant.h>

class Garlic : public Plant {
public:
    static const int COST      = 50;
    static const int GARLIC_HP = 400;

    Garlic(int x, int y);

    PlantType getPlantType() const override;

protected:
    const unsigned char* idleFrame(int f) const override;
    int idleFrameCount() const override;
    int spriteWidth() const override;
    int spriteHeight() const override;
};

#endif
