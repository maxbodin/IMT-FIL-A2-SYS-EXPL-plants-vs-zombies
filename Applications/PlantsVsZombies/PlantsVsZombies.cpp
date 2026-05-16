#include <Applications/PlantsVsZombies/PlantsVsZombies.h>
#include <Applications/PlantsVsZombies/SnowPeashooter.h>
#include <Applications/PlantsVsZombies/Sunflower.h>
#include <Applications/PlantsVsZombies/Jalapeno.h>
#include <Applications/PlantsVsZombies/PotatoMine.h>
#include <Applications/PlantsVsZombies/WallNut.h>
#include <Applications/PlantsVsZombies/Chomper.h>
#include <Applications/PlantsVsZombies/GatlingPea.h>
#include <Applications/PlantsVsZombies/Blover.h>
#include <Applications/PlantsVsZombies/CabbagePult.h>
#include <Applications/PlantsVsZombies/CactusPlant.h>
#include <Applications/PlantsVsZombies/CherryBomb.h>
#include <Applications/PlantsVsZombies/CobCannon.h>
#include <Applications/PlantsVsZombies/Garlic.h>
#include <Applications/PlantsVsZombies/KernelPult.h>
#include <Applications/PlantsVsZombies/Marigold.h>
#include <Applications/PlantsVsZombies/MelonPult.h>
#include <Applications/PlantsVsZombies/Repeater.h>
#include <Applications/PlantsVsZombies/SplitPea.h>
#include <Applications/PlantsVsZombies/Squash.h>
#include <Applications/PlantsVsZombies/Threepeater.h>
#include <Applications/PlantsVsZombies/Torchwood.h>
#include <Applications/PlantsVsZombies/TwinSunflower.h>
#include <Applications/PlantsVsZombies/WinterMelon.h>
#include <Applications/PlantsVsZombies/sprites/shared_palette.h>
#include <Applications/PlantsVsZombies/sprites/plants/peashooter_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/snow_peashooter_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/sunflower_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/jalapeno_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/potato_mine_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/wallnut_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/chomper_idle_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/gatlingpea_idle_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/blover_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/cabbagepult_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/cactus_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/cherrybomb_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/cobcannon_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/garlic_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/kernelpult_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/marigold_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/melonpult_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/reapeater_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/splitpea_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/squash_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/threepeater_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/torchwood_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/twinsunflower_sprite.h>
#include <Applications/PlantsVsZombies/sprites/plants/wintermelon_sprite.h>
#include <Applications/PlantsVsZombies/sprites/objects/sun_small_sprite.h>
#include <Applications/PlantsVsZombies/sprites/ui/background_sprite.h>
#include <Applications/PlantsVsZombies/sprites/ui/game_over_sprite.h>
#include <Applications/PlantsVsZombies/sprites/zombies/zombie_walk_sprite.h>
#include <sextant/interruptions/handler/handler_clavier.h>
#include <sextant/memoire/memoire.h>
#include <sextant/threads/Threads.h>
#include <sextant/ordonnancements/preemptif/thread.h>
#include <vga/vga.h>

/* =========================================================================
 * Threads de jeu — Round-Robin coopératif
 *
 * CHOIX DE L'ALGORITHME : Round-Robin coopératif (Yield() explicite)
 *
 * L'alternative préemptive (sched_clk hooked sur le timer IRQ) est écartée
 * parce que irq_wrappers.S sauvegarde le contexte puis exécute le handler C
 * puis fait "iret". Si le handler appelle cpu_context_switch(), l'"iret"
 * suivant restaure le mauvais pile ← corruption silencieuse.
 *
 * Avec le mode coopératif :
 *   - Un seul thread tourne à la fois (mono-core).
 *   - Pas de race condition réelle entre threads : la mémoire partagée
 *     (positions des entités, suns, lives…) ne peut pas être modifiée
 *     pendant qu'un autre thread la lit — il faut qu'un thread cède
 *     explicitement le CPU pour que l'autre puisse démarrer.
 *   - Le sémaphore frameSem (initial=0) assure que le thread rendu ne tente
 *     de dessiner qu'une fois que le thread logique a terminé une mise à jour.
 *
 * Flux d'exécution (une itération) :
 *   LogicThread : handleInput() + updateLogic() → frameSem.V() → Yield()
 *   RenderThread: tryP(frameSem) → renderFrame() → Yield()
 *   (initial thread) : Yield() en boucle — thread idle
 * ========================================================================= */

/* PvZLogicThread : met à jour l'état du jeu toutes les 16 ms (≈ 62 fps),
   puis signale le thread rendu via frameSem. */
struct PvZLogicThread : public Threads {
    PlantsVsZombies* game;
    void run() override { game->runLogic(); }
};

/* PvZRenderThread : attend le signal du thread logique, dessine la frame. */
struct PvZRenderThread : public Threads {
    PlantsVsZombies* game;
    void run() override { game->runRender(); }
};

extern volatile int compt;

bool PlantsVsZombies::canAfford(int cost) const { return suns >= cost; }

void PlantsVsZombies::spendSuns(int cost) { if (suns >= cost) suns -= cost; }

void PlantsVsZombies::addSuns(int amount) { suns += amount; }

// Palette indices (shared_palette) : 1=gris foncé, 3=gris clair, 15=blanc, 25=jaune, 205=vert
static const unsigned char SUN_HUD_BG     =   1;
static const unsigned char SUN_HUD_TEXT   =  15;
static const unsigned char SUN_HUD_YELLOW =  25;
static const unsigned char SUN_HUD_GREEN  = 205;
static const unsigned char SUN_HUD_RED    =  32;

// Dessine un "+" 3×3 pixels (à l'échelle scale)
static void draw_plus(int x, int y, unsigned char color, int scale) {
    plot_square(x + scale, y,           scale, color); // haut
    plot_square(x,         y + scale,   scale, color); // gauche
    plot_square(x + scale, y + scale,   scale, color); // centre
    plot_square(x + scale*2, y + scale, scale, color); // droite
    plot_square(x + scale, y + scale*2, scale, color); // bas
}

