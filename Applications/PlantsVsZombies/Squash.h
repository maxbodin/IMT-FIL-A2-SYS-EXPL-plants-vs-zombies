#ifndef SQUASH_H
#define SQUASH_H

#include <Applications/PlantsVsZombies/Plant.h>

class Squash : public Plant {
public:
    static const int COST = 50;
    static const int HP   = 300;
    static const int SQUASH_DAMAGE = 1800;
    static const int JUMP_SPEED = 3;
    static const int ATTACK_ANIM_SPEED = 4;

    Squash(int x, int y);

    void update() override;
    void render() override;
    PlantType getPlantType() const override;

    bool isJumping() const { return jumping; }
    void setTarget(int tx, int ty);
    int getTargetX() const { return targetX; }
    int getTargetY() const { return targetY; }
    bool hasTarget() const { return jumping; }
    bool hasLanded() const { return landed; }

protected:
    const unsigned char* idleFrame(int f) const override;
    int idleFrameCount() const override;
    int spriteWidth() const override;
    int spriteHeight() const override;

private:
    bool jumping;
    bool landed;
    int targetX;
    int targetY;
    int attackFrame;
    int attackAnimTick;
    int landedTicks;
};

#endif
