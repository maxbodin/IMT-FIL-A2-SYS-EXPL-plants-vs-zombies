#ifndef WAVE_MANAGER_H
#define WAVE_MANAGER_H

#include <Applications/PlantsVsZombies/Zombie.h>
#include <Applications/PlantsVsZombies/ConeZombie.h>
#include <Applications/PlantsVsZombies/BucketheadZombie.h>
#include <Applications/PlantsVsZombies/DiscoZombie.h>
#include <Applications/PlantsVsZombies/PogoZombie.h>
#include <Applications/PlantsVsZombies/FootballZombie.h>
#include <Applications/PlantsVsZombies/BackupDancerZombie.h>
#include <Applications/PlantsVsZombies/BalloonZombie.h>
#include <Applications/PlantsVsZombies/BaseballZombie.h>
#include <Applications/PlantsVsZombies/CatapultZombie.h>
#include <Applications/PlantsVsZombies/DiggerZombie.h>
#include <Applications/PlantsVsZombies/GargantuarZombie.h>
#include <Applications/PlantsVsZombies/JackInTheBoxZombie.h>
#include <Applications/PlantsVsZombies/JalapenoZombie.h>
#include <Applications/PlantsVsZombies/NewspaperZombie.h>
#include <Applications/PlantsVsZombies/PoleVaultingZombie.h>
#include <Applications/PlantsVsZombies/ScreenDoorZombie.h>
#include <Applications/PlantsVsZombies/YetiZombie.h>
#include <Applications/PlantsVsZombies/Grid.h>

#define BASE_ZOMBIES         2
#define MAX_ZOMBIES         100   // zombies at MAX_WAVE
#define MAX_WAVE             20   // waves loop after this
#define WAVE_PAUSE       12000   // ticks between waves (12 s)
#define SPAWN_INTERVAL    3000   // ticks between zombie spawns within a wave

/* Wave start text display durations (ticks). */
#define START_TEXT_DURATION 800   // each text shown for this long
#define START_TEXT_TOTAL   3200   // 4 texts × 800 ticks (Wave N + Ready + Set + Plant!)

class WaveManager {
public:
    WaveManager();

    /* Call every game tick. Returns a newly spawned Zombie,
       or 0 if no spawn this tick. Caller owns the pointer. */
    Zombie* update();

    /* Must be called each tick so WaveManager knows when all zombies are dead. */
    void setZombieCount(int count) { currentZombieCount = count; }

    int  getWave()      const { return wave; }
    bool isInPause()    const { return pauseUntil > 0; }
    void reset();

    /* Wave start text: returns 0=none, 1=Wave N, 2=Ready, 3=Set, 4=Plant! */
    int  getStartTextPhase() const;
    void renderStartText();

private:
    int wave;
    int remaining;
    int nextSpawnTick;
    int pauseUntil;

    int zombiesForWave(int w) const;
    int randomLane();

    int startTextTick;       // when the start text sequence began
    int currentZombieCount;  // live zombie count (set by game loop)
    bool wavePendingClear;   // waiting for all zombies to die before next wave
};

#endif