void PlantsVsZombies::drawSunHud() {
    // Panneau centré : x=120, y=1, 80×20 px
    const int px = 120, py = 1, pw = 80, ph = 20;
    for (int row = 0; row < ph; row++)
        for (int col = 0; col < pw; col++)
            video[(py + row) * SCREEN_WIDTH + (px + col)] = SUN_HUD_BG;

    bool sunFlash = compt < sunFlashEndTick;
    unsigned char sunColor    = sunFlash ? SUN_HUD_RED : SUN_HUD_TEXT;
    unsigned char borderColor = sunFlash ? SUN_HUD_RED : (unsigned char)3;

    for (int col = 0; col < pw; col++) {
        video[py * SCREEN_WIDTH + (px + col)]            = borderColor;
        video[(py + ph - 1) * SCREEN_WIDTH + (px + col)] = borderColor;
    }

    // Icône soleil animée (sun_small, 2 frames)
    {
        static int sunHudFrame = 0;
        static int sunHudAnimTick = 0;
        if (++sunHudAnimTick >= 30) {
            sunHudAnimTick = 0;
            sunHudFrame = (sunHudFrame + 1) % SUN_SMALL_FRAMES;
        }
        draw_sprite_scaled(sun_small_frames[sunHudFrame],
                           SUN_SMALL_WIDTH, SUN_SMALL_HEIGHT,
                           px + 2, py + 2, 16, 16);
    }

    draw_number(suns, px + 22, py + 6, sunColor, 2);

    // Affichage "+Y" pendant SUN_DISPLAY_DURATION ticks après un gain automatique
    if (compt < sunGainDisplayEnd) {
        draw_plus(px + 52, py + 6, SUN_HUD_GREEN, 2);
        draw_number(SUN_TICK_AMOUNT, px + 60, py + 6, SUN_HUD_GREEN, 2);
    }

    // Affichage "+X" après collecte de soleil
    if (compt < sunCollectDisplayEnd) {
        draw_plus(px + 52, py + 6, SUN_HUD_YELLOW, 2);
        draw_number(lastSunCollected, px + 60, py + 6, SUN_HUD_YELLOW, 2);
    }
}

/* Couleurs des curseurs (indices dans shared_palette).
   P1 = blanc (index 15), P2 = vert (index 205, même teinte que la barre de vie). */
#define CURSOR_P1_COLOR 15
#define CURSOR_P2_COLOR 205

PlantsVsZombies::PlantsVsZombies() : plantCount(0), zombieCount(0) {
    for (int i = 0; i < MAX_PLANTS; i++)
        plants[i] = 0;
    for (int i = 0; i < MAX_ZOMBIES; i++)
        zombies[i] = 0;
    for (int i = 0; i < MAX_SUNS; i++)
        suns_on_ground[i] = 0;
}

void PlantsVsZombies::init(Ecran* e, Clavier* c) {
    ecran   = e;
    clavier = c;
    backbuffer = (unsigned char*) getmem(SCREEN_WIDTH * SCREEN_HEIGHT);
    set_vga_mode13();
    set_palette_vga(shared_palette);
    clear_vga_screen(0);
}

static bool aabb(int ax, int ay, int aw, int ah,
                 int bx, int by, int bw, int bh) {
    return ax < bx + bw && ax + aw > bx
        && ay < by + bh && ay + ah > by;
}

/* [LOGIQUE] Met à jour l'état du jeu : vagues, plantes, zombies, balles, soleils.
   Appelé par runLogic() toutes les 16 ms. Ne touche pas au framebuffer. */
