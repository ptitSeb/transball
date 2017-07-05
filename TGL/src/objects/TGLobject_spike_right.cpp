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
#include "TGLobject_spike_right.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_spike_right::TGLobject_spike_right(float x,float y,int ao) : TGLobject(x,y,ao)
{
	m_start_x=m_x;
} /* TGLobject_spike_right::TGLobject_spike_right */ 


TGLobject_spike_right::~TGLobject_spike_right()
{
} /* TGLobject_spike_right::~TGLobject_spike_right */ 


bool TGLobject_spike_right::cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{
	int tmp;

	m_cycle++;

	tmp=(m_cycle/2)%64;

	if (tmp<32) {
		m_x=m_start_x+(32-tmp);
	} else {
		m_x=m_start_x+(tmp-32);
	} // if 
		
	return true;
} /* TGLobject_spike_right::cycle */ 


bool TGLobject_spike_right::editor_cycle(TGLmap *map,GLTManager *GLTM)
{
	int tmp;

	m_cycle++;

	tmp=((m_animation_offset+m_cycle)/2)%64;

	if (tmp<32) {
		m_x=m_start_x+(32-tmp);
	} else {
		m_x=m_start_x+(tmp-32);
	} // if 
		
	return true;
} /* TGLobject_spike_right::editor_cycle */ 


void TGLobject_spike_right::draw(GLTManager *GLTM)
{
	if (m_last_tile==0) m_last_tile=GLTM->get("objects/spike-right");
	if (m_last_tile!=0) m_last_tile->draw(m_x,m_y,0,0,1);
} /* TGLobject_spike_right::draw */ 


bool TGLobject_spike_right::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_spike_right")) return true;
	return TGLobject::is_a(c);
} /* TGLobject_spike_right::is_a */ 


bool TGLobject_spike_right::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_spike_right::is_a */ 


const char *TGLobject_spike_right::get_class(void)
{
	return "TGLobject_spike_right";
} /* TGLobject_spike_right:get_class */ 


