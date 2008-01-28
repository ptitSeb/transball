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
#include "TGLobject_FX_explosion1.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_bullet::TGLobject_bullet(float x,float y,int ao,int angle,float speed,int power,GLTile *tile,TGLobject *ship) : TGLobject(x,y,ao)
{
	m_angle=angle;
	m_speed=speed;
	m_power=power;
	m_tile=tile;
	exclude_for_collision(ship);
	m_state=0;
} /* TGLobject_bullet::TGLobject_bullet */ 


TGLobject_bullet::~TGLobject_bullet()
{
} /* TGLobject_bullet::~TGLobject_bullet */ 


bool TGLobject_bullet::cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{
	m_cycle++;

	switch(m_state) {
	case 0: // Collision:
			if (map->collision(this,0,0,0)) {
				TGLobject *o=map->collision_with_object(this);
				if (o!=0) {
					if (o->is_a("TGLobject_enemy") && this->check_collision(o)) {
						((TGLobject_enemy *)o)->hit(m_power);
					} // if

					// This prevents a bullet to collide with another bulled fired by the same ship:
					if (map->collision_with_foreground(this,0,0,0) ||
						!o->is_a("TGLobject_bullet") ||
						o->check_collision(m_excluded_from_collision[0])) {

						m_cycle=0;
						m_state=1;
					} // if 
				} else {		
					m_cycle=0;
					m_state=1;
				} // if 	
			} // if 

			// Move:
			if (m_state==0) {
				int a=m_angle-90;
				while(a<0) a+=360;
				while(a>=360) a-=360;
				m_x+=float((cos_table[a]*m_speed));
				m_y+=float((sin_table[a]*m_speed));
			} // if			


			if (get_x()<0 || get_y()<0 || get_x()>map->get_dx() || get_y()>map->get_dy()) return false;

			break;
	case 1: // bullet explosion:
			if (m_cycle==1) {	
				Sound_play(SFXM->get("sfx/bullet-collision"),sfx_volume);
				map->add_auxiliary_front_object(new TGLobject_FX_explosion1(get_x(),get_y(),float(4+m_power*2),30+m_power*8));
			} // if
			if (m_cycle==2) return false;
			break;
	} // switch

	
	return true;
} /* TGLobject_bullet::cycle */ 


void TGLobject_bullet::draw(GLTManager *GLTM)
{
	m_last_tile=m_tile;
	if (m_last_tile!=0) m_last_tile->draw(m_x,m_y,0,float(m_angle),1);
} /* TGLobject_bullet::draw */ 


bool TGLobject_bullet::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_bullet")) return true;
	return TGLobject::is_a(c);
} /* TGLobject_bullet::is_a */ 


bool TGLobject_bullet::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_bullet::is_a */ 


const char *TGLobject_bullet::get_class(void)
{
	return "TGLobject_bullet";
} /* TGLobject_bullet::get_class */ 

