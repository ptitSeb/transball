#ifdef KITSCHY_DEBUG_MEMORY 
#include "debug_memorymanager.h"
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

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
#include "TGLobject_tank_cannon.h"
#include "TGLobject_FX_particle.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_tank_cannon::TGLobject_tank_cannon(float x,float y,int ao,TGLobject_enemy *turret) : TGLobject_enemy(x+15,y+12,ao)
{
	m_state=0;
	m_hitpoints=12;
	m_angle=270;
	m_turret=turret;
} /* TGLobject_tank_cannon::TGLobject_tank_cannon */ 


TGLobject_tank_cannon::~TGLobject_tank_cannon()
{
} /* TGLobject_tank_cannon::~TGLobject_tank_cannon */ 


bool TGLobject_tank_cannon::cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{
	TGLobject *ship;
	m_cycle++;
	bool in_range=false;

	if (m_turret->get_hitpoints()<m_hitpoints) m_hitpoints=m_turret->get_hitpoints();
	if (m_turret->get_hitpoints()>m_hitpoints) m_turret->set_hitpoints(m_hitpoints);

	{
		int a=-m_turret->get_angle();
		float ox=0,oy=-8;
		float offs_x;
		float offs_y;

		while(a<0) a+=360;
		while(a>=360) a-=360;
		offs_x=float(cos_degree(a)*ox+sin_degree(a)*oy);
		offs_y=float(cos_degree(a)*oy-sin_degree(a)*ox);

		m_x=float(m_turret->get_x()+offs_x);
		m_y=float(m_turret->get_y()+offs_y);
	}


	ship=map->object_exists("TGLobject_ship",get_x()-320,get_y()-240,get_x()+320,get_y()+240);
	if (ship!=0) {
		// compute desired angle:
		float dx=ship->get_x()-get_x(),dy=ship->get_y()-get_y();
		float a=float((180*atan2(float(dx),float(dy)))/M_PI);
		a=90-a;
		while (a>360) a-=360;
		while (a<0) a+=360;

		if (a>=190 && a<=350) {
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
				bullet=new TGLobject_bullet(float(get_x()+(cos_degree(a)*14)),float(get_y()+(sin_degree(a)*14)),0,m_angle+90,2,1,GLTM->get("objects/bullet-grey"),this);
				bullet->exclude_for_collision(m_turret);
				map->add_object_back(bullet);
				m_state=128;
				Sound_play(SFXM->get("sfx/shot"),sfx_volume);

				// Smoke:
				{
					int i;
					for(i=0;i<4;i++) {
						map->add_auxiliary_back_object(new TGLobject_FX_particle(float(get_x()+(cos_degree(a)*14)),float(get_y()+(sin_degree(a)*14)),rand()%60,float((rand()%101)-50)/256.0f,float((rand()%101)-50)/256.0f,1,false,0.25f,0,0.25f,0.75f,50,GLTM->get("objects/smoke")));
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

	if (m_hitpoints<=0) return false;

	return true;
} /* TGLobject_tank_cannon::cycle */ 


bool TGLobject_tank_cannon::editor_cycle(TGLmap *map,GLTManager *GLTM)
{
	TGLobject *ship;
	m_cycle++;
	bool in_range=false;

	if (m_turret->get_hitpoints()<m_hitpoints) m_hitpoints=m_turret->get_hitpoints();
	if (m_turret->get_hitpoints()>m_hitpoints) m_turret->set_hitpoints(m_hitpoints);

	{
		int a=-m_turret->get_angle();
		float ox=0,oy=-8;
		float offs_x;
		float offs_y;

		while(a<0) a+=360;
		while(a>=360) a-=360;
		offs_x=float(cos_degree(a)*ox+sin_degree(a)*oy);
		offs_y=float(cos_degree(a)*oy-sin_degree(a)*ox);

		m_x=float(m_turret->get_x()+offs_x);
		m_y=float(m_turret->get_y()+offs_y);
	}


	ship=map->object_exists("TGLobject_ship",get_x()-320,get_y()-240,get_x()+320,get_y()+240);
	if (ship!=0) {
		// compute desired angle:
		float dx=ship->get_x()-get_x(),dy=ship->get_y()-get_y();
		float a=float((180*atan2(float(dx),float(dy)))/M_PI);
		a=90-a;
		while (a>360) a-=360;
		while (a<0) a+=360;

		if (a>=190 && a<=350) {
			// ship in valid range:
			if (m_angle<a) m_angle++;
			if (m_angle>a) m_angle--;
			if (fabs(m_angle-a)<2) in_range=true;
		} // if 
	} // if 

	return true;
} /* TGLobject_tank_cannon::editor_cycle */


void TGLobject_tank_cannon::draw(GLTManager *GLTM)
{
	if (m_last_tile==0) m_last_tile=GLTM->get("objects/grey-tank-cannon");
	if (m_last_tile!=0) m_last_tile->draw(m_x,m_y,0,float(m_angle),1);
} /* TGLobject_tank_cannon::draw */ 


bool TGLobject_tank_cannon::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_tank_cannon")) return true;
	return TGLobject_enemy::is_a(c);
} /* TGLobject_tank_cannon::is_a */ 


bool TGLobject_tank_cannon::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_tank_cannon::is_a */ 


const char *TGLobject_tank_cannon::get_class(void)
{
	return "TGLobject_tank_cannon";
} /* TGLobject_tank_cannon:get_class */ 


