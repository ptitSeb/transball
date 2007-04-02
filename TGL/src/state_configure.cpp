#ifdef KITSCHY_DEBUG_MEMORY 
#include "debug_memorymanager.h"
#endif

#ifdef _WIN32
#include "windows.h"
#endif

#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"

#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "pthread.h"

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
#include "TGLmap.h"
#include "TGL.h"
#include "TGLapp.h"
#include "TGLinterface.h"

#include "LevelPack.h"
#include "PlayerProfile.h"



int TGLapp::configure_cycle(KEYBOARDSTATE *k)
{
	if (SDL_ShowCursor(SDL_QUERY)!=SDL_ENABLE) SDL_ShowCursor(SDL_ENABLE);
	if (m_state_cycle==0) {

		m_lpb_mouse_over_lp=-1;
		m_lpb_lp_selected=-1;

		TGLinterface::reset();
		SDL_WarpMouse(120,436);
		TGLinterface::add_element(new TGLbutton("Back",m_font32,40,404,160,64,0));

		TGLinterface::add_element(new TGLbutton("Thrust",m_font16,40,20,100,32,1));
		TGLinterface::add_element(new TGLbutton("Special",m_font16,40,60,100,32,2));
		TGLinterface::add_element(new TGLbutton("Left",m_font16,40,100,100,32,3));
		TGLinterface::add_element(new TGLbutton("Right",m_font16,40,140,100,32,4));
		TGLinterface::add_element(new TGLbutton("Fire",m_font16,40,180,100,32,5));
		TGLinterface::add_element(new TGLbutton("Attractor",m_font16,40,220,100,32,6));
		TGLinterface::add_element(new TGLbutton("Pause",m_font16,40,260,100,32,7));
		TGLinterface::add_element(new TGLbutton("Quit",m_font16,40,300,100,32,8));
	
		m_configure_key_to_change=-1;
	} // if 

	if (m_state_fading==1) {
		int mouse_x=0,mouse_y=0,button=0;
		int ID=-1;
		if (!m_mouse_click_x.EmptyP()) {
			int *tmp;

			tmp=m_mouse_click_x.ExtractIni();
			mouse_x=*tmp;
			delete tmp;
			tmp=m_mouse_click_y.ExtractIni();
			mouse_y=*tmp;
			delete tmp;
			button=1;
		} else {
			SDL_GetMouseState(&mouse_x,&mouse_y);
			button=0;
		} // if 

		if (k->key_press(SDLK_SPACE) || k->key_press(SDLK_RETURN)) button=1;

		ID=TGLinterface::update_state(mouse_x,mouse_y,button,k);

		if (ID==0) {
			m_state_fading=2;
			m_state_fading_cycle=0;
			m_state_selection=ID;
		} // if 

		if (ID>=1 && ID<=8) m_configure_key_to_change=ID-1;

		if (m_configure_key_to_change!=-1) {
			int i;

			for(i=0;i<SDLK_LAST && m_configure_key_to_change!=-1;i++) {
				if (k->key_press(i)) {
					m_player_profile->m_keys_configuration[0][m_configure_key_to_change]=i;
					m_configure_key_to_change=-1;
				} // if 
			} // if 

			save_playerprofile();
		} // if 

	}

	if (m_state_fading==2 && m_state_fading_cycle>25) {
		switch(m_state_selection) {
		case 0:
				return TGL_STATE_MAINMENU;
				break;
		} // switch
	} // if 

	if (m_state_fading==0 || m_state_fading==2) m_state_fading_cycle++;
	if (m_state_fading==0 && m_state_fading_cycle>25) m_state_fading=1;


	return TGL_STATE_CONFIGURE;
} /* TGLapp::configure_cycle */ 


void TGLapp::configure_draw(void)
{

	glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

	TGLinterface::draw();

	{
		int i=0;
		char tmp[256];

		for(i=0;i<8;i++) {

			if (m_configure_key_to_change==i) {
				TGLinterface::print_left("press a new key",m_font16,160,float(44+i*40),1,0,0,1);
			} else {
				strcpy(tmp,SDL_GetKeyName((SDLKey)(m_player_profile->m_keys_configuration[0][i])));
				TGLinterface::print_left(tmp,m_font16,160,float(44+i*40),1,1,1,1);
			} // if 
		} // if 
	}

	switch(m_state_fading) {
	case 0:	
			{
				float f=0;
				f=abs(int(25-m_state_fading_cycle))/25.0F;

				fade_in_alpha(f);
			}
			break;
	case 1:
			break;
	case 2:
			{
				float f=0;
				f=abs(int(m_state_fading_cycle))/25.0F;

				fade_in_alpha(f);
			}
			break;
	} // switch
} /* TGLapp::configure_draw */ 

