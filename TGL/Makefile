#TransballGL (c) Braingames
#Copyright (c) ptitSeb

ifeq ($(ODROID),1)
 CFLAGS = -mcpu=cortex-a9 -mfpu=neon -mfloat-abi=hard -ftree-vectorize -fsingle-precision-constant -DODROID -I/usr/X11R6/include
else ifeq ($(LINUX),1)
 CFLAGS = -mcpu=native -Ofast -I/usr/X11R6/include
else
 CFLAGS = -mfloat-abi=softfp -fsigned-char -mcpu=cortex-a8 -mfpu=neon -ffast-math -fsingle-precision-constant -fuse-ld=gold -fuse-linker-plugin -DPANDORA
endif

CFLAGS = -Isrc -Isrc/objects -Isrc/interface `sdl-config --cflags`
LIBS = `sdl-config --libs` -lSDL_mixer -lSDL_ttf -lSDL_image -lz -lm -lrt -lGL -lGLU

CFLAGS += -Wno-write-strings
CFLAGS += -Ofast -g
CPPFLAGS := $(CFLAGS) -fpermissive

#C_INCLUDES := -I$(SDL_PATH)/include 
SRC_C := src/SDL_rotozoom.c

SRC_CPP := src/2DCMC.cpp \
			src/GLTManager.cpp \
			src/GLTile.cpp \
			src/LevelPack.cpp \
			src/PlayerProfile.cpp \
			src/SDL_glutaux.cpp \
			src/SFXManager.cpp \
			src/Symbol.cpp \
			src/TGL.cpp \
			src/TGLapp.cpp \
			src/TGLmap.cpp \
			src/TGLobject.cpp \
			src/TGLreplay.cpp \
			src/Vector.cpp \
			src/VirtualController.cpp \
			src/XMLparser.cpp \
			src/auxiliar.cpp \
			src/collision.cpp \
			src/compression.cpp \
			src/debug.cpp \
			src/fade_effects.cpp \
			src/geometrics.cpp \
			src/keyboardstate.cpp \
			src/main.cpp \
			src/ranrotb.cpp \
			src/smartTiles.cpp \
			src/sound.cpp \
			src/interface/TGLinterface.cpp \
			src/interface/TGLinterfaceBrowser.cpp \
			src/interface/TGLinterfaceConfirmation.cpp \
			src/interface/TGLinterfacePrint.cpp \
			src/interface/TGLinterfaceTextInput.cpp \
			src/interface/TGLinterfaceTileBrowser.cpp \
			src/memory_manager/debug_memorymanager.cpp \
			src/objects/TGLobject_ball.cpp \
			src/objects/TGLobject_ballstand.cpp \
			src/objects/TGLobject_big_tank_cannon.cpp \
			src/objects/TGLobject_big_tank.cpp \
			src/objects/TGLobject_bullet.cpp \
			src/objects/TGLobject_bullet_hmissile.cpp \
			src/objects/TGLobject_bullet_missile.cpp \
			src/objects/TGLobject_button.cpp \
			src/objects/TGLobject_cannon_down.cpp \
			src/objects/TGLobject_cannon_left.cpp \
			src/objects/TGLobject_cannon_right.cpp \
			src/objects/TGLobject_cannon_up.cpp \
			src/objects/TGLobject_cog.cpp \
			src/objects/TGLobject_directionalcannon_down.cpp \
			src/objects/TGLobject_directionalcannon_left.cpp \
			src/objects/TGLobject_directionalcannon_right.cpp \
			src/objects/TGLobject_directionalcannon_up.cpp \
			src/objects/TGLobject_enemy.cpp \
			src/objects/TGLobject_fastcannon_down.cpp \
			src/objects/TGLobject_fastcannon_left.cpp \
			src/objects/TGLobject_fastcannon_right.cpp \
			src/objects/TGLobject_fastcannon_up.cpp \
			src/objects/TGLobject_fast_directionalcannon_down.cpp \
			src/objects/TGLobject_fast_directionalcannon_left.cpp \
			src/objects/TGLobject_fast_directionalcannon_right.cpp \
			src/objects/TGLobject_fast_directionalcannon_up.cpp \
			src/objects/TGLobject_fuelrecharge.cpp \
			src/objects/TGLobject_FX_explosion1.cpp \
			src/objects/TGLobject_FX_explosion2.cpp \
			src/objects/TGLobject_FX_particle.cpp \
			src/objects/TGLobject_ha_directionalcannon_down.cpp \
			src/objects/TGLobject_ha_directionalcannon_left.cpp \
			src/objects/TGLobject_ha_directionalcannon_right.cpp \
			src/objects/TGLobject_ha_directionalcannon_up.cpp \
			src/objects/TGLobject_laser_down.cpp \
			src/objects/TGLobject_laser_horizontal.cpp \
			src/objects/TGLobject_laser_left.cpp \
			src/objects/TGLobject_laser_right.cpp \
			src/objects/TGLobject_laser_up.cpp \
			src/objects/TGLobject_laser_vertical.cpp \
			src/objects/TGLobject_leftdoor.cpp \
			src/objects/TGLobject_pipehscreen.cpp \
			src/objects/TGLobject_pipevscreen.cpp \
			src/objects/TGLobject_radar.cpp \
			src/objects/TGLobject_redlight.cpp \
			src/objects/TGLobject_rightdoor.cpp \
			src/objects/TGLobject_ship_accura.cpp \
			src/objects/TGLobject_ship_chain.cpp \
			src/objects/TGLobject_ship_c_harpoon.cpp \
			src/objects/TGLobject_ship.cpp \
			src/objects/TGLobject_ship_dflecter.cpp \
			src/objects/TGLobject_ship_dodger.cpp \
			src/objects/TGLobject_ship_grabber.cpp \
			src/objects/TGLobject_ship_gravis.cpp \
			src/objects/TGLobject_ship_gyrus.cpp \
			src/objects/TGLobject_ship_nitroblaster.cpp \
			src/objects/TGLobject_ship_pulsar_bullet.cpp \
			src/objects/TGLobject_ship_pulsar.cpp \
			src/objects/TGLobject_ship_shadowrunner.cpp \
			src/objects/TGLobject_ship_shield.cpp \
			src/objects/TGLobject_ship_vipperbeam.cpp \
			src/objects/TGLobject_ship_vpanther.cpp \
			src/objects/TGLobject_ship_xterminator.cpp \
			src/objects/TGLobject_soft_bullet.cpp \
			src/objects/TGLobject_spike_left.cpp \
			src/objects/TGLobject_spike_right.cpp \
			src/objects/TGLobject_tank_cannon.cpp \
			src/objects/TGLobject_tank.cpp \
			src/objects/TGLobject_tank_turret.cpp \
			src/objects/TGLobject_techno_computer.cpp \
			src/states/state_configure.cpp \
			src/states/state_editor.cpp \
			src/states/state_game.cpp \
			src/states/state_highscores.cpp \
			src/states/state_highscores_times.cpp \
			src/states/state_intro.cpp \
			src/states/state_levelpackbrowser.cpp \
			src/states/state_levelpackscreen.cpp \
			src/states/state_loadlevelpack.cpp \
			src/states/state_loadmap.cpp \
			src/states/state_mainmenu.cpp \
			src/states/state_mapeditor.cpp \
			src/states/state_playerprofile.cpp \
			src/states/state_postgame.cpp \
			src/states/state_pregame.cpp \
			src/states/state_replaybrowser.cpp \
			src/states/state_savemap.cpp \
			src/states/state_savereplay.cpp

OBJ_CPP  	:= $(SRC_CPP:.cpp=.o)
OBJ_C       := $(SRC_C:.c=.o)

all: transballgl

transballgl: $(OBJ_CPP) $(OBJ_C)
	g++ -o transballgl $(CPPFLAGS) $^ $(LIBS) 

%.o:  %.c
	gcc $(CFLAGS) -c $< -o $@

%.o:  %.cpp
	g++ $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf src/*.o
	rm transballgl
