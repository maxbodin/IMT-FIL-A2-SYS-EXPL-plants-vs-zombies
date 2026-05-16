#ifndef BLOVER_H
#define BLOVER_H

#include <Applications/PlantsVsZombies/Plant.h>

class Blover : public Plant {
public:
    static const int COST        = 100;
    static const int HP          = 50;
    static const int BLOW_RANGE  = 60;
    static const int DURATION    = 400;
    static const int WIND_DAMAGE = 50;

    Blover(int x, int y);

    void update() override;
    void render() override;
    PlantType getPlantType() const override;

    bool isBlowing() const;
    int getBlowRange() const;

protected:
    const unsigned char* idleFrame(int f) const override;
    int idleFrameCount() const override;
    int spriteWidth() const override;
    int spriteHeight() const override;

private:
    bool blowing;
    int blowStartTick;
    int blowFrame;
    int blowAnimTick;
};

#endif
