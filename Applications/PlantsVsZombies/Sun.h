#ifndef SUN_H
#define SUN_H

#define MAX_SUNS 20
#define SUN_COLLECT_MIN 25
#define SUN_COLLECT_MAX 75
#define SUN_LIFETIME    15000  // ticks before sun disappears (15s)

class Sun {
public:
    Sun(int x, int y);

    void update();
    void render();

    bool isActive() const;
    void collect();

    int getX() const;
    int getY() const;
    int getTileCol() const;
    int getTileRow() const;
    int getValue() const;

private:
    static const int ANIM_SPEED       = 20;
    static const int ALPHA_COLOR_IDX  = 179;  // palette index for transparency
    int x, y;
    int tileCol, tileRow;
    int value;
    int spawnTick;
    int frame;
    int animTick;
    bool active;

    int randomValue();
};

#endif
