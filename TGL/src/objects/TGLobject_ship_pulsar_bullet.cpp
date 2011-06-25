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

#include "gl.h"
#include "glu.h"
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
#include "TGLobject_ship_pulsar_bullet.h"
#include "TGLobject_FX_particle.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_ship_pulsar_bullet::TGLobject_ship_pulsar_bullet(float x,float y, float angle, TGLobject *ship) : TGLobject(x,y,0)
{
	m_angle=angle;
	m_radius=8;
	exclude_for_collision(ship);
} /* TGLobject_ship_shield::TGLobject_ship_shield */ 


TGLobject_ship_pulsar_bullet::~TGLobject_ship_pulsar_bullet()
{
} /* TGLobject_ship_pulsar_bullet::~TGLobject_ship_pulsar_bullet */


bool TGLobject_ship_pulsar_bullet::cycle(VirtualController *k,TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{
	m_cycle++;
	m_radius++;
	
	// Move:
	if (m_state==0) {
		int a=m_angle-90;
		while(a<0) a+=360;
		while(a>=360) a-=360;
		m_x+=float((cos_table[a]*m_speed));
		m_y+=float((sin_table[a]*m_speed));
	} // if			
	if (get_x()<0 || get_y()<0 || get_x()>map->get_dx() || get_y()>map->get_dy()) return false;
	
	return true;
} /* TGLobject_ship_pulsar_bullet::cycle */ 

void TGLobject_ship_pulsar_bullet::draw(GLTManager *GLTM)
{
	glPushMatrix();
	glTranslatef(x,y,z);
	if (scale!=1) glScalef(scale,scale,scale);
	if (angle!=0) glRotatef(angle,0,0,1);

	// draw ripple!!!
	// ...
	
	glPopMatrix();
} /* TGLobject_ship_pulsar_bullet::draw */ 


bool TGLobject_ship_pulsar_bullet::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_ship_pulsar_bullet")) return true;
	return TGLobject::is_a(c);
} /* TGLobject_ship_pulsar_bullet::is_a */ 


bool TGLobject_ship_pulsar_bullet::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_ship_pulsar_bullet::is_a */ 


const char *TGLobject_ship_pulsar_bullet::get_class(void)
{
	return "TGLobject_ship_pulsar_bullet";
} /* TGLobject_ship_pulsar_bullet:get_class */ 

