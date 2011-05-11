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
#include "TGLobject_button.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_button::TGLobject_button(float x,float y,int ao,int event,int type) : TGLobject(x,y,ao)
{
	m_event=event;
	m_type=type;
	m_state=0;
} /* TGLobject_button::TGLobject_button */ 


TGLobject_button::~TGLobject_button()
{
} /* TGLobject_button::~TGLobject_button */ 


bool TGLobject_button::cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{
	if (m_state==1) {
		if (m_cycle==0) {
			map->action(m_event+1);
			Sound_play(SFXM->get("sfx/switch"),sfx_volume);
		} // if 
		m_cycle++;
		if (m_cycle>8) {
			m_state=0;
			m_cycle=0;
		} // if 
	} // if 
	return true;
} /* TGLobject_button::cycle */ 


void TGLobject_button::draw(GLTManager *GLTM)
{
	switch(m_type) {
	case 0: if (m_state==0) m_last_tile=GLTM->get("objects/button-red-left1");
					   else m_last_tile=GLTM->get("objects/button-red-left2");
			break;
	case 1: if (m_state==0) m_last_tile=GLTM->get("objects/button-red-right1");
					   else m_last_tile=GLTM->get("objects/button-red-right2");
			break;
	case 2: if (m_state==0) m_last_tile=GLTM->get("objects/button-red-up1");
					   else m_last_tile=GLTM->get("objects/button-red-up2");
			break;
	case 3: if (m_state==0) m_last_tile=GLTM->get("objects/button-red-down1");
					   else m_last_tile=GLTM->get("objects/button-red-down2");
			break;
	case 4: if (m_state==0) m_last_tile=GLTM->get("objects/button-purple-left1");
					   else m_last_tile=GLTM->get("objects/button-purple-left2");
			break;
	case 5: if (m_state==0) m_last_tile=GLTM->get("objects/button-purple-right1");
					   else m_last_tile=GLTM->get("objects/button-purple-right2");
			break;
	case 6: if (m_state==0) m_last_tile=GLTM->get("objects/button-purple-up1");
					   else m_last_tile=GLTM->get("objects/button-purple-up2");
			break;
	case 7: if (m_state==0) m_last_tile=GLTM->get("objects/button-purple-down1");
					   else m_last_tile=GLTM->get("objects/button-purple-down2");
			break;
	case 8: if (m_state==0) m_last_tile=GLTM->get("objects/button-blue-left1");
					   else m_last_tile=GLTM->get("objects/button-blue-left2");
			break;
	case 9: if (m_state==0) m_last_tile=GLTM->get("objects/button-blue-right1");
					   else m_last_tile=GLTM->get("objects/button-blue-right2");
			break;
	case 10: if (m_state==0) m_last_tile=GLTM->get("objects/button-blue-up1");
					   else m_last_tile=GLTM->get("objects/button-blue-up2");
			break;
	case 11: if (m_state==0) m_last_tile=GLTM->get("objects/button-blue-down1");
					   else m_last_tile=GLTM->get("objects/button-blue-down2");
			break;
	} // switch
	if (m_last_tile!=0) m_last_tile->draw(m_x,m_y,0,0,1);
} /* TGLobject_button::draw */ 


bool TGLobject_button::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_button")) return true;
	return TGLobject::is_a(c);
} /* TGLobject_button::is_a */ 


bool TGLobject_button::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_button::is_a */ 


void TGLobject_button::ball_hit(void)
{
	m_state=1;
	m_cycle=0;
} /* TGLobject_button::ball_hit */ 


const char *TGLobject_button::get_class(void)
{
	return "TGLobject_button";
} /* TGLobject_button::get_class */ 

