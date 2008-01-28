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
#include "TGLobject_ballstand.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_ballstand::TGLobject_ballstand(float x,float y,int animation_offset) : TGLobject(x,y,animation_offset)
{
} /* TGLobject_ballstand::TGLobject_ballstand */ 


TGLobject_ballstand::~TGLobject_ballstand()
{
} /* TGLobject_ballstand::~TGLobject_ballstand */ 


void TGLobject_ballstand::draw(GLTManager *GLTM)
{
	m_last_tile=GLTM->get("objects/pipe-ball-stand1");
	m_last_tile->draw(m_x,m_y,0,0,1);
} /* TGLobject_ballstand::draw */ 


bool TGLobject_ballstand::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_ballstand")) return true;
	return TGLobject::is_a(c);
} /* TGLobject_ballstand::is_a */ 


bool TGLobject_ballstand::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_ballstand::is_a */ 


const char *TGLobject_ballstand::get_class(void)
{
	return "TGLobject_ballstand";
} /* TGLobject_ballstand::get_class */ 

