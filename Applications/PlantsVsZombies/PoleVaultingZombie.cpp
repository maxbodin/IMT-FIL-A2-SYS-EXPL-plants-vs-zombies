#include <Applications/PlantsVsZombies/PoleVaultingZombie.h>
#include <Applications/PlantsVsZombies/sprites/zombies/polevaulting_zombie_walk_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/polevaulting_zombie_vault_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/polevaulting_zombie_fight_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/polevaulting_zombie_death_sprite.h>

PoleVaultingZombie::PoleVaultingZombie(int x, int y, int speedBonus)
    : Zombie(x, y, VAULT_HP, speedBonus + SPEED_BONUS),
      hasVaulted(false), vaulting(false), vaultFrame(0), vaultAnimTick(0) {}

bool PoleVaultingZombie::canBeBlocked() const {
    // Can't be blocked while still has pole (will vault over)
    return hasVaulted;
}

void PoleVaultingZombie::onUpdate() {
    if (vaulting) {
        ++vaultAnimTick;
        if (vaultAnimTick >= VAULT_ANIM_SPEED) {
            vaultAnimTick = 0;
            ++vaultFrame;
            if (vaultFrame >= POLEVAULTING_ZOMBIE_VAULT_FRAMES) {
                vaulting = false;
                hasVaulted = true;
                // Jump over the plant: move left past it
                x -= 40;
                if (x < 0) x = 0;
            }
        }
    }
}

int PoleVaultingZombie::getWidth() const {
    if (vaulting) return POLEVAULTING_ZOMBIE_VAULT_WIDTH;
    return POLEVAULTING_ZOMBIE_WALK_WIDTH;
}

int PoleVaultingZombie::getHeight() const {
    if (vaulting) return POLEVAULTING_ZOMBIE_VAULT_HEIGHT;
    return POLEVAULTING_ZOMBIE_WALK_HEIGHT;
}

const unsigned char* PoleVaultingZombie::currentWalkFrame(int f) const {
    if (vaulting)
        return polevaulting_zombie_vault_frames[vaultFrame % POLEVAULTING_ZOMBIE_VAULT_FRAMES];
    return polevaulting_zombie_walk_frames[f];
}

const unsigned char* PoleVaultingZombie::currentFightFrame(int f) const {
    return polevaulting_zombie_fight_frames[f];
}

int PoleVaultingZombie::currentWalkFrameCount() const {
    if (vaulting) return POLEVAULTING_ZOMBIE_VAULT_FRAMES;
    return POLEVAULTING_ZOMBIE_WALK_FRAMES;
}

int PoleVaultingZombie::currentFightFrameCount() const { return POLEVAULTING_ZOMBIE_FIGHT_FRAMES; }

int PoleVaultingZombie::currentWalkWidth() const {
    if (vaulting) return POLEVAULTING_ZOMBIE_VAULT_WIDTH;
    return POLEVAULTING_ZOMBIE_WALK_WIDTH;
}

int PoleVaultingZombie::currentWalkHeight() const {
    if (vaulting) return POLEVAULTING_ZOMBIE_VAULT_HEIGHT;
    return POLEVAULTING_ZOMBIE_WALK_HEIGHT;
}

int PoleVaultingZombie::currentFightWidth() const { return POLEVAULTING_ZOMBIE_FIGHT_WIDTH; }
int PoleVaultingZombie::currentFightHeight() const { return POLEVAULTING_ZOMBIE_FIGHT_HEIGHT; }

const unsigned char* PoleVaultingZombie::currentDeathFrame(int f) const { return polevaulting_zombie_death_frames[f]; }
int PoleVaultingZombie::currentDeathFrameCount() const { return POLEVAULTING_ZOMBIE_DEATH_FRAMES; }
int PoleVaultingZombie::currentDeathWidth() const { return POLEVAULTING_ZOMBIE_DEATH_WIDTH; }
int PoleVaultingZombie::currentDeathHeight() const { return POLEVAULTING_ZOMBIE_DEATH_HEIGHT; }
bool PoleVaultingZombie::hasDeathAnimation() const { return true; }
