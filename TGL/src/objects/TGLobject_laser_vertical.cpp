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

#include "GLTManager.h"
#include "SFXManager.h"
#include "TGLobject.h"
#include "TGLobject_laser_vertical.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_laser_vertical::TGLobject_laser_vertical(float x,float y,int ao,TGLobject *l1,TGLobject *l2) : TGLobject(x,y,ao)
{
	m_l1=l1;
	m_l2=l2;
} /* TGLobject_laser_vertical::TGLobject_laser_vertical */ 


TGLobject_laser_vertical::~TGLobject_laser_vertical()
{
} /* TGLobject_laser_vertical::~TGLobject_laser_vertical */ 


bool TGLobject_laser_vertical::cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{
	m_cycle++;

	if (!map->object_exists(m_l1) || !map->object_exists(m_l2)) return false;
	
	return true;
} /* TGLobject_laser_vertical::cycle */ 


void TGLobject_laser_vertical::draw(GLTManager *GLTM)
{
	int local_cycle=((m_animation_offset+m_cycle)/96)%2;
	if (local_cycle==1) {
		local_cycle=0;
		if ((m_cycle%32)==0) local_cycle=1;
		if ((m_cycle%32)==3) local_cycle=1;
		if ((m_cycle%32)==11) local_cycle=1;
		if ((m_cycle%32)==20) local_cycle=1;
		if (local_cycle==0) m_last_tile=GLTM->get("objects/laser-vertical1");
					   else m_last_tile=GLTM->get("objects/laser-vertical2");
		if (m_last_tile!=0) m_last_tile->draw(m_x,m_y,0,0,1);
	} else {
		m_last_tile=0;
	} // if 
} /* TGLobject_laser_vertical::draw */ 


bool TGLobject_laser_vertical::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_laser_vertical")) return true;
	return TGLobject::is_a(c);
} /* TGLobject_laser_vertical::is_a */ 


bool TGLobject_laser_vertical::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_laser_vertical::is_a */ 


const char *TGLobject_laser_vertical::get_class(void)
{
	return "TGLobject_laser_vertical";
} /* TGLobject_laser_vertical:get_class */ 

