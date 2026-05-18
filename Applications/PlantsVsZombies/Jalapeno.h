#ifndef JALAPENO_H
#define JALAPENO_H

#include <Applications/PlantsVsZombies/Plant.h>

class Jalapeno : public Plant {
public:
    static const int COST             = 125;
    static const int HP               = 100;
    static const int FIRE_DURATION    = 600;   // ticks the fire animation plays
    static const int ATTACK_DURATION  = 200;   // ticks the attacking animation plays before fire
    static const int FIRE_DMG_MIN     = 10;
    static const int FIRE_DMG_MAX     = 40;
    static const int FIRE_TICK_MIN    = 50;    // min ticks between fire damage ticks
    static const int FIRE_TICK_MAX    = 150;   // max ticks between fire damage ticks
    static const int FIRE_EFFECT_MIN  = 800;   // min fire effect duration on zombie
    static const int FIRE_EFFECT_MAX  = 2000;  // max fire effect duration on zombie
    static const int FIRE_ANIM_SPEED  = 8;

    Jalapeno(int x, int y);

    void update() override;
    void render() override;
    PlantType getPlantType() const override;

    /* Called when a zombie steps on the jalapeno. */
    void ignite();
    bool isOnFire()   const { return onFire; }
    bool isFireDone() const { return fireDone; }

    /* Fire effect applied to zombie: random damage, random duration */
    int  getFireDamage();
    int  getFireEffectDuration();

protected:
    const unsigned char* idleFrame(int f) const override;
    int idleFrameCount() const override;
    int spriteWidth() const override;
    int spriteHeight() const override;

private:
    bool attacking;
    bool onFire;
    bool fireDone;
    int  attackStartTick;
    int  attackFrame;
    int  attackAnimTick;
    int  fireStartTick;
    int  fireFrame;
    int  fireAnimTick;
};

#endif
