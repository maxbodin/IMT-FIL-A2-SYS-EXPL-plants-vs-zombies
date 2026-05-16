#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <Applications/PlantsVsZombies/Entity.h>

class Zombie : public Entity {
public:
    static const int HP        = 150;
    static const int HIT_DELAY = 150; // ticks entre chaque coup
    static const int BASE_ANIM_SPEED = 10; // base ticks between frames
    static const int MIN_ANIM_SPEED  = 4;  // fastest allowed

    Zombie(int x, int y, int speedBonus = 0);
    Zombie(int x, int y, int customHp, int speedBonus);

    void update() override;
    void render() override;

    bool canHit() const;
    void resetCooldown();

    int getWidth()  const override;
    int getHeight() const override;

    void block();
    void unblock();
    bool isBlocked() const;

    void applySlow(int duration);
    bool isSlowed() const;

    void applyFire(int damage, int duration);
    bool isOnFire() const;

    virtual bool hasPendingSummon() const;
    virtual void consumeSummon();
    virtual bool canBeBlocked() const;

protected:
    int animSpeed;
    int cooldown;
    int frame;
    int animTick;
    int slowTicks;

    /* Fire effect */
    int fireDamage;
    int fireEndTick;
    int fireNextDmgTick;
    int fireFrame;
    int fireAnimTick;

    /* Sprite data accessors — override in subclasses for different sprites */
    virtual const unsigned char* walkFrame(int f) const;
    virtual const unsigned char* fightFrame(int f) const;
    virtual int walkFrameCount() const;
    virtual int fightFrameCount() const;

    /* Hook called at the start of update(), before animation */
    virtual void onUpdate();
};

#endif