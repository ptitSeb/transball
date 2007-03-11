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
#include "TGLobject_big_tank.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_big_tank::TGLobject_big_tank(float x,float y) : TGLobject_enemy(x,y)
{
	m_state=0;
	m_hitpoints=10;
} /* TGLobject_big_tank::TGLobject_big_tank */ 


TGLobject_big_tank::~TGLobject_big_tank()
{
} /* TGLobject_big_tank::~TGLobject_big_tank */ 


bool TGLobject_big_tank::cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{
	TGLobject *ship;

	// The bottom part of the tanks never disappears
	if (m_hitpoints<=0) return true;

	if (m_hit) {
		Sound_play(SFXM->get("sfx/enemyhit"),sfx_volume);
		m_hit=false;
	} // if 

	ship=map->object_exists("TGLobject_ship");

	// Test if the tank has to go down:
	if (!map->collision_with_foreground(this,0,1,0)) {
		m_y+=1;
	} else {
/*
		{
			float vx=0,vy=0;
			if (map->collision_vector(this,&vx,&vy)) {
				if (vy>0) {
					if (vx<0 && !map->collision_with_foreground(this,0,-1,1)) m_angle++;
					if (vx>0 && !map->collision_with_foreground(this,0,-1,-1)) m_angle--;
				} // if 
			} else {
				m_y+=PRECISION_FACTOR;
				if (map->collision_vector(this,&vx,&vy)) {
					m_y-=PRECISION_FACTOR;
					if (vy>0) {
						if (vx<0 && !map->collision_with_foreground(this,0,-1,1)) m_angle++;
						if (vx>0 && !map->collision_with_foreground(this,0,-1,-1)) m_angle--;
					} // if 
				} else {
					m_y-=PRECISION_FACTOR;
				} // if
			} // if 
		}
*/		
		if (map->collision_with_foreground(this,0,0,0) &&
			!map->collision_with_foreground(this,0,-4,0)) m_y-=1;

		{
			int i;
			int m=7;
			int max_cc=0;
			int max_acc=0;

			for(i=1;i<m;i+=2) if (!map->collision_with_foreground(this,0,0,i)) max_cc=i;
			for(i=1;i<m;i+=2) if (!map->collision_with_foreground(this,0,0,-i)) max_acc=i;

			if (max_cc>max_acc) {
				if (m_angle<35) {
					m_angle++;
				} else {
					if (m_state==1) m_state=0;
				} // if 
			} // if
			if (max_acc>max_cc && m_angle>-35) {
				if (m_angle>-35) {
					m_angle--;
				} else {
					if (m_state==0) m_state=1;
				} // if 
			} // if
		}

		if (ship!=0) {
			if (ship->get_x()<get_x()) m_state=1;
								  else m_state=0;

			switch(m_state) {
			case 0: if (!map->collision_with_foreground(this,1,-8,0)) {
						m_x+=0.5;
						m_cycle++;
					} // if 
					break;
			case 1: if (!map->collision_with_foreground(this,-1,-8,0)) {
						m_x-=0.5;
						m_cycle--;
						if (m_cycle<0) m_cycle+=8*100;
					} // if 
					break;
			} // if 
		} // if

	} // if 

	return true;
} /* TGLobject_big_tank::cycle */ 


void TGLobject_big_tank::draw(GLTManager *GLTM)
{
	int tmp=(m_cycle/2)%8;

	if (tmp==0) m_last_tile=GLTM->get("objects/big-tank-body1");
	if (tmp==1) m_last_tile=GLTM->get("objects/big-tank-body2");
	if (tmp==2) m_last_tile=GLTM->get("objects/big-tank-body3");
	if (tmp==3) m_last_tile=GLTM->get("objects/big-tank-body4");
	if (tmp==4) m_last_tile=GLTM->get("objects/big-tank-body5");
	if (tmp==5) m_last_tile=GLTM->get("objects/big-tank-body6");
	if (tmp==6) m_last_tile=GLTM->get("objects/big-tank-body7");
	if (tmp==7) m_last_tile=GLTM->get("objects/big-tank-body8");
	if (m_last_tile!=0) m_last_tile->draw(m_x,m_y,0,float(m_angle),1);
} /* TGLobject_big_tank::draw */ 


bool TGLobject_big_tank::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_big_tank")) return true;
	return TGLobject_enemy::is_a(c);
} /* TGLobject_big_tank::is_a */ 


bool TGLobject_big_tank::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_big_tank::is_a */ 


void TGLobject_big_tank::hit(int power)
{
} /* TGLobject_big_tank::is_a */ 


const char *TGLobject_big_tank::get_class(void)
{
	return "TGLobject_big_tank";
} /* TGLobject_big_tank::get_class */ 

