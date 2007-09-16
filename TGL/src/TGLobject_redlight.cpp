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
#include "TGLobject_redlight.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_redlight::TGLobject_redlight(float x,float y,int type) : TGLobject(x,y)
{
	m_type = type;
} /* TGLobject_redlight::TGLobject_redlight */ 


TGLobject_redlight::~TGLobject_redlight()
{
} /* TGLobject_redlight::~TGLobject_redlight */ 


void TGLobject_redlight::draw(GLTManager *GLTM)
{
	int local_cycle=m_cycle%48;
	if (m_type==0) {
		if (local_cycle<24) m_last_tile=GLTM->get("objects/red-light1");
					   else m_last_tile=GLTM->get("objects/red-light2");
	} // if
	if (m_type==1) {
		if (local_cycle<24) m_last_tile=GLTM->get("objects/red-light1-snow");
					   else m_last_tile=GLTM->get("objects/red-light2-snow");
	} // if 
	if (m_last_tile!=0) m_last_tile->draw(m_x,m_y,0,0,1);
} /* TGLobject_redlight::draw */ 


bool TGLobject_redlight::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_redlight")) return true;
	return TGLobject::is_a(c);
} /* TGLobject_redlight::is_a */ 


bool TGLobject_redlight::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_redlight::is_a */ 


const char *TGLobject_redlight::get_class(void)
{
	return "TGLobject_redlight";
} /* TGLobject_redlight:get_class */ 

