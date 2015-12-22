#TransballGL (c) Braingames
#Copyright (c) ptitSeb

CFLAGS = -mfloat-abi=softfp -fsigned-char -mcpu=cortex-a8 -mfpu=neon -ffast-math -fsingle-precision-constant -O3  -fuse-ld=gold -fuse-linker-plugin

CFLAGS += `sdl-config --cflags` -I/mnt/utmp/codeblocks/usr/include/GL -ITGL/src -ITGL/src/objects -ITGL/src/interface

CFLAGS += -Wno-write-strings

CPPFLAGS := $(CFLAGS) -fpermissive

LIBS = `sdl-config --libs` -lSDL_mixer -lSDL_ttf -lSDL_image -lz -lm -lrt -lGL -lGLU


#C_INCLUDES := -I$(SDL_PATH)/include 
SRC_C := TGL/src/SDL_rotozoom.c

SRC_CPP := TGL/src/2DCMC.cpp \
			TGL/src/GLTManager.cpp \
			TGL/src/GLTile.cpp \
			TGL/src/LevelPack.cpp \
			TGL/src/PlayerProfile.cpp \
			TGL/src/SDL_glutaux.cpp \
			TGL/src/SFXManager.cpp \
			TGL/src/Symbol.cpp \
			TGL/src/TGL.cpp \
			TGL/src/TGLapp.cpp \
			TGL/src/TGLmap.cpp \
			TGL/src/TGLobject.cpp \
			TGL/src/TGLreplay.cpp \
			TGL/src/Vector.cpp \
			TGL/src/VirtualController.cpp \
			TGL/src/XMLparser.cpp \
			TGL/src/auxiliar.cpp \
			TGL/src/collision.cpp \
			TGL/src/compression.cpp \
			TGL/src/debug.cpp \
			TGL/src/fade_effects.cpp \
			TGL/src/geometrics.cpp \
			TGL/src/keyboardstate.cpp \
			TGL/src/main.cpp \
			TGL/src/ranrotb.cpp \
			TGL/src/smartTiles.cpp \
			TGL/src/sound.cpp \
			TGL/src/interface/TGLinterface.cpp \
			TGL/src/interface/TGLinterfaceBrowser.cpp \
			TGL/src/interface/TGLinterfaceConfirmation.cpp \
			TGL/src/interface/TGLinterfacePrint.cpp \
			TGL/src/interface/TGLinterfaceTextInput.cpp \
			TGL/src/interface/TGLinterfaceTileBrowser.cpp \
			TGL/src/memory_manager/debug_memorymanager.cpp \
			TGL/src/objects/TGLobject_ball.cpp \
			TGL/src/objects/TGLobject_ballstand.cpp \
			TGL/src/objects/TGLobject_big_tank_cannon.cpp \
			TGL/src/objects/TGLobject_big_tank.cpp \
			TGL/src/objects/TGLobject_bullet.cpp \
			TGL/src/objects/TGLobject_bullet_hmissile.cpp \
			TGL/src/objects/TGLobject_bullet_missile.cpp \
			TGL/src/objects/TGLobject_button.cpp \
			TGL/src/objects/TGLobject_cannon_down.cpp \
			TGL/src/objects/TGLobject_cannon_left.cpp \
			TGL/src/objects/TGLobject_cannon_right.cpp \
			TGL/src/objects/TGLobject_cannon_up.cpp \
			TGL/src/objects/TGLobject_cog.cpp \
			TGL/src/objects/TGLobject_directionalcannon_down.cpp \
			TGL/src/objects/TGLobject_directionalcannon_left.cpp \
			TGL/src/objects/TGLobject_directionalcannon_right.cpp \
			TGL/src/objects/TGLobject_directionalcannon_up.cpp \
			TGL/src/objects/TGLobject_enemy.cpp \
			TGL/src/objects/TGLobject_fastcannon_down.cpp \
			TGL/src/objects/TGLobject_fastcannon_left.cpp \
			TGL/src/objects/TGLobject_fastcannon_right.cpp \
			TGL/src/objects/TGLobject_fastcannon_up.cpp \
			TGL/src/objects/TGLobject_fast_directionalcannon_down.cpp \
			TGL/src/objects/TGLobject_fast_directionalcannon_left.cpp \
			TGL/src/objects/TGLobject_fast_directionalcannon_right.cpp \
			TGL/src/objects/TGLobject_fast_directionalcannon_up.cpp \
			TGL/src/objects/TGLobject_fuelrecharge.cpp \
			TGL/src/objects/TGLobject_FX_explosion1.cpp \
			TGL/src/objects/TGLobject_FX_explosion2.cpp \
			TGL/src/objects/TGLobject_FX_particle.cpp \
			TGL/src/objects/TGLobject_ha_directionalcannon_down.cpp \
			TGL/src/objects/TGLobject_ha_directionalcannon_left.cpp \
			TGL/src/objects/TGLobject_ha_directionalcannon_right.cpp \
			TGL/src/objects/TGLobject_ha_directionalcannon_up.cpp \
			TGL/src/objects/TGLobject_laser_down.cpp \
			TGL/src/objects/TGLobject_laser_horizontal.cpp \
			TGL/src/objects/TGLobject_laser_left.cpp \
			TGL/src/objects/TGLobject_laser_right.cpp \
			TGL/src/objects/TGLobject_laser_up.cpp \
			TGL/src/objects/TGLobject_laser_vertical.cpp \
			TGL/src/objects/TGLobject_leftdoor.cpp \
			TGL/src/objects/TGLobject_pipehscreen.cpp \
			TGL/src/objects/TGLobject_pipevscreen.cpp \
			TGL/src/objects/TGLobject_radar.cpp \
			TGL/src/objects/TGLobject_redlight.cpp \
			TGL/src/objects/TGLobject_rightdoor.cpp \
			TGL/src/objects/TGLobject_ship_accura.cpp \
			TGL/src/objects/TGLobject_ship_chain.cpp \
			TGL/src/objects/TGLobject_ship_c_harpoon.cpp \
			TGL/src/objects/TGLobject_ship.cpp \
			TGL/src/objects/TGLobject_ship_dflecter.cpp \
			TGL/src/objects/TGLobject_ship_dodger.cpp \
			TGL/src/objects/TGLobject_ship_grabber.cpp \
			TGL/src/objects/TGLobject_ship_gravis.cpp \
			TGL/src/objects/TGLobject_ship_gyrus.cpp \
			TGL/src/objects/TGLobject_ship_nitroblaster.cpp \
			TGL/src/objects/TGLobject_ship_pulsar_bullet.cpp \
			TGL/src/objects/TGLobject_ship_pulsar.cpp \
			TGL/src/objects/TGLobject_ship_shadowrunner.cpp \
			TGL/src/objects/TGLobject_ship_shield.cpp \
			TGL/src/objects/TGLobject_ship_vipperbeam.cpp \
			TGL/src/objects/TGLobject_ship_vpanther.cpp \
			TGL/src/objects/TGLobject_ship_xterminator.cpp \
			TGL/src/objects/TGLobject_soft_bullet.cpp \
			TGL/src/objects/TGLobject_spike_left.cpp \
			TGL/src/objects/TGLobject_spike_right.cpp \
			TGL/src/objects/TGLobject_tank_cannon.cpp \
			TGL/src/objects/TGLobject_tank.cpp \
			TGL/src/objects/TGLobject_tank_turret.cpp \
			TGL/src/objects/TGLobject_techno_computer.cpp \
			TGL/src/states/state_configure.cpp \
			TGL/src/states/state_editor.cpp \
			TGL/src/states/state_game.cpp \
			TGL/src/states/state_highscores.cpp \
			TGL/src/states/state_highscores_times.cpp \
			TGL/src/states/state_intro.cpp \
			TGL/src/states/state_levelpackbrowser.cpp \
			TGL/src/states/state_levelpackscreen.cpp \
			TGL/src/states/state_loadlevelpack.cpp \
			TGL/src/states/state_loadmap.cpp \
			TGL/src/states/state_mainmenu.cpp \
			TGL/src/states/state_mapeditor.cpp \
			TGL/src/states/state_playerprofile.cpp \
			TGL/src/states/state_postgame.cpp \
			TGL/src/states/state_pregame.cpp \
			TGL/src/states/state_replaybrowser.cpp \
			TGL/src/states/state_savemap.cpp \
			TGL/src/states/state_savereplay.cpp

OBJ_CPP  	:= $(SRC_CPP:.cpp=.o)
OBJ_C       := $(SRC_C:.c=.o)

all: transballgl

transballgl: $(OBJ_CPP) $(OBJ_C)
	g++ -o transballgl $(CPPFLAGS) $(LIBS) $^

%.o:  %.c
	gcc $(CFLAGS) -c $< -o $@

%.o:  %.cpp
	g++ $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf *.o *.o_bs
	rm transballgl