void PlantsVsZombies::updateLogic() {

    // --- Wave spawning (always runs to track wave state) ---
    waveManager.setZombieCount(zombieCount);
    if (zombieCount < MAX_ZOMBIES) {
        Zombie* z = waveManager.update();
        if (z) zombies[zombieCount++] = z;
    }

    // --- Pause all game logic while start text is displayed ---
    bool gamePaused = (waveManager.getStartTextPhase() != 0);

    if (!gamePaused) {

    // --- Gain automatique de soleils ---
    if (compt - lastSunTick >= SUN_TICK_INTERVAL) {
        addSuns(SUN_TICK_AMOUNT);
        lastSunTick       = compt;
        sunGainDisplayEnd = compt + SUN_DISPLAY_DURATION;
    }

    // --- Cactus: detect zombies in same row to trigger growing ---
    for (int p = 0; p < plantCount; p++) {
        if (plants[p]->getPlantType() != PLANT_CACTUS) continue;
        CactusPlant* cactus = (CactusPlant*)plants[p];
        int pCol, pRow;
        if (!grid.pixelToTile(plants[p]->getX(), plants[p]->getY(), pCol, pRow))
            continue;
        for (int z = 0; z < zombieCount; z++) {
            int zFootY = zombies[z]->getY() + zombies[z]->getHeight();
            int zCol, zRow;
            if (!grid.pixelToTile(zombies[z]->getX(), zFootY - 1, zCol, zRow))
                continue;
            if (zRow == pRow) {
                cactus->setZombieInRange(true);
                break;
            }
        }
    }

    // --- Plants: update + spawn bullets/suns + remove dead ---
    for (int i = 0; i < plantCount; i++) {
        plants[i]->update();
        if (plants[i]->isDead()) {
            int tc, tr;
            if (grid.pixelToTile(plants[i]->getX(), plants[i]->getY(), tc, tr)) {
                Tile* t = grid.getTile(tc, tr);
                if (t) t->setState(TileState::Empty);
            }
            delete plants[i];
            plants[i] = plants[--plantCount];
            plants[plantCount] = 0;
            i--;
            continue;
        }

        if (plants[i]->hasSunReady() && sunOnGroundCount < MAX_SUNS) {
            suns_on_ground[sunOnGroundCount++] = new Sun(plants[i]->getX(), plants[i]->getY() + plants[i]->getHeight() / 2);
            if (plants[i]->getPlantType() == PLANT_TWINSUNFLOWER && sunOnGroundCount < MAX_SUNS) {
                suns_on_ground[sunOnGroundCount++] = new Sun(plants[i]->getX() + 10, plants[i]->getY() + plants[i]->getHeight() / 2 + 5);
            }
            plants[i]->resetSunTimer();
        }

        if (plants[i]->canShoot()) {
            int bx = plants[i]->getX();
            int by = plants[i]->getY() - plants[i]->getHeight() / 4;

            if (plants[i]->getPlantType() == PLANT_THREEPEATER) {
                int pc, pr;
                if (grid.pixelToTile(plants[i]->getX(), plants[i]->getY(), pc, pr)) {
                    for (int lane = -1; lane <= 1; lane++) {
                        int row = pr + lane;
                        if (row < 0 || row >= Grid::ROWS) continue;
                        int laneY;
                        int dummyX;
                        grid.tileToPixel(pc, row, dummyX, laneY);
                        laneY -= Grid::TILE_SIZE / 4;
                        Bullet* b = bulletPool.acquire();
                        if (b) {
                            b->init(bx, laneY, plants[i]->getBulletType());
                        }
                    }
                    plants[i]->resetCooldown();
                }
            } else if (plants[i]->getPlantType() == PLANT_SPLITPEA) {
                Bullet* b = bulletPool.acquire();
                if (b) {
                    b->init(bx, by, plants[i]->getBulletType());
                }
                Bullet* b2 = bulletPool.acquire();
                if (b2) {
                    b2->init(bx, by, plants[i]->getBulletType());
                    b2->setDirection(-1);
                }
                plants[i]->resetCooldown();
            } else {
                Bullet* b = bulletPool.acquire();
                if (b) {
                    b->init(bx, by, plants[i]->getBulletType());
                    plants[i]->resetCooldown();
                }
            }
        }
    }

    // --- Potato mines: tile-based detection (separate from general collision) ---
    for (int p = 0; p < plantCount; p++) {
        if (plants[p]->getPlantType() != PLANT_POTATO_MINE) continue;
        PotatoMine* pm = (PotatoMine*)plants[p];
        if (pm->isExploding() || !pm->isArmed()) continue;

        int mineTileCol, mineTileRow;
        if (!grid.pixelToTile(plants[p]->getX(), plants[p]->getY(), mineTileCol, mineTileRow))
            continue;

        for (int i = 0; i < zombieCount; i++) {
            if (!zombies[i]->canBeBlocked()) continue; // pogo zombies jump over mines
            int zCenterX = zombies[i]->getX() + zombies[i]->getWidth() / 2;
            int zFootY   = zombies[i]->getY() + zombies[i]->getHeight();
            int zcol, zrow;
            if (!grid.pixelToTile(zCenterX, zFootY - 1, zcol, zrow)) continue;
            if (zcol == mineTileCol && zrow == mineTileRow) {
                pm->explode();
                /* Damage all zombies on explosion tile and adjacent tiles */
                for (int z2 = 0; z2 < zombieCount; z2++) {
                    if (!zombies[z2]->canBeBlocked()) continue; // pogo zombies immune
                    int z2cx = zombies[z2]->getX() + zombies[z2]->getWidth() / 2;
                    int z2fy = zombies[z2]->getY() + zombies[z2]->getHeight();
                    int z2col, z2row;
                    if (!grid.pixelToTile(z2cx, z2fy - 1, z2col, z2row)) continue;
                    int dc = z2col - mineTileCol;
                    int dr = z2row - mineTileRow;
                    if (dc < 0) dc = -dc;
                    if (dr < 0) dr = -dr;
                    if (dc <= 1 && dr <= 1) {
                        zombies[z2]->takeDamage(pm->getExplosionDamage());
                        DmgIndicator* di = dmgPool.acquire();
                        if (di) di->init(zombies[z2]->getX(),
                                         zombies[z2]->getY() - DMG_INDICATOR_Y_OFFSET,
                                         pm->getExplosionDamage(), DMG_INDICATOR_DURATION);
                    }
                }
                break; // mine already triggered
            }
        }
    }

    // --- Cherry bombs: tile-based AoE explosion ---
    for (int p = 0; p < plantCount; p++) {
        if (plants[p]->getPlantType() != PLANT_CHERRYBOMB) continue;
        CherryBomb* cb = (CherryBomb*)plants[p];
        if (cb->isExploding() || cb->isAttacking()) continue;

        int cbCol, cbRow;
        if (!grid.pixelToTile(plants[p]->getX(), plants[p]->getY(), cbCol, cbRow))
            continue;

        for (int i = 0; i < zombieCount; i++) {
            int zCenterX = zombies[i]->getX() + zombies[i]->getWidth() / 2;
            int zFootY   = zombies[i]->getY() + zombies[i]->getHeight();
            int zcol, zrow;
            if (!grid.pixelToTile(zCenterX, zFootY - 1, zcol, zrow)) continue;
            int dc = zcol - cbCol; if (dc < 0) dc = -dc;
            int dr = zrow - cbRow; if (dr < 0) dr = -dr;
            if (dc <= 1 && dr <= 1) {
                cb->explode();
                // Damage all zombies in 3x3 area
                for (int z2 = 0; z2 < zombieCount; z2++) {
                    int z2cx = zombies[z2]->getX() + zombies[z2]->getWidth() / 2;
                    int z2fy = zombies[z2]->getY() + zombies[z2]->getHeight();
                    int z2col, z2row;
                    if (!grid.pixelToTile(z2cx, z2fy - 1, z2col, z2row)) continue;
                    int d2c = z2col - cbCol; if (d2c < 0) d2c = -d2c;
                    int d2r = z2row - cbRow; if (d2r < 0) d2r = -d2r;
                    if (d2c <= 1 && d2r <= 1) {
                        zombies[z2]->takeDamage(cb->getExplosionDamage());
                        DmgIndicator* di = dmgPool.acquire();
                        if (di) di->init(zombies[z2]->getX(),
                                         zombies[z2]->getY() - DMG_INDICATOR_Y_OFFSET,
                                         cb->getExplosionDamage(), DMG_INDICATOR_DURATION);
                    }
                }
                break;
            }
        }
    }

    // --- Squash: jump on nearest zombie in same lane ---
    for (int p = 0; p < plantCount; p++) {
        if (plants[p]->getPlantType() != PLANT_SQUASH) continue;
        Squash* sq = (Squash*)plants[p];
        if (sq->isJumping() || sq->hasLanded()) {
            if (sq->hasLanded()) {
                for (int z = 0; z < zombieCount; z++) {
                    int dx = zombies[z]->getX() - sq->getX();
                    int dy = zombies[z]->getY() - sq->getY();
                    if (dx < 0) dx = -dx;
                    if (dy < 0) dy = -dy;
                    if (dx < Grid::TILE_SIZE && dy < Grid::TILE_SIZE) {
                        zombies[z]->takeDamage(Squash::SQUASH_DAMAGE);
                        DmgIndicator* di = dmgPool.acquire();
                        if (di) di->init(zombies[z]->getX(),
                                         zombies[z]->getY() - DMG_INDICATOR_Y_OFFSET,
                                         Squash::SQUASH_DAMAGE, DMG_INDICATOR_DURATION);
                    }
                }
            }
            continue;
        }
        int sqCol, sqRow;
        if (!grid.pixelToTile(sq->getX(), sq->getY(), sqCol, sqRow)) continue;
        int bestDist = 99999;
        int bestZ = -1;
        for (int z = 0; z < zombieCount; z++) {
            int zCenterX = zombies[z]->getX() + zombies[z]->getWidth() / 2;
            int zFootY = zombies[z]->getY() + zombies[z]->getHeight();
            int zcol, zrow;
            if (!grid.pixelToTile(zCenterX, zFootY - 1, zcol, zrow)) continue;
            if (zrow != sqRow) continue;
            int dist = zCenterX - sq->getX();
            if (dist > 0 && dist < bestDist) {
                bestDist = dist;
                bestZ = z;
            }
        }
        if (bestZ >= 0 && bestDist < 5 * Grid::TILE_SIZE) {
            sq->setTarget(zombies[bestZ]->getX(), zombies[bestZ]->getY());
        }
    }

    // --- Torchwood: turn pea bullets passing through into fire peas ---
    for (int p = 0; p < plantCount; p++) {
        if (plants[p]->getPlantType() != PLANT_TORCHWOOD) continue;
        Torchwood* tw = (Torchwood*)plants[p];
        int twX = plants[p]->getX();
        int twY = plants[p]->getY();
        int twW = plants[p]->getWidth();
        int twH = plants[p]->getHeight();
        for (int bi = 0; bi < bulletPool.CAPACITY; bi++) {
            Bullet* b = bulletPool.get(bi);
            if (!b->isActive() || b->isImpacting()) continue;
            if (b->getDirection() < 0) continue;
            if (b->getType() != BULLET_PEASHOOTER) continue;
            int bx = b->getX();
            int by = b->getY();
            if (bx >= twX && bx <= twX + twW &&
                by + b->getHeight() >= twY && by <= twY + twH) {
                b->igniteByTorchwood();
                tw->ignite();
            }
        }
    }

    // --- Blover: blow away all zombies on the field ---
    for (int p = 0; p < plantCount; p++) {
        if (plants[p]->getPlantType() != PLANT_BLOVER) continue;
        Blover* bl = (Blover*)plants[p];
        if (!bl->isBlowing()) continue;

        for (int i = 0; i < zombieCount; i++) {
            // Push zombies to the right off-screen
            int zx = zombies[i]->getX();
            if (zx < SCREEN_WIDTH) {
                zombies[i]->takeDamage(Blover::WIND_DAMAGE); // wind damage per tick
            }
        }
    }

    // --- Garlic: redirect zombies to adjacent lanes on contact ---
    for (int p = 0; p < plantCount; p++) {
        if (plants[p]->getPlantType() != PLANT_GARLIC) continue;
        int gx = plants[p]->getX();
        int gy = plants[p]->getY();
        int gw = plants[p]->getWidth();

        for (int i = 0; i < zombieCount; i++) {
            if (!zombies[i]->canBeBlocked()) continue;
            int dx = zombies[i]->getX() - (gx + gw);
            int dy = zombies[i]->getY() - gy;
            if (dy < 0) dy = -dy;
            if (dx >= 0 && dx < COLLISION_DISTANCE && dy < Grid::TILE_SIZE) {
                // Move zombie to adjacent lane (alternate up/down)
                int zy = zombies[i]->getY();
                int lane = (zy + zombies[i]->getHeight() - Grid::OFFSET_Y) / Grid::TILE_SIZE;
                int newLane = (lane > 0) ? lane - 1 : lane + 1;
                if (newLane >= Grid::ROWS) newLane = Grid::ROWS - 1;
                int newY = Grid::OFFSET_Y + newLane * Grid::TILE_SIZE
                         + Grid::TILE_SIZE / 2 - zombies[i]->getHeight();
                zombies[i]->setPosition(zombies[i]->getX() + Garlic::PUSH_OFFSET, newY);
                plants[p]->takeDamage(Garlic::BITE_DAMAGE); // garlic takes bite damage
            }
        }
    }

    // --- Zombies: block/unblock + damage plant + special interactions + update + remove dead ---
    for (int i = 0; i < zombieCount; i++) {
        bool blocked = false;
        if (zombies[i]->canBeBlocked()) {
        for (int p = 0; p < plantCount; p++) {
            int dx = zombies[i]->getX() - (plants[p]->getX() + plants[p]->getWidth());
            int dy = zombies[i]->getY() - plants[p]->getY();
            if (dy < 0) dy = -dy;
            if (dx >= 0 && dx < COLLISION_DISTANCE && dy < Grid::TILE_SIZE) {
                blocked = true;

                PlantType pt = plants[p]->getPlantType();

                /* Jalapeno: ignite on contact, apply fire to zombie */
                if (pt == PLANT_JALAPENO) {
                    Jalapeno* jal = (Jalapeno*)plants[p];
                    if (!jal->isOnFire()) {
                        jal->ignite();
                        zombies[i]->applyFire(jal->getFireDamage(), jal->getFireEffectDuration());
                    }
                    /* Jalapeno doesn't block — zombie walks through the fire */
                    blocked = false;
                }

                /* PotatoMine: don't block (explosion handled in separate pass above) */
                if (pt == PLANT_POTATO_MINE) {
                    blocked = false;
                }

                /* Chomper: eat zombie on contact if idle */
                if (pt == PLANT_CHOMPER) {
                    Chomper* ch = (Chomper*)plants[p];
                    if (ch->getChomperState() == CHOMP_IDLE) {
                        ch->startAttack();
                        zombies[i]->takeDamage(Chomper::CHOMP_DAMAGE); // instant kill
                        DmgIndicator* di = dmgPool.acquire();
                        if (di) di->init(zombies[i]->getX(),
                                         zombies[i]->getY() - DMG_INDICATOR_Y_OFFSET,
                                         Chomper::CHOMP_DAMAGE, DMG_INDICATOR_DURATION);
                    }
                    /* Chomper doesn't block while chewing — vulnerable */
                    if (ch->isChewing()) blocked = false;
                }

                /* Normal plants (peashooter, sunflower, wallnut): zombie blocks and attacks */
                if (blocked && zombies[i]->canHit()) {
                    plants[p]->takeDamage(ZOMBIE_DAMAGE);
                    zombies[i]->resetCooldown();
                }
                break;
            }
        }
        } // canBeBlocked
        blocked ? zombies[i]->block() : zombies[i]->unblock();
        zombies[i]->update();

        if (zombies[i]->getX() <= 0) {
            lives--;
            delete zombies[i];
            zombies[i] = zombies[--zombieCount];
            zombies[zombieCount] = 0;
            i--;
            continue;
        }
        if (zombies[i]->isDead()) {
            delete zombies[i];
            zombies[i] = zombies[--zombieCount];
            zombies[zombieCount] = 0;
            i--;
        }
    }

    // --- Disco zombie summons: spawn backup dancers around summoner ---
    for (int i = 0; i < zombieCount; i++) {
        if (!zombies[i]->hasPendingSummon()) continue;
        zombies[i]->consumeSummon();
        int zx = zombies[i]->getX();
        int zy = zombies[i]->getY() + zombies[i]->getHeight();
        // Compute summoner lane from foot position
        int lane = (zy - Grid::OFFSET_Y) / Grid::TILE_SIZE;
        // Spawn backup dancers in adjacent lanes (up to SUMMON_COUNT)
        int offsets[] = { -1, 1, -2, 2 };
        for (int s = 0; s < DiscoZombie::SUMMON_COUNT && zombieCount < MAX_ZOMBIES; s++) {
            int targetLane = lane + offsets[s];
            if (targetLane < 0 || targetLane >= Grid::ROWS) continue;
            int ny = Grid::OFFSET_Y + targetLane * Grid::TILE_SIZE
                     + Grid::TILE_SIZE / 2 - ZOMBIE_WALK_HEIGHT;
            zombies[zombieCount++] = new Zombie(zx, ny);
        }
    }

    // --- Bullets: update + collision vs zombies (object pool, no new/delete) ---
    for (int i = 0; i < bulletPool.CAPACITY; i++) {
        Bullet* b = bulletPool.get(i);
        if (!b->isActive()) continue;

        b->update();
        if (!b->isActive()) {
            bulletPool.release(b);
            continue;
        }

        if (b->isImpacting()) continue;

        int travelDist = b->getX() - b->getSpawnX();
        if (travelDist < 0) travelDist = -travelDist;
        if (travelDist > COLLISION_DISTANCE) {
            for (int z = 0; z < zombieCount; z++) {
                if (aabb(b->getX(),  b->getY(),
                         b->getWidth(), b->getHeight(),
                         zombies[z]->getX(),  zombies[z]->getY(),
                         zombies[z]->getWidth(), zombies[z]->getHeight())) {
                    int dmg = b->getDamage();
                    b->onHit(*zombies[z]);
                    b->startImpact();
                    /* Damage indicator via pool */
                    DmgIndicator* di = dmgPool.acquire();
                    if (di) di->init(zombies[z]->getX(),
                                     zombies[z]->getY() - DMG_INDICATOR_Y_OFFSET,
                                     dmg, DMG_INDICATOR_DURATION);
                    break;
                }
            }
        }
    }

    // --- Suns on ground: update + cursor collection ---
    for (int i = 0; i < sunOnGroundCount; i++) {
        suns_on_ground[i]->update();
        if (!suns_on_ground[i]->isActive()) {
            delete suns_on_ground[i];
            suns_on_ground[i] = suns_on_ground[--sunOnGroundCount];
            suns_on_ground[sunOnGroundCount] = 0;
            i--;
            continue;
        }

        int sc = suns_on_ground[i]->getTileCol();
        int sr = suns_on_ground[i]->getTileRow();
        if ((cursorCol == sc && cursorRow == sr) ||
            (cursorCol2 == sc && cursorRow2 == sr)) {
            int val = suns_on_ground[i]->getValue();
            addSuns(val);
            lastSunCollected     = val;
            sunCollectDisplayEnd = compt + SUN_COLLECT_DISPLAY;
            delete suns_on_ground[i];
            suns_on_ground[i] = suns_on_ground[--sunOnGroundCount];
            suns_on_ground[sunOnGroundCount] = 0;
            i--;
        }
    }

    } // end if (!gamePaused)
}

