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
#include "TGLobject_enemy.h"
#include "TGLobject_laser_left.h"
#include "TGLobject_FX_explosion2.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_laser_left::TGLobject_laser_left(float x,float y,int ao) : TGLobject_enemy(x,y,ao)
{
	m_hitpoints=16;
} /* TGLobject_laser_left::TGLobject_laser_left */ 


TGLobject_laser_left::~TGLobject_laser_left()
{
} /* TGLobject_laser_left::~TGLobject_laser_left */ 


bool TGLobject_laser_left::cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{
	m_cycle++;

	if (m_hit) {
		Sound_play(SFXM->get("sfx/enemyhit"),sfx_volume);
		m_hit=false;
	} // if 

	if (m_hitpoints<=0) {
		Sound_play(SFXM->get("sfx/explosion"),sfx_volume);
		map->add_auxiliary_front_object(new TGLobject_FX_explosion2(get_x()+20,get_y()+16,128,200));
		return false;
	} // if 

	return true;
} /* TGLobject_laser_left::cycle */ 


void TGLobject_laser_left::draw(GLTManager *GLTM)
{
	int local_cycle=((m_animation_offset+m_cycle)/96)%2;
	if (local_cycle==0) m_last_tile=GLTM->get("objects/laser-left1");
				   else m_last_tile=GLTM->get("objects/laser-left2");
	if (m_last_tile!=0) m_last_tile->draw(m_x,m_y,0,0,1);
} /* TGLobject_laser_left::draw */ 


bool TGLobject_laser_left::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_laser_left")) return true;
	return TGLobject_enemy::is_a(c);
} /* TGLobject_laser_left::is_a */ 


bool TGLobject_laser_left::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_laser_left::is_a */ 


const char *TGLobject_laser_left::get_class(void)
{
	return "TGLobject_laser_left";
} /* TGLobject_laser_left::get_class */ 

