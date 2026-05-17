#include <Applications/PlantsVsZombies/KernelPult.h>
#include <Applications/PlantsVsZombies/sprites/plants/kernelpult/kernelpult_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/kernelpult/kernelpult_attacking_sprite.h>

extern volatile int compt;

KernelPult::KernelPult(int x, int y)
    : ShooterPlant(x, y, HP, SHOOT_DELAY) {}

BulletType KernelPult::getBulletType() const {
    // 25% chance to shoot butter instead of kernel
    return (compt % 4 == 0) ? BULLET_BUTTER : BULLET_KERNEL;
}
PlantType KernelPult::getPlantType() const { return PLANT_KERNELPULT; }

const unsigned char* KernelPult::idleFrame(int f) const { return kernelpult_frames[f]; }
int KernelPult::idleFrameCount() const { return KERNELPULT_FRAMES; }
int KernelPult::spriteWidth() const { return KERNELPULT_WIDTH; }
int KernelPult::spriteHeight() const { return KERNELPULT_HEIGHT; }

const unsigned char* KernelPult::shootingSprite(int f) const { return kernelpult_attacking_frames[f]; }
int KernelPult::shootingSpriteCount() const { return KERNELPULT_ATTACKING_FRAMES; }
int KernelPult::shootingSpriteWidth() const { return KERNELPULT_ATTACKING_WIDTH; }
int KernelPult::shootingSpriteHeight() const { return KERNELPULT_ATTACKING_HEIGHT; }