/* [RENDU] Compose le backbuffer et le copie dans le framebuffer VGA.
   Appelé par runRender() après que runLogic() a signalé une nouvelle frame
   via frameSem. Aucune modification de l'état du jeu ici. */
void PlantsVsZombies::renderFrame() {
    unsigned char* real_video = (unsigned char*) video;
    video = backbuffer;

    if (gameOver) {
        // Clear to black
        for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
            backbuffer[i] = 0;

        // Draw game over sprite centered
        int imgX = (SCREEN_WIDTH - GAME_OVER_WIDTH) / 2;
        int imgY = 4;
        draw_sprite(game_over_sprite_data, GAME_OVER_WIDTH, GAME_OVER_HEIGHT, imgX, imgY);

        // Stats below the image
        int statsY = imgY + GAME_OVER_HEIGHT + 6;

        int len = 0, tmp = lastSeconds;
        if (tmp == 0) len = 1; else { while (tmp > 0) { len++; tmp /= 10; } }
        int sx = (SCREEN_WIDTH - (len + 1) * 8) / 2;
        draw_number(lastSeconds, sx, statsY, 15, 2);
        draw_text("s", sx + len * 8, statsY, 15, 2);

        draw_text("wave", 134, statsY + 14, 15, 2);
        draw_number(waveManager.getWave(), 134 + 4 * 8, statsY + 14, 15, 2);

        int restartX = (SCREEN_WIDTH - 22 * 4) / 2;
        draw_text("press space to restart", restartX, statsY + 30, 15, 1);
    } else {
        // Normal game rendering
        for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
            backbuffer[i] = background_sprite_data[i];

        grid.render();

    for (int i = 0; i < plantCount; i++)
        if (plants[i]) plants[i]->render();

    for (int i = 0; i < bulletPool.CAPACITY; i++) {
        Bullet* b = bulletPool.get(i);
        if (b->isActive()) b->render();
    }

    for (int i = 0; i < zombieCount; i++)
        if (zombies[i]) zombies[i]->render();

    for (int i = 0; i < sunOnGroundCount; i++)
        if (suns_on_ground[i]) suns_on_ground[i]->render();

    unsigned char c1 = grid.isTileOccupied(cursorCol, cursorRow)   ? SUN_HUD_RED : CURSOR_P1_COLOR;
    unsigned char c2 = grid.isTileOccupied(cursorCol2, cursorRow2) ? SUN_HUD_RED : CURSOR_P2_COLOR;
    drawCursor(cursorCol,  cursorRow,  c1);
    drawCursor(cursorCol2, cursorRow2, c2);

    // Timer (scale 1) with "s" unit
    {
        int len = 0, tmp = lastSeconds;
        if (tmp == 0) len = 1; else while (tmp > 0) { len++; tmp /= 10; }
        draw_number(lastSeconds, 1, 2, 15, 1);
        draw_text("s", 1 + len * 4, 2, 15, 1);
    }

    // FPS (scale 1) with "fps" unit, right-aligned
    {
        int len = 0, tmp = lastFps;
        if (tmp == 0) len = 1; else while (tmp > 0) { len++; tmp /= 10; }
        int totalW = (len + 3) * 4;
        int startX = SCREEN_WIDTH - 1 - totalW;
        draw_number(lastFps, startX, 2, 15, 1);
        draw_text("fps", startX + len * 4, 2, 15, 1);
    }

    // Wave indicator
    {
        draw_text("w", 1, 9, 15, 1);
        draw_number(waveManager.getWave(), 5, 9, 15, 1);
    }

    drawSunHud();
    drawLivesHud();

    for (int i = 0; i < dmgPool.CAPACITY; i++) {
        DmgIndicator* di = dmgPool.get(i);
        if (!di->isActive()) continue;
        di->update();
        if (di->isActive()) di->render();
    }

    queue1.update();
    queue2.update();
    drawQueueHud(queue1, 2,   186, CURSOR_P1_COLOR);
    drawQueueHud(queue2, 200, 186, CURSOR_P2_COLOR);

    /* Wave start text (Ready / Set / Plant!) */
    waveManager.renderStartText();

    } // end if (!gameOver) — normal rendering

    unsigned char* src = backbuffer;
    unsigned char* dst = real_video;
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) dst[i] = src[i];
    video = real_video;
}

