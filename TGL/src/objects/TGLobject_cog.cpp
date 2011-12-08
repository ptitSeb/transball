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
#include "TGLobject_cog.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_cog::TGLobject_cog(float x,float y, int ao) : TGLobject(x,y,ao)
{
} /* TGLobject_cog::TGLobject_cog */ 


TGLobject_cog::~TGLobject_cog()
{
} /* TGLobject_cog::~TGLobject_cog */ 


bool TGLobject_cog::cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{
	m_cycle++;
	m_angle+=0.25;
	
	return true;
} /* TGLobject_cog::cycle */ 


bool TGLobject_cog::editor_cycle(TGLmap *map,GLTManager *GLTM)
{
	m_cycle++;
	m_angle+=0.25;

	return true;
} /* TGLobject_cog::editor_cycle */ 


void TGLobject_cog::draw(GLTManager *GLTM)
{
	if (m_last_tile==0) m_last_tile=GLTM->get("objects/cog");
	if (m_last_tile!=0) m_last_tile->draw(m_x,m_y,0,m_angle,1);
} /* TGLobject_cog::draw */ 


bool TGLobject_cog::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_cog")) return true;
	return TGLobject::is_a(c);
} /* TGLobject_cog::is_a */ 


bool TGLobject_cog::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_cog::is_a */ 


const char *TGLobject_cog::get_class(void)
{
	return "TGLobject_cog";
} /* TGLobject_cog:get_class */ 


