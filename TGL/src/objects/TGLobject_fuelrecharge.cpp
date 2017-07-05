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
#include "TGLobject_ship.h"
#include "TGLobject_fuelrecharge.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_fuelrecharge::TGLobject_fuelrecharge(float x,float y,int ao) : TGLobject(x,y,ao)
{
} /* TGLobject_fuelrecharge::TGLobject_fuelrecharge */ 


TGLobject_fuelrecharge::~TGLobject_fuelrecharge()
{
} /* TGLobject_fuelrecharge::~TGLobject_fuelrecharge */ 


bool TGLobject_fuelrecharge::cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{
	TGLobject_ship *o;

	m_cycle++;

	o=(TGLobject_ship *)map->object_exists("TGLobject_ship",get_x(),get_y(),get_x()+64,get_y()+64);
	if (o!=0) o->recharge_fuel(SFXM,sfx_volume);
			
	return true;
} /* TGLobject_fuelrecharge::cycle */ 


void TGLobject_fuelrecharge::draw(GLTManager *GLTM)
{
	if (m_last_tile==0) m_last_tile=GLTM->get("objects/fuel-recharge");
	if (m_last_tile!=0) m_last_tile->draw(m_x,m_y,0,0,1);
} /* TGLobject_fuelrecharge::draw */ 


bool TGLobject_fuelrecharge::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_fuelrecharge")) return true;
	return TGLobject::is_a(c);
} /* TGLobject_fuelrecharge::is_a */ 


bool TGLobject_fuelrecharge::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_fuelrecharge::is_a */ 


const char *TGLobject_fuelrecharge::get_class(void)
{
	return "TGLobject_fuelrecharge";
} /* TGLobject_fuelrecharge::get_class */ 