// ---------------------------------------------------------------------------
// Lives HUD
// ---------------------------------------------------------------------------

void PlantsVsZombies::drawLivesHud() {
    // Pixel-art heart, 5×4 cells, scale 2 → 10×8 px per heart
    static const bool heart[4][5] = {
        { 0, 1, 0, 1, 0 },
        { 1, 1, 1, 1, 1 },
        { 0, 1, 1, 1, 0 },
        { 0, 0, 1, 0, 0 },
    };
    const int scale  = 2;
    const int heartW = 5 * scale;
    const int gap    = 3;
    const int totalW = 3 * heartW + 2 * gap;
    const int startX = SCREEN_WIDTH - totalW - 1;
    const int startY = 16;

    for (int i = 0; i < 3; i++) {
        unsigned char color = (i < lives) ? (unsigned char)32 : (unsigned char)1;
        int ox = startX + i * (heartW + gap);
        for (int row = 0; row < 4; row++)
            for (int col = 0; col < 5; col++)
                if (heart[row][col])
                    plot_square(ox + col * scale, startY + row * scale, scale, color);
    }
}

// ---------------------------------------------------------------------------
// Game Over
// ---------------------------------------------------------------------------

void PlantsVsZombies::showGameOver() {
    gameOver = true;

    // Drain any buffered key events
    KeyEvent evt;
    while (keyboardQueue.pop(evt)) {}

    // Keep signaling the render thread so it draws the game over screen,
    // while waiting for space press.
    while (true) {
        frameSem.V();
        thread_yield();
        while (keyboardQueue.pop(evt)) {
            if (evt.pressed && evt.scanCode == SC_P1_PLACE) {
                gameOver = false;
                resetGame();
                return;
            }
        }
    }
}

