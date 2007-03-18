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
#include "TGLobject_ship.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_ship::TGLobject_ship(float x,float y,int initial_fuel) : TGLobject(x,y)
{
	m_speed_x=0;
	m_speed_y=0;
	m_thrust_channel=-1;
	m_fuel=(initial_fuel/2)*64;
} /* TGLobject_ship::TGLobject_ship */ 


TGLobject_ship::~TGLobject_ship()
{
} /* TGLobject_ship::~TGLobject_ship */ 


void TGLobject_ship::draw(GLTManager *GLTM)
{
} /* TGLobject_ship::draw */ 


bool TGLobject_ship::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_ship")) return true;
	return TGLobject::is_a(c);
} /* TGLobject_ship::is_a */ 


bool TGLobject_ship::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_ship::is_a */ 


const char *TGLobject_ship::get_class(void)
{
	return "TGLobject_ship";
} /* TGLobject_ship:get_class */ 




int TGLobject_ship::get_fuel(void)
{
	return m_fuel;
} /* TGLobject_ship::get_fuel */ 


void TGLobject_ship::recharge_fuel(void)
{
	if (m_fuel<m_max_fuel) {
		m_fuel+=8;
		if (m_fuel>m_max_fuel) m_fuel=m_max_fuel;
	} // if 
} /* TGLobject_ship::recharge_fuel */ 


float TGLobject_ship::get_speedx(void)
{
	return m_speed_x;
} /* TGLobject_ship::get_speedx */ 



float TGLobject_ship::get_speedy(void)
{
	return m_speed_y;
} /* TGLobject_ship::get_speedy */ 


void TGLobject_ship::set_speedx(float speedx)
{
	m_speed_x=speedx;
} /* TGLobject_ship::set_speedx */ 


void TGLobject_ship::set_speedy(float speedy)
{
	m_speed_y=speedy;
} /* TGLobject_ship::set_speedy */ 
