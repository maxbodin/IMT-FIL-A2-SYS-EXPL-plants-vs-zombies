OS=$(shell uname)
COMPILATEUR=CPP_$(OS)
LIEUR=LD_$(OS)
COMPILATEUR_OPTION=COMPOP_$(OS)
LIEUR_OPTION=LIEUR_$(OS)

#-------------------
##Pour Mac
CPP_Darwin=i386-elf-g++-4.3.2
LD_Darwin=i386-elf-ld

#-------------------
##Pour Linux
CPP_Linux=g++
LD_Linux=ld
COMPOP_Linux=-fno-stack-protector -m32 -fpermissive
LIEUR_Linux=-m elf_i386
QEMU=qemu-system-i386 

#-------------------
##POUR WINDOWS
CPP_WindowsNT=g++
LD_WindowsNT=ld
 
#-------------------
## Partie commune a toutes les configurations

CPPFLAGS  = -gdwarf-2 -g3 -Wall -fno-builtin -fno-rtti -fno-exceptions -fno-threadsafe-statics -nostdinc --std=c++11 $($(COMPILATEUR_OPTION))
LDFLAGS = --warn-common -nostdlib $($(LIEUR_OPTION))

PWD :=.
DELE = rm -rf
MV = mv -f


KERNEL_OBJ   = sextant.elf

