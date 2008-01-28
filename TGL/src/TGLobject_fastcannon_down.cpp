#ifdef KITSCHY_DEBUG_MEMORY 
#include "debug_memorymanager.h"
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "string.h"

#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include "List.h"
#include "Symbol.h"
#include "2DCMC.h"
#include "auxiliar.h"
#include "GLTile.h"
#include "2DCMC.h"
#include "sound.h"
#include "keyboardstate.h"
#include "randomc.h"
#include "VirtualController.h"

#include "GLTManager.h"
#include "SFXManager.h"
#include "TGLobject.h"
#include "TGLobject_bullet.h"
#include "TGLobject_enemy.h"
#include "TGLobject_fastcannon_down.h"
#include "TGLobject_FX_particle.h"
#include "TGLobject_FX_explosion2.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_fastcannon_down::TGLobject_fastcannon_down(float x,float y,int ao) : TGLobject_enemy(x,y,ao)
{
	m_state=0;
	m_hitpoints=8;
} /* TGLobject_fastcannon_down::TGLobject_fastcannon_down */ 


TGLobject_fastcannon_down::~TGLobject_fastcannon_down()
{
} /* TGLobject_fastcannon_down::~TGLobject_fastcannon_down */ 


bool TGLobject_fastcannon_down::cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{
	TGLobject *ship;
	m_cycle++;

	switch(m_state) {
	case 0:	ship=map->object_exists("TGLobject_ship",get_x()-16,get_y()-240,get_x()+48,get_y()+32);
			if (ship!=0) m_state=1;
			break;
	case 1: ship=map->object_exists("TGLobject_ship",get_x()-16,get_y()-240,get_x()+48,get_y()+32);
			if (ship!=0) {
				// fire
				map->add_object_back(new TGLobject_bullet(get_x()+15,get_y(),0,0,5,1,GLTM->get("objects/bullet-grey"),this));
				m_state=64;
				Sound_play(SFXM->get("sfx/shot"),sfx_volume);

				// Smoke:
				{
					int i;
					for(i=0;i<4;i++) {
						map->add_auxiliary_back_object(new TGLobject_FX_particle(get_x()+15,get_y(),rand()%60,float((rand()%101)-50)/256.0f,float((rand()%101)-50)/256.0f,1,false,0.25f,0,0.25f,0.75f,50,GLTM->get("objects/smoke")));
					} // for
				}
			} else {
				m_state=0;
			} // if
			break;
	default:
			m_state--;
	} // switch

	if (m_hit) {
		Sound_play(SFXM->get("sfx/enemyhit"),sfx_volume);
		m_hit=false;
	} // if 

	if (m_hitpoints<=0) {
		Sound_play(SFXM->get("sfx/explosion"),sfx_volume);
		map->add_auxiliary_front_object(new TGLobject_FX_explosion2(get_x()+16,get_y()+16,128,200));
		return false;
	} // if 


	return true;
} /* TGLobject_fastcannon_down::cycle */ 


void TGLobject_fastcannon_down::draw(GLTManager *GLTM)
{
	m_last_tile=GLTM->get("objects/fastcannon-techno-down1");
	if (m_last_tile!=0) m_last_tile->draw(m_x,m_y,0,0,1);
} /* TGLobject_fastcannon_down::draw */ 


bool TGLobject_fastcannon_down::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_fastcannon_down")) return true;
	return TGLobject_enemy::is_a(c);
} /* TGLobject_fastcannon_down::is_a */ 


bool TGLobject_fastcannon_down::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_fastcannon_down::is_a */ 


const char *TGLobject_fastcannon_down::get_class(void)
{
	return "TGLobject_fastcannon_down";
} /* TGLobject_fastcannon_down::get_class */ 

