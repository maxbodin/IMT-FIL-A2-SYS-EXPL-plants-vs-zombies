#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <Applications/PlantsVsZombies/Entity.h>

class Zombie : public Entity {
public:
    static const int HP        = 150;
    static const int HIT_DELAY = 150; // ticks entre chaque coup
    static const int BASE_ANIM_SPEED = 10; // base ticks between frames
    static const int MIN_ANIM_SPEED  = 4;  // fastest allowed
    static const int WALK_STEP       = 5;  // pixels moved per walk frame
    static const int FIRE_TICK_INTERVAL = 100; // ticks between fire damage
    static const int FIRE_ANIM_SPEED = 8;  // ticks between fire sprite frames
    static const int DEATH_ANIM_SPEED = 12; // ticks between death frames

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
    virtual bool isFlying() const;

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

    /* Death animation */
    int deathFrame;
    int deathAnimTick;

    /* Sprite data accessors — override in subclasses for different sprites */
    virtual const unsigned char* currentWalkFrame(int f) const;
    virtual const unsigned char* currentFightFrame(int f) const;
    virtual int currentWalkFrameCount() const;
    virtual int currentFightFrameCount() const;
    virtual int currentWalkWidth() const;
    virtual int currentWalkHeight() const;
    virtual int currentFightWidth() const;
    virtual int currentFightHeight() const;

    /* Death animation — override for zombie-specific death sprites */
    virtual const unsigned char* currentDeathFrame(int f) const;
    virtual int currentDeathFrameCount() const;
    virtual int currentDeathWidth() const;
    virtual int currentDeathHeight() const;
    virtual bool hasDeathAnimation() const;

    /* Hook called at the start of update(), before animation */
    virtual void onUpdate();
};

#endif