OBJECTSNAMES= main multiboot fonctionsES irq_wrappers i8259 idt irq Ecran Clavier timer handler_tic handler_clavier memoire vga shared_palette peashooter_sprite peashooter_shooting_sprite snow_peashooter_sprite snow_peashooter_shooting_sprite sunflower_sprite sunflower_producing_sprite jalapeno_sprite potato_mine_sprite potato_mine_popping_sprite wallnut_sprite wallnut_damaged_sprite wallnut_very_damaged_sprite chomper_idle_sprite chomper_attacking_sprite chomper_chewing_sprite gatlingpea_idle_sprite gatlingpea_shooting_sprite peabullet_sprite fire_pea_sprite snowpeabullet_sprite peabullet_impact_sprite snowpea_impact_sprite jalapeno_fire_sprite potato_mine_explosion_sprite sun_big_sprite sun_small_sprite zombie_fire_sprite grass_tile_sprite background_sprite game_over_sprite start_text_sprite titlescreen_bg_sprite titlescreen_title_sprite titlescreen_dirt_sprite titlescreen_grass_sprite zombie_walk_sprite zombie_fight_sprite basic_zombie_death_sprite disco_zombie_walk_full_sprite disco_zombie_walk_headless_sprite disco_zombie_walk_armless_sprite disco_zombie_walk_armless_headless_sprite disco_zombie_dance_walk_sprite disco_zombie_fight_full_sprite disco_zombie_fight_headless_sprite disco_zombie_death_sprite cone_zombie_walk_full_sprite cone_zombie_walk_damaged_sprite cone_zombie_walk_no_cone_sprite cone_zombie_fight_full_sprite cone_zombie_fight_damaged_sprite cone_zombie_fight_no_cone_sprite cone_zombie_death_full_sprite cone_zombie_death_damaged_sprite cone_zombie_death_no_cone_sprite pogo_zombie_pogo_full_sprite pogo_zombie_walk_full_sprite pogo_zombie_fight_full_sprite pogo_zombie_death_sprite football_zombie_walk_full_sprite football_zombie_walk_damage1_sprite football_zombie_walk_damage2_sprite football_zombie_walk_no_helmet_sprite football_zombie_fight_full_sprite football_zombie_fight_damage_sprite football_zombie_fight_no_helmet_sprite football_zombie_fight_headless_sprite football_zombie_fight_armless_sprite football_zombie_walk_headless_sprite football_zombie_walk_armless_sprite football_zombie_death_sprite backupdancer_zombie_walk_full_sprite backupdancer_zombie_walk_damaged_sprite backupdancer_zombie_walk_no_arm_sprite backupdancer_zombie_fight_full_sprite backupdancer_zombie_fight_damaged_sprite backupdancer_zombie_fight_no_arm_sprite backupdancer_zombie_dance_walk_sprite backupdancer_zombie_dance_walk_damaged_sprite backupdancer_zombie_dance_walk_no_arm_sprite backupdancer_zombie_rising_sprite backupdancer_zombie_death_sprite balloon_zombie_fly_sprite balloon_zombie_pop_sprite balloon_zombie_walk_sprite balloon_zombie_walk_no_arm_sprite balloon_zombie_fight_sprite balloon_zombie_fight_no_arm_sprite balloon_zombie_death_sprite buckethead_zombie_walk_full_sprite buckethead_zombie_walk_damaged_sprite buckethead_zombie_walk_no_bucket_sprite buckethead_zombie_fight_full_sprite buckethead_zombie_fight_damaged_sprite buckethead_zombie_fight_no_bucket_sprite buckethead_zombie_death_full_sprite buckethead_zombie_death_damaged_sprite buckethead_zombie_death_no_bucket_sprite catapult_zombie_walk_sprite catapult_zombie_fight_sprite catapult_zombie_death_sprite digger_zombie_dig_sprite digger_zombie_walk_sprite digger_zombie_fight_sprite digger_zombie_death_sprite gargantuar_zombie_walk_sprite gargantuar_zombie_fight_sprite gargantuar_zombie_death_sprite jackinthebox_zombie_walk_sprite jackinthebox_zombie_fight_sprite jackinthebox_zombie_death_sprite jalapeno_zombie_walk_sprite jalapeno_zombie_fight_sprite jalapeno_zombie_death_sprite newspaper_zombie_walk_full_sprite newspaper_zombie_walk_angry_sprite newspaper_zombie_fight_full_sprite newspaper_zombie_fight_angry_sprite newspaper_zombie_death_full_sprite newspaper_zombie_death_angry_sprite polevaulting_zombie_walk_sprite polevaulting_zombie_vault_sprite polevaulting_zombie_fight_sprite polevaulting_zombie_death_sprite screendoor_zombie_walk_full_sprite screendoor_zombie_walk_damaged_sprite screendoor_zombie_walk_no_door_sprite screendoor_zombie_fight_full_sprite screendoor_zombie_fight_damaged_sprite screendoor_zombie_fight_no_door_sprite screendoor_zombie_death_full_sprite screendoor_zombie_death_damaged_sprite screendoor_zombie_death_no_door_sprite yeti_zombie_walk_sprite yeti_zombie_walk_damaged_sprite yeti_zombie_walk_headless_sprite yeti_zombie_walk_no_arm_sprite yeti_zombie_fight_sprite yeti_zombie_death_sprite pogo_zombie_pogo_headless_sprite pogo_zombie_walk_headless_sprite pogo_zombie_walk_armless_sprite pogo_zombie_fight_headless_sprite baseball_zombie_walk_full_sprite baseball_zombie_walk_damaged_sprite baseball_zombie_walk_headless_sprite baseball_zombie_fight_full_sprite baseball_zombie_fight_damaged_sprite baseball_zombie_fight_no_bat_sprite baseball_zombie_death_sprite baseball_zombie_death_damaged_sprite basic_zombie_walk_full_sprite basic_zombie_walk_damaged_sprite basic_zombie_walk_no_arm_sprite basic_zombie_walk_headless_sprite basic_zombie_fight_full_sprite basic_zombie_fight_damaged_sprite basic_zombie_fight_no_arm_sprite basic_zombie_death_damaged_sprite blover_sprite cabbagepult_sprite cactus_sprite cherrybomb_sprite cherrybomb_explosion_sprite cobcannon_sprite garlic_sprite garlic_damaged_sprite garlic_very_damaged_sprite jalapeno_attacking_sprite kernelpult_sprite marigold_sprite marigold_attacking_sprite melonpult_sprite melonpult_attacking_sprite melon_sprite melon_impact_sprite reapeater_sprite reapeater_shooting_sprite splitpea_sprite splitpea_shooting_right_sprite splitpea_shooting_left_sprite splitpea_shooting_both_sprite squash_sprite squash_attacking_sprite threepeater_sprite threepeater_shooting_sprite torchwood_sprite torchwood_fire_sprite twinsunflower_sprite wintermelon_sprite wintermelon_attacking_sprite wintermelon_projectile_sprite wintermelon_impact_sprite Entity Plant ShooterPlant Bullet DmgIndicator Peashooter SnowPeashooter Sunflower Jalapeno PotatoMine WallNut Chomper GatlingPea Blover CabbagePult CactusPlant CherryBomb CobCannon Garlic KernelPult Marigold MelonPult Repeater SplitPea Squash Threepeater Torchwood TwinSunflower WinterMelon Sun Grid Tile MainMenu PlantsVsZombies PlantQueue WaveManager Zombie DiscoZombie ConeZombie BucketheadZombie PogoZombie FootballZombie BackupDancerZombie BalloonZombie BaseballZombie CatapultZombie DiggerZombie GargantuarZombie JackInTheBoxZombie JalapenoZombie NewspaperZombie PoleVaultingZombie ScreenDoorZombie YetiZombie Spinlock Semaphore KeyboardQueue cpu_context cpu_context_switch sched thread Threads


