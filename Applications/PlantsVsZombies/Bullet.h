#ifndef BULLET_H
#define BULLET_H

class Zombie;

enum BulletType { BULLET_PEASHOOTER, BULLET_FIRE_PEA, BULLET_SNOW_PEA, BULLET_CABBAGE, BULLET_THORN, BULLET_KERNEL, BULLET_BUTTER, BULLET_COB, BULLET_MELON, BULLET_WINTER_MELON };

class Bullet {
public:
    Bullet();

    void init(int x, int y, BulletType type);
    void update();
    void render();
    void onHit(Zombie& target);

    bool isActive() const;
    bool isImpacting() const;
    void deactivate();
    void startImpact();

    int getX() const;
    int getY() const;
    int getSpawnX() const;
    int getWidth()  const;
    int getHeight() const;
    int getDamage() const;
    BulletType getType() const;
    int getDirection() const;
    void setDirection(int dir);
    void doubleDamage();
    void igniteByTorchwood();

private:
    static const int SPEED             = 4;
    static const int SLOW_DURATION     = 300;
    static const int IMPACT_ANIM_SPEED = 8;
    static const int IMPACT_HOLD       = 16;

    int x, y;
    int spawnX;
    bool active;
    BulletType type;
    int damage;
    bool impacting;
    int impactFrame;
    int impactAnimTick;
    int impactHoldTicks;
    int projectileFrame;
    int projectileAnimTick;
    int direction;
    bool torchUpgraded;
};

#endif
