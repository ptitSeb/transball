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
#include "sincos.h"

#include "GLTManager.h"
#include "SFXManager.h"
#include "TGLobject.h"
#include "TGLobject_bullet.h"
#include "TGLobject_enemy.h"
#include "TGLobject_directionalcannon_left.h"
#include "TGLobject_FX_particle.h"
#include "TGLobject_FX_explosion2.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_directionalcannon_left::TGLobject_directionalcannon_left(float x,float y,int ao) : TGLobject_enemy(x+19,y+15,ao)
{
	m_state=0;
	m_hitpoints=12;
	m_angle=0;
} /* TGLobject_directionalcannon_left::TGLobject_directionalcannon_left */ 


TGLobject_directionalcannon_left::~TGLobject_directionalcannon_left()
{
} /* TGLobject_directionalcannon_left::~TGLobject_directionalcannon_left */ 


bool TGLobject_directionalcannon_left::cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{
	TGLobject *ship;
	m_cycle++;
	bool in_range=false;

	ship=map->object_exists("TGLobject_ship",get_x()-320,get_y()-240,get_x()+320,get_y()+240);
	if (ship!=0) {
		// compute desired angle:
		float dx=ship->get_x()-get_x(),dy=ship->get_y()-get_y();
		float a=float((180*atan2(float(dx),float(dy)))/M_PI);
		a=90-a;
		while (a>180) a-=360;
		while (a<-180) a+=360;

		if (a>=-80 && a<=80) {
			// ship in valid range:
			if (m_angle<a) m_angle++;
			if (m_angle>a) m_angle--;

			if (fabs(m_angle-a)<2) in_range=true;
		} // if 
	} // if 


	switch(m_state) {
	case 0:	if (in_range) m_state=1;
			break;
	case 1: if (in_range) {
				// fire
				TGLobject *bullet;
				int a=m_angle;
				while(a<0) a+=360;
				while(a>=360) a-=360;
				bullet=new TGLobject_bullet(float(get_x()+(cos_table[a]*14)),float(get_y()+(sin_table[a]*14)),0,m_angle+90,2,1,GLTM->get("objects/bullet-grey"),this);
				map->add_object_back(bullet);
				m_state=128;
				Sound_play(SFXM->get("sfx/shot"),sfx_volume);

				// Smoke:
				{
					int i;
					for(i=0;i<4;i++) {
						map->add_auxiliary_back_object(new TGLobject_FX_particle(float(get_x()+(cos_table[a]*14)),float(get_y()+(sin_table[a]*14)),rand()%60,float((rand()%101)-50)/256.0f,float((rand()%101)-50)/256.0f,1,false,0.25f,0,0.25f,0.75f,50,GLTM->get("objects/smoke")));
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
		map->add_auxiliary_front_object(new TGLobject_FX_explosion2(get_x(),get_y(),128,200));
		return false;
	} // if 


	return true;
} /* TGLobject_directionalcannon_left::cycle */ 


void TGLobject_directionalcannon_left::draw(GLTManager *GLTM)
{
	if (m_state==0) m_last_tile=GLTM->get("objects/directionalcannon1");
	if (m_state==1) m_last_tile=GLTM->get("objects/directionalcannon1");
	if (m_state>124) m_last_tile=GLTM->get("objects/directionalcannon2");
	if (m_state>120 && m_state<=124) m_last_tile=GLTM->get("objects/directionalcannon3");
	if (m_state>116 && m_state<=120) m_last_tile=GLTM->get("objects/directionalcannon2");
	if (m_state>1 && m_state<=116) m_last_tile=GLTM->get("objects/directionalcannon1");
	if (m_last_tile!=0) m_last_tile->draw(m_x,m_y,0,float(m_angle),1);
} /* TGLobject_directionalcannon_left::draw */ 


bool TGLobject_directionalcannon_left::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_directionalcannon_left")) return true;
	return TGLobject_enemy::is_a(c);
} /* TGLobject_directionalcannon_left::is_a */ 


bool TGLobject_directionalcannon_left::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_directionalcannon_left::is_a */ 


const char *TGLobject_directionalcannon_left::get_class(void)
{
	return "TGLobject_directionalcannon_left";
} /* TGLobject_directionalcannon_left::get_class */ 