OBJECTS=$(patsubst %,build/all-o/%.o,$(OBJECTSNAMES))					  		

#variable pour demander a make de chercher les dependances dans n'importe quel repertoire jusqu'à 3 rep de profondeur :
VPATH=$(wildcard *):$(wildcard */*):$(wildcard */*/*):$(wildcard */*/*/*)

# Tous les headers du projet : tout .o doit être recompilé quand un .h change
# (pas de tracking transitive des #include, donc on reconstruit tout par sécurité)
HEADERS=$(wildcard */*.h) $(wildcard */*/*.h) $(wildcard */*/*/*.h) $(wildcard */*/*/*/*.h)

# les target all et clean ne sont pas "constructibles" mais appellent des recettes :
.PHONY:all clean run show


# Main target
all: $(KERNEL_OBJ)


OBJ_FILES = $(wildcard build/all-o/*.o)

$(KERNEL_OBJ): $(OBJECTS)
	@echo 'Votre compilateur $($(COMPILATEUR)) et votre lieur $($(LIEUR))'
	$($(LIEUR)) $(LDFLAGS) -T ./support/sextant.lds -o build/boot/$@ $(OBJECTS)

#compiler tout .cpp dans les repertoires de sources en .o dans le build/all-o . On cree d'abord le repertoire build/all-o s'il n'existe pas.
build/all-o/%.o:%.cpp %.h $(HEADERS)
	$($(COMPILATEUR)) -I$(PWD) -c $< $(CPPFLAGS) -o $@

# la meme que precedement, si pas de .h correspondant on compile quand meme.
build/all-o/%.o:%.cpp $(HEADERS)
	$($(COMPILATEUR)) -I$(PWD) -c $< $(CPPFLAGS) -o $@
	
#meme regle que precedente pour les fichiers .s
build/all-o/%.o: %.S $(HEADERS)
	$($(COMPILATEUR)) -I$(PWD)  -c $< $(CPPFLAGS) -DASM_SOURCE=1 -o $@



# Clean directory
clean:
	$(DELE) build/all-o/*.o
	$(DELE) *.s
	$(DELE) build/boot/*.elf

run: $(KERNEL_OBJ)
	$(QEMU) -display curses -serial file:serial.log -net nic,model=ne2k_isa -net user,tftp=./build/boot -cdrom ./build/boot/grub.iso

run_gui: $(KERNEL_OBJ)
	$(QEMU) -serial file:serial.log -net nic,model=ne2k_isa -net user,tftp=./build/boot -cdrom ./build/boot/grub.iso -monitor stdio

debug: $(KERNEL_OBJ)
	$(QEMU) -display curses -serial file:serial.log -S -s  -net nic,model=ne2k_isa -net user,tftp=./build/boot -cdrom ./build/boot/grub.iso

debug_gui: $(KERNEL_OBJ)
	$(QEMU) -serial file:serial.log -S -s  -net nic,model=ne2k_isa -net user,tftp=./build/boot -cdrom ./build/boot/grub.iso -monitor stdio

show:
	@echo "objects:$(OBJECTS)"
	@echo "VPATHS:$(VPATH)"
	@echo "compilateur:"$($(COMPILATEUR))" ; lieur:"$($(LIEUR))
 
	
