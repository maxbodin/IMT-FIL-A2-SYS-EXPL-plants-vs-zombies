#ifndef BLOVER_H
#define BLOVER_H

#include <Applications/PlantsVsZombies/Plant.h>

class Blover : public Plant {
public:
    static const int COST          = 100;
    static const int HP            = 300;
    static const int PUSH_INTERVAL = 120;  // ticks between pushes
    static const int PUSH_DISTANCE = 10;    // pixels pushed per event
    static const int PUSH_DAMAGE   = 10;    // small damage per push

    Blover(int x, int y);

    void update() override;
    void render() override;
    PlantType getPlantType() const override;

    bool isBlowing() const;
    bool shouldPush() const;
    void resetPushCooldown();
    int getLane() const;

protected:
    const unsigned char* idleFrame(int f) const override;
    int idleFrameCount() const override;
    int spriteWidth() const override;
    int spriteHeight() const override;

private:
    int blowFrame;
    int blowAnimTick;
    int pushCooldown;
};

#endif