void PlantsVsZombies::resetGame() {
    // Delete all plants
    for (int i = 0; i < plantCount; i++) {
        if (plants[i]) {
            int tc, tr;
            if (grid.pixelToTile(plants[i]->getX(), plants[i]->getY(), tc, tr)) {
                Tile* t = grid.getTile(tc, tr);
                if (t) t->setState(TileState::Empty);
            }
            delete plants[i];
            plants[i] = 0;
        }
    }
    plantCount = 0;

    // Delete all zombies
    for (int i = 0; i < zombieCount; i++) {
        if (zombies[i]) { delete zombies[i]; zombies[i] = 0; }
    }
    zombieCount = 0;

    // Release all bullets and damage indicators
    for (int i = 0; i < bulletPool.CAPACITY; i++) {
        Bullet* b = bulletPool.get(i);
        if (b->isActive()) { b->deactivate(); bulletPool.release(b); }
    }
    for (int i = 0; i < dmgPool.CAPACITY; i++) {
        DmgIndicator* di = dmgPool.get(i);
        if (di->isActive()) di->deactivate();
    }

    // Delete all suns on ground
    for (int i = 0; i < sunOnGroundCount; i++) {
        if (suns_on_ground[i]) { delete suns_on_ground[i]; suns_on_ground[i] = 0; }
    }
    sunOnGroundCount = 0;

    // Reset game state
    suns                 = SUN_INITIAL;
    lastSunTick          = compt;
    sunGainDisplayEnd    = 0;
    sunFlashEndTick      = 0;
    sunCollectDisplayEnd = 0;
    lastSunCollected     = 0;
    lives                = 3;
    lastFps              = 0;
    lastSeconds          = 0;

    // Reset cursors
    cursorCol  = 0;
    cursorRow  = 0;
    cursorCol2 = Grid::COLS - 1;
    cursorRow2 = Grid::ROWS - 1;

    // Reset wave manager and queues
    waveManager.reset();
    queue1.reset();
    queue2.reset();
    queue1.seed(2);
    queue2.seed(2);
}

// ---------------------------------------------------------------------------
// Input
// ---------------------------------------------------------------------------

/* [EXPLICATION] Architecture producteur / consommateur.
   Le handler IRQ1 (producteur) push() les événements dans keyboardQueue.
   handleInput() (consommateur) les pop() un par un.

   Avantage : chaque appui physique génère exactement UN événement make dans
   la file — pas de front montant à détecter, pas de problème typematique.
   Le ring buffer absorbe les rafales de touches pressées entre deux frames.

   Schéma de flux :
     IRQ1 (IF=0)     →  push(scanCode, pressed)  →  keyboardQueue (ring buffer)
     main loop (~62fps) →  while(pop(evt))         →  action curseur / placement

   La section critique (disable_IRQs dans pop) protège tryP() contre V() en IRQ :
   voir KeyboardQueue.cpp pour le détail.                                      */
void PlantsVsZombies::handleInput() {
    /* Block all input while wave start text is displayed. */
    if (waveManager.getStartTextPhase() != 0) return;

    KeyEvent evt;
    while (keyboardQueue.pop(evt)) {
        if (!evt.pressed) continue;

        if      (evt.scanCode == SC_P1_UP)    cursorRow--;
        else if (evt.scanCode == SC_P1_DOWN)  cursorRow++;
        else if (evt.scanCode == SC_P1_LEFT)  cursorCol--;
        else if (evt.scanCode == SC_P1_RIGHT) cursorCol++;
        else if (evt.scanCode == SC_P1_PLACE) {
            int idx = queue1.getRosterCursor();
            if (idx >= queue1.getCount()) {
                queue1.triggerFlash();
            } else {
                PlantType type = queue1.getSlot(idx);
                if (!canAfford(PlantQueue::costOf(type))) {
                    sunFlashEndTick = compt + QUEUE_EMPTY_FLASH;
                    queue1.triggerFlash();
                } else if (placePlant(cursorCol, cursorRow, type)) {
                    PlantType taken;
                    queue1.tryTake(idx, taken);
                }
            }
        }
        /* Curseur roster P1 : W / X */
        else if (evt.scanCode == SC_P1_ROSTER_LEFT)  queue1.moveRosterLeft();
        else if (evt.scanCode == SC_P1_ROSTER_RIGHT) queue1.moveRosterRight();

        else if (evt.scanCode == SC_P2_UP)    cursorRow2--;
        else if (evt.scanCode == SC_P2_DOWN)  cursorRow2++;
        else if (evt.scanCode == SC_P2_LEFT)  cursorCol2--;
        else if (evt.scanCode == SC_P2_RIGHT) cursorCol2++;
        else if (evt.scanCode == SC_P2_PLACE) {
            int idx = queue2.getRosterCursor();
            if (idx >= queue2.getCount()) {
                queue2.triggerFlash();
            } else {
                PlantType type = queue2.getSlot(idx);
                if (!canAfford(PlantQueue::costOf(type))) {
                    sunFlashEndTick = compt + QUEUE_EMPTY_FLASH;
                    queue2.triggerFlash();
                } else if (placePlant(cursorCol2, cursorRow2, type)) {
                    PlantType taken;
                    queue2.tryTake(idx, taken);
                }
            }
        }
        /* Curseur roster P2 : Y / U */
        else if (evt.scanCode == SC_P2_ROSTER_LEFT)  queue2.moveRosterLeft();
        else if (evt.scanCode == SC_P2_ROSTER_RIGHT) queue2.moveRosterRight();
    }

    if (cursorCol  < 0)            cursorCol  = 0;
    if (cursorCol  >= Grid::COLS)  cursorCol  = Grid::COLS - 1;
    if (cursorRow  < 0)            cursorRow  = 0;
    if (cursorRow  >= Grid::ROWS)  cursorRow  = Grid::ROWS - 1;
    if (cursorCol2 < 0)            cursorCol2 = 0;
    if (cursorCol2 >= Grid::COLS)  cursorCol2 = Grid::COLS - 1;
    if (cursorRow2 < 0)            cursorRow2 = 0;
    if (cursorRow2 >= Grid::ROWS)  cursorRow2 = Grid::ROWS - 1;
}

