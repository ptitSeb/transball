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
#include "TGLobject_pipehscreen.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_pipehscreen::TGLobject_pipehscreen(float x,float y,int ao) : TGLobject(x,y,ao)
{
} /* TGLobject_pipehscreen::TGLobject_pipehscreen */ 


TGLobject_pipehscreen::~TGLobject_pipehscreen()
{
} /* TGLobject_pipehscreen::~TGLobject_pipehscreen */ 


void TGLobject_pipehscreen::draw(GLTManager *GLTM)
{
	int local_cycle=((m_animation_offset+m_cycle)%96)/8;
	if (local_cycle==0) m_last_tile=GLTM->get("objects/pipe-horizontal-screen1");
	if (local_cycle==1) m_last_tile=GLTM->get("objects/pipe-horizontal-screen2");
	if (local_cycle==2) m_last_tile=GLTM->get("objects/pipe-horizontal-screen1");
	if (local_cycle==3) m_last_tile=GLTM->get("objects/pipe-horizontal-screen2");
	if (local_cycle==4) m_last_tile=GLTM->get("objects/pipe-horizontal-screen3");
	if (local_cycle==5) m_last_tile=GLTM->get("objects/pipe-horizontal-screen2");
	if (local_cycle==6) m_last_tile=GLTM->get("objects/pipe-horizontal-screen1");
	if (local_cycle==7) m_last_tile=GLTM->get("objects/pipe-horizontal-screen3");
	if (local_cycle==8) m_last_tile=GLTM->get("objects/pipe-horizontal-screen4");
	if (local_cycle==9) m_last_tile=GLTM->get("objects/pipe-horizontal-screen2");
	if (local_cycle==10) m_last_tile=GLTM->get("objects/pipe-horizontal-screen3");
	if (local_cycle==11) m_last_tile=GLTM->get("objects/pipe-horizontal-screen2");
	if (m_last_tile!=0) m_last_tile->draw(m_x,m_y,0,0,1);
} /* TGLobject_pipehscreen::draw */ 


bool TGLobject_pipehscreen::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_pipehscreen")) return true;
	return TGLobject::is_a(c);
} /* TGLobject_pipehscreen::is_a */ 


bool TGLobject_pipehscreen::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_pipehscreen::is_a */ 


const char *TGLobject_pipehscreen::get_class(void)
{
	return "TGLobject_pipehscreen";
} /* TGLobject_pipehscreen:get_class */ 

