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
#include "TGLobject_ship.h"
#include "TGLobject_ha_directionalcannon_right.h"
#include "TGLobject_FX_particle.h"
#include "TGLobject_FX_explosion2.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_ha_directionalcannon_right::TGLobject_ha_directionalcannon_right(float x,float y) : TGLobject_enemy(x+12,y+15)
{
	m_state=0;
	m_hitpoints=12;
	m_angle=180;
} /* TGLobject_ha_directionalcannon_right::TGLobject_ha_directionalcannon_right */ 


TGLobject_ha_directionalcannon_right::~TGLobject_ha_directionalcannon_right()
{
} /* TGLobject_ha_directionalcannon_right::~TGLobject_ha_directionalcannon_right */ 


bool TGLobject_ha_directionalcannon_right::cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{
	TGLobject_ship *ship;
	m_cycle++;
	bool in_range=false;

	ship=(TGLobject_ship *)map->object_exists("TGLobject_ship",get_x()-400,get_y()-320,get_x()+400,get_y()+320);
	if (ship!=0) {
		// compute desired angle:
		float dx=ship->get_x()-get_x(),dy=ship->get_y()-get_y();
		float angle=float((180*atan2(float(dx),float(dy)))/M_PI);
		angle=90-angle;
		while (angle>360) angle-=360;
		while (angle<0) angle+=360;

		{
			int a;
			float best_a=angle;
			float s_sx=ship->get_speedx();
			float s_sy=ship->get_speedy();
			float b_sx,b_sy;
			float d;
			float ls,lb;
			float error,min_error=10000;

			int min=190,max=350;
			/* Copute the error given an angle "alpha": */ 
			for(a=min;a<max;a+=1) {
				b_sx=float(2*cos_table[a]);
				b_sy=float(2*sin_table[a]);

				d=s_sy*b_sx-s_sx*b_sy;
				if (d!=0) {
					ls=(dx*b_sy-dy*b_sx)/d;
					lb=(s_sy*dx-s_sx*dy)/d;

					if (lb>0) {
						error=float(fabs(ls-lb));
					} else {
						error=10000;
					} /* if */ 
				} else {
					error=10000;
				} /* if */ 

				if (error<min_error) {
					best_a=float(a);
					min_error=error;
				} /* if */ 
			} /* for */ 

			while (best_a>360) best_a-=360;
			while (best_a<0) best_a+=360;

			if (float(fabs(angle-best_a))>60 &&
				float(fabs(angle-best_a))<300) {
				float d=angle-best_a;

				if (d>0 && d<180) best_a=angle-60;
						     else best_a=angle+60;
			} /* if */ 

			angle=best_a;
		}	

		if (angle>=100 && angle<=260) {
			// ship in valid range:
			if (m_angle<angle) m_angle++;
			if (m_angle>angle) m_angle--;
			if (m_angle<angle) m_angle++;
			if (m_angle>angle) m_angle--;
			if (abs(m_angle-int(angle))<2) in_range=true;
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
				bullet=new TGLobject_bullet(float(get_x()+(cos_table[a]*14)),float(get_y()+(sin_table[a]*14)),m_angle+90,2,1,GLTM->get("objects/bullet-grey"),this);
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
} /* TGLobject_ha_directionalcannon_right::cycle */ 


void TGLobject_ha_directionalcannon_right::draw(GLTManager *GLTM)
{
	if (m_state==0) m_last_tile=GLTM->get("objects/ha-directionalcannon1");
	if (m_state==1) m_last_tile=GLTM->get("objects/ha-directionalcannon1");
	if (m_state>124) m_last_tile=GLTM->get("objects/ha-directionalcannon2");
	if (m_state>120 && m_state<=124) m_last_tile=GLTM->get("objects/ha-directionalcannon3");
	if (m_state>116 && m_state<=120) m_last_tile=GLTM->get("objects/ha-directionalcannon2");
	if (m_state>1 && m_state<=116) m_last_tile=GLTM->get("objects/ha-directionalcannon1");
	if (m_last_tile!=0) m_last_tile->draw(m_x,m_y,0,float(m_angle),1);
} /* TGLobject_ha_directionalcannon_right::draw */ 


bool TGLobject_ha_directionalcannon_right::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_ha_directionalcannon_right")) return true;
	return TGLobject_enemy::is_a(c);
} /* TGLobject_ha_directionalcannon_right::is_a */ 


bool TGLobject_ha_directionalcannon_right::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_ha_directionalcannon_right::is_a */ 


const char *TGLobject_ha_directionalcannon_right::get_class(void)
{
	return "TGLobject_ha_directionalcannon_right";
} /* TGLobject_ha_directionalcannon_right::get_class */ 