// ---------------------------------------------------------------------------
// Placement
// ---------------------------------------------------------------------------

bool PlantsVsZombies::placePlant(int col, int row, PlantType type) {
    if (plantCount >= MAX_PLANTS) return false;

    int cost = PlantQueue::costOf(type);
    if (!canAfford(cost)) return false;

    if (grid.isTileOccupied(col, row)) return false;

    int px, py;
    grid.tileToPixel(col, row, px, py);

    Plant* p = 0;

    switch (type) {
        case PLANT_SNOW_PEASHOOTER:
            p = new SnowPeashooter(px, py);
            break;
        case PLANT_SUNFLOWER:
            p = new Sunflower(px, py);
            break;
        case PLANT_JALAPENO:
            p = new Jalapeno(px, py);
            break;
        case PLANT_POTATO_MINE:
            p = new PotatoMine(px, py);
            break;
        case PLANT_WALLNUT:
            p = new WallNut(px, py);
            break;
        case PLANT_CHOMPER:
            p = new Chomper(px, py);
            break;
        case PLANT_GATLING_PEA:
            p = new GatlingPea(px, py);
            break;
        case PLANT_BLOVER:
            p = new Blover(px, py);
            break;
        case PLANT_CABBAGEPULT:
            p = new CabbagePult(px, py);
            break;
        case PLANT_CACTUS:
            p = new CactusPlant(px, py);
            break;
        case PLANT_CHERRYBOMB:
            p = new CherryBomb(px, py);
            break;
        case PLANT_COBCANNON:
            p = new CobCannon(px, py);
            break;
        case PLANT_GARLIC:
            p = new Garlic(px, py);
            break;
        case PLANT_KERNELPULT:
            p = new KernelPult(px, py);
            break;
        case PLANT_MARIGOLD:
            p = new Marigold(px, py);
            break;
        case PLANT_MELONPULT:
            p = new MelonPult(px, py);
            break;
        case PLANT_REPEATER:
            p = new Repeater(px, py);
            break;
        case PLANT_SPLITPEA:
            p = new SplitPea(px, py);
            break;
        case PLANT_SQUASH:
            p = new Squash(px, py);
            break;
        case PLANT_THREEPEATER:
            p = new Threepeater(px, py);
            break;
        case PLANT_TORCHWOOD:
            p = new Torchwood(px, py);
            break;
        case PLANT_TWINSUNFLOWER:
            p = new TwinSunflower(px, py);
            break;
        case PLANT_WINTERMELON:
            p = new WinterMelon(px, py);
            break;
        case PLANT_PEASHOOTER:
        default:
            p = new Peashooter(px, py);
            break;
    }

    if (p) {
        spendSuns(cost);
        plants[plantCount++] = p;
        Tile* t = grid.getTile(col, row);
        if (t) t->setState(TileState::HasPlant);
        return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
// Cursor
// ---------------------------------------------------------------------------

void PlantsVsZombies::drawCursor(int col, int row, unsigned char color) {
    int px, py;
    grid.tileToPixel(col, row, px, py);
    int w = Grid::TILE_SIZE, h = Grid::TILE_SIZE;
    for (int x = px; x < px + w; x++) {
        video[py * SCREEN_WIDTH + x]           = color;
        video[(py + h - 1) * SCREEN_WIDTH + x] = color;
    }
    for (int y = py; y < py + h; y++) {
        video[y * SCREEN_WIDTH + px]           = color;
        video[y * SCREEN_WIDTH + (px + w - 1)] = color;
    }
}

/* Retourne le sprite (frame 0) correspondant à un PlantType. */
static const unsigned char* spriteForPlant(PlantType type, int& w, int& h) {
    switch (type) {
        case PLANT_SNOW_PEASHOOTER:
            w = SNOW_PEASHOOTER_WIDTH;
            h = SNOW_PEASHOOTER_HEIGHT;
            return snow_peashooter_frames[0];
        case PLANT_SUNFLOWER:
            w = SUNFLOWER_WIDTH;
            h = SUNFLOWER_HEIGHT;
            return sunflower_frames[0];
        case PLANT_JALAPENO:
            w = JALAPENO_WIDTH;
            h = JALAPENO_HEIGHT;
            return jalapeno_frames[0];
        case PLANT_POTATO_MINE:
            w = POTATO_MINE_WIDTH;
            h = POTATO_MINE_HEIGHT;
            return potato_mine_frames[0];
        case PLANT_WALLNUT:
            w = WALLNUT_WIDTH;
            h = WALLNUT_HEIGHT;
            return wallnut_frames[0];
        case PLANT_CHOMPER:
            w = CHOMPER_IDLE_WIDTH;
            h = CHOMPER_IDLE_HEIGHT;
            return chomper_idle_frames[0];
        case PLANT_GATLING_PEA:
            w = GATLINGPEA_IDLE_WIDTH;
            h = GATLINGPEA_IDLE_HEIGHT;
            return gatlingpea_idle_frames[0];
        case PLANT_BLOVER:
            w = BLOVER_WIDTH;
            h = BLOVER_HEIGHT;
            return blover_frames[0];
        case PLANT_CABBAGEPULT:
            w = CABBAGEPULT_WIDTH;
            h = CABBAGEPULT_HEIGHT;
            return cabbagepult_frames[0];
        case PLANT_CACTUS:
            w = CACTUS_WIDTH;
            h = CACTUS_HEIGHT;
            return cactus_frames[0];
        case PLANT_CHERRYBOMB:
            w = CHERRYBOMB_WIDTH;
            h = CHERRYBOMB_HEIGHT;
            return cherrybomb_frames[0];
        case PLANT_COBCANNON:
            w = COBCANNON_WIDTH;
            h = COBCANNON_HEIGHT;
            return cobcannon_frames[0];
        case PLANT_GARLIC:
            w = GARLIC_WIDTH;
            h = GARLIC_HEIGHT;
            return garlic_frames[0];
        case PLANT_KERNELPULT:
            w = KERNELPULT_WIDTH;
            h = KERNELPULT_HEIGHT;
            return kernelpult_frames[0];
        case PLANT_MARIGOLD:
            w = MARIGOLD_WIDTH;
            h = MARIGOLD_HEIGHT;
            return marigold_frames[0];
        case PLANT_MELONPULT:
            w = MELONPULT_WIDTH;
            h = MELONPULT_HEIGHT;
            return melonpult_frames[0];
        case PLANT_REPEATER:
            w = REAPEATER_WIDTH;
            h = REAPEATER_HEIGHT;
            return reapeater_frames[0];
        case PLANT_SPLITPEA:
            w = SPLITPEA_WIDTH;
            h = SPLITPEA_HEIGHT;
            return splitpea_frames[0];
        case PLANT_SQUASH:
            w = SQUASH_WIDTH;
            h = SQUASH_HEIGHT;
            return squash_frames[0];
        case PLANT_THREEPEATER:
            w = THREEPEATER_WIDTH;
            h = THREEPEATER_HEIGHT;
            return threepeater_frames[0];
        case PLANT_TORCHWOOD:
            w = TORCHWOOD_WIDTH;
            h = TORCHWOOD_HEIGHT;
            return torchwood_frames[0];
        case PLANT_TWINSUNFLOWER:
            w = TWINSUNFLOWER_WIDTH;
            h = TWINSUNFLOWER_HEIGHT;
            return twinsunflower_frames[0];
        case PLANT_WINTERMELON:
            w = WINTERMELON_WIDTH;
            h = WINTERMELON_HEIGHT;
            return wintermelon_frames[0];
        case PLANT_PEASHOOTER:
        default:
            w = PEASHOOTER_WIDTH;
            h = PEASHOOTER_HEIGHT;
            return peashooter_frames[0];
    }
}

/* Affiche la file de plantes d'un joueur.
   px, py : coin supérieur gauche du HUD.
   color : couleur du curseur roster. */
void PlantsVsZombies::drawQueueHud(const PlantQueue& q, int px, int py, unsigned char color) {
    const int ICON_H  = 12;
    const int ICON_W  = 10;
    const int COST_H  =  6;
    const int SLOT_W  = ICON_W + 22;   // icon + espace + chiffres coût
    const int SLOT_H  = ICON_H + 2;    // icon + 1px padding haut/bas
    const int SLOT_GAP = 2;
    bool flash = q.isFlashing();

    for (int i = 0; i < QUEUE_CAPACITY; i++) {
        int sx = px + i * (SLOT_W + SLOT_GAP);

        /* Fond du slot */
        unsigned char bg = (i < q.getCount())
            ? (unsigned char)1
            : (flash ? (unsigned char)32 : (unsigned char)0);
        for (int r = 0; r < SLOT_H; r++)
            for (int c = 0; c < SLOT_W; c++)
                video[(py + r) * SCREEN_WIDTH + (sx + c)] = bg;

        if (i < q.getCount()) {
            /* Icône de plante (sprite mis à l'échelle) */
            int srcW, srcH;
            const unsigned char* spr = spriteForPlant(q.getSlot(i), srcW, srcH);
            draw_sprite_scaled(spr, srcW, srcH,
                               sx + 1, py + 1,
                               ICON_W, ICON_H);

            /* Coût en soleils à droite de l'icône */
            draw_number(PlantQueue::costOf(q.getSlot(i)),
                        sx + ICON_W + 2, py + (SLOT_H - COST_H) / 2,
                        SUN_HUD_YELLOW, 1);

            /* Surbrillance du slot sélectionné par le curseur roster */
            if (i == q.getRosterCursor()) {
                unsigned char borderCol = flash ? SUN_HUD_RED : color;
                for (int c = sx; c < sx + SLOT_W; c++) {
                    video[py * SCREEN_WIDTH + c]                = borderCol;
                    video[(py + SLOT_H - 1) * SCREEN_WIDTH + c] = borderCol;
                }
                for (int r = py; r < py + SLOT_H; r++) {
                    video[r * SCREEN_WIDTH + sx]                = borderCol;
                    video[r * SCREEN_WIDTH + (sx + SLOT_W - 1)] = borderCol;
                }
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Boucles des threads
// ---------------------------------------------------------------------------

/* [THREAD LOGIQUE] Tourne en Round-Robin coopératif.
   Toutes les 16 ms : lit les entrées clavier, met à jour l'état du jeu,
   puis signale le thread rendu via frameSem. Cède le CPU (Yield) après
   chaque itération pour laisser le thread rendu s'exécuter.

   Le compteur de ticks (compt) est incrémenté par l'IRQ timer (1000 Hz).
   On attend le prochain tick en cédant le CPU plutôt qu'en spinant
   (while + Yield), ce qui évite d'affamer le thread rendu pendant l'attente. */
void PlantsVsZombies::runLogic() {
    int lastTick     = compt;
    int fpsTimer     = compt;
    int renderFrames = 0;

    while (true) {
        /* Attente coopérative du prochain tick — Yield() plutôt que spin pur
           afin de laisser le thread rendu dessiner la frame précédente. */
        while (compt == lastTick) thread_yield();
        lastTick = compt;

        if ((compt % LOGIC_TICK_INTERVAL) == 0) {
            /* L'input est traité avant la logique pour que les actions du joueur
               (déplacement curseur, placement de plante) soient prises en compte
               dans la frame courante. */
            handleInput();
            updateLogic();

            /* Signal au thread rendu : une nouvelle frame est prête. */
            frameSem.V();
            renderFrames++;
        }

        /* Mise à jour du compteur FPS une fois par seconde. */
        if (compt - fpsTimer >= FPS_UPDATE_INTERVAL) {
            lastFps      = renderFrames;
            lastSeconds  = compt / FPS_UPDATE_INTERVAL;
            renderFrames = 0;
            fpsTimer     = compt;
        }

        /* Fin de partie : attend que le joueur appuie sur espace, puis reset. */
        if (lives <= 0) {
            showGameOver();
            fpsTimer     = compt;
            renderFrames = 0;
        }

        /* Cède le CPU au thread rendu (et éventuellement au thread idle). */
        thread_yield();
    }
}

/* [THREAD RENDU] Tourne en Round-Robin coopératif.
   Attend le signal de runLogic() via frameSem (tryP non bloquant + Yield),
   puis compose le backbuffer et le copie dans le framebuffer VGA. */
void PlantsVsZombies::runRender() {
    while (true) {
        /* tryP() non bloquant : si aucune frame logique n'est prête, on cède
           le CPU immédiatement plutôt que de spinner (important en coopératif :
           le spinning empêcherait runLogic de s'exécuter). */
        while (!frameSem.tryP()) thread_yield();

        renderFrame();

        /* Cède le CPU après chaque frame rendue. */
        thread_yield();
    }
}

// ---------------------------------------------------------------------------
// Démarrage — crée les threads et devient thread idle
// ---------------------------------------------------------------------------

void PlantsVsZombies::start() {
    mainMenu.show();

    /* Pré-remplir les files des joueurs avec 2 plantes chacune. */
    queue1.seed(2);
    queue2.seed(2);

    /* [THREADS] Création des threads de jeu.
       Les structures sont statiques : elles persistent au-delà du retour de
       start() (qui ne se produit jamais en pratique), et leur durée de vie
       est celle du programme.

       Ordre de création : logicThread en premier, renderThread en second.
       L'ordonnanceur Round-Robin va tourner : init → logicThread → renderThread
       → init (idle) → logicThread → … */
    static PvZLogicThread  logicThread;
    static PvZRenderThread renderThread;

    logicThread.game  = this;
    renderThread.game = this;

    logicThread.start();
    renderThread.start();

    /* Le thread initial (celui qui a exécuté main()) devient un thread idle :
       il cède le CPU indéfiniment. Les threads de jeu ne terminent jamais
       (showGameOver() boucle à l'infini), donc ce thread idle ne reprend
       jamais la main en pratique. */
    while (true) thread_yield();
}
