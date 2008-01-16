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
#include "TGLobject_rightdoor.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_rightdoor::TGLobject_rightdoor(float x,float y,int state,int action) : TGLobject(x,y)
{
	m_start_x=x;
	m_state=state;
	m_action=action;
	m_local_cycle=0;
} /* TGLobject_rightdoor::TGLobject_rightdoor */ 


TGLobject_rightdoor::~TGLobject_rightdoor()
{
} /* TGLobject_rightdoor::~TGLobject_rightdoor */ 


bool TGLobject_rightdoor::cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{
	if (m_local_cycle>22) m_local_cycle=22;

	if (m_state==0) {
		m_x=(m_start_x+22-m_local_cycle);
	} else {
		m_x=(m_start_x+m_local_cycle);
	} // if

	m_local_cycle++;

	return true;
} /* TGLobject_rightdoor::cycle */ 



bool TGLobject_rightdoor::editor_cycle(TGLmap *map,GLTManager *GLTM)
{
	if (m_state==0) {
		m_x=(m_start_x);
	} else {
		m_x=(m_start_x+22);
	} // if

	return true;
} /* TGLobject_rightdoor::cycle */ 


void TGLobject_rightdoor::draw(GLTManager *GLTM)
{
	if (m_last_tile==0) m_last_tile=GLTM->get("objects/door-right");
	if (m_last_tile!=0) m_last_tile->draw(m_x,m_y,0,0,1);
} /* TGLobject_rightdoor::draw */ 


bool TGLobject_rightdoor::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_rightdoor")) return true;
	return TGLobject::is_a(c);
} /* TGLobject_rightdoor::is_a */ 


bool TGLobject_rightdoor::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_rightdoor::is_a */ 


const char *TGLobject_rightdoor::get_class(void)
{
	return "TGLobject_rightdoor";
} /* TGLobject_rightdoor:get_class */ 



void TGLobject_rightdoor::action(int action)
{
	if (action==m_action) {
		m_state=(1-m_state);
		if (m_local_cycle>22) m_local_cycle=0;
						 else m_local_cycle=22-m_local_cycle;
	} // if 
} /* TGLobject_rightdoor::action */ 