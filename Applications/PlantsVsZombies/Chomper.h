#ifndef CHOMPER_H
#define CHOMPER_H

#include <Applications/PlantsVsZombies/Plant.h>

enum ChomperState { CHOMP_IDLE, CHOMP_ATTACKING, CHOMP_CHEWING };

class Chomper : public Plant {
public:
    static const int COST       = 150;
    static const int CHEW_TIME  = 15000; // ~15s to digest

    Chomper(int x, int y);

    void update() override;
    void render() override;
    PlantType getPlantType() const override;

    ChomperState getChomperState() const;
    void startAttack();
    bool isChewing() const;

protected:
    const unsigned char* idleFrame(int f) const override;
    int idleFrameCount() const override;
    int spriteWidth() const override;
    int spriteHeight() const override;

private:
    static const int ATTACK_ANIM_SPEED = 6;
    static const int CHEW_ANIM_SPEED   = 15;

    ChomperState chomperState;
    int attackFrame;
    int attackAnimTick;
    int chewFrame;
    int chewAnimTick;
    int chewEndTick;
};

#endif
