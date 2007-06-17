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
#include "TGLreplay.h"
#include "TGLinterface.h"

#include "LevelPack.h"
#include "PlayerProfile.h"


int TGLapp::highscores_times_cycle(KEYBOARDSTATE *k)
{
	if (SDL_ShowCursor(SDL_QUERY)!=SDL_ENABLE) SDL_ShowCursor(SDL_ENABLE);

	if (m_state_cycle==0) {
	
		// compute the highscore matrix:
		{
			// ...
		}

		m_highscores_first_name=0;

		TGLinterface::reset();
		TGLinterface::add_element(new TGLbutton("Back",m_font32,10,10,160,50,0));
		TGLinterface::add_element(new TGLframe(10,70,580,325));
		TGLinterface::add_element(new TGLText("Top Times:",m_font32,410,30,true));

		TGLinterface::add_element(new TGLbutton("Best Players",m_font32,10,410,200,50,4));
		TGLinterface::add_element(new TGLbutton("Best Times",m_font32,230,410,200,50,5));
		TGLinterface::enable(4);
		TGLinterface::disable(5);

		m_highscores_uparrow=new TGLbutton(m_GLTM->get("interface/uparrow"),600,70,30,100,2);
		TGLinterface::add_element(m_highscores_uparrow);
		if (m_highscores_first_name==0) m_highscores_uparrow->m_enabled=false;
								   else m_highscores_uparrow->m_enabled=true;
		m_highscores_downarrow=new TGLbutton(m_GLTM->get("interface/downarrow"),600,290,30,100,3);
		TGLinterface::add_element(m_highscores_downarrow);
		if (m_highscores_names.Length()>HIGHSCORE_GLOBAL_PERPAGE) m_highscores_downarrow->m_enabled=true;
														     else m_highscores_downarrow->m_enabled=false;

	} // if 

	if (m_state_fading==1) {
		int mouse_x=0,mouse_y=0,button=0,button_status=0;
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
			button_status=SDL_GetMouseState(&mouse_x,&mouse_y);
			button=0;
		} // if 

		if (k->key_press(SDLK_SPACE) || k->key_press(SDLK_RETURN)) button=1;

		ID=TGLinterface::update_state(mouse_x,mouse_y,button,button_status,k);

		if (ID!=-1) {
			switch(ID) {
			case 0:
					m_state_fading=2;
					m_state_fading_cycle=0;
					m_state_selection=ID;
					break;
			case 2:
					m_highscores_first_name--;
					if (m_highscores_first_name==0) m_highscores_uparrow->m_enabled=false;
											   else m_highscores_uparrow->m_enabled=true;
					if (m_highscores_names.Length()>m_highscores_first_name+HIGHSCORE_GLOBAL_PERPAGE) m_highscores_downarrow->m_enabled=true;
																			  					 else m_highscores_downarrow->m_enabled=false;	
					break;
			case 3:
					m_highscores_first_name++;
					if (m_highscores_first_name==0) m_highscores_uparrow->m_enabled=false;
											   else m_highscores_uparrow->m_enabled=true;
					if (m_highscores_names.Length()>m_highscores_first_name+HIGHSCORE_GLOBAL_PERPAGE) m_highscores_downarrow->m_enabled=true;
																			  					 else m_highscores_downarrow->m_enabled=false;
					break;
			case 4:
					m_state_fading=2;
					m_state_fading_cycle=0;
					m_state_selection=ID;
					TGLinterface::disable(4);
					TGLinterface::disable(5);
					break;
			case 5:
					break;

			} // switch
		} // if 
	}

	if (m_state_fading==2 && m_state_fading_cycle>25) {
		SDL_ShowCursor(SDL_DISABLE);
		switch(m_state_selection) {
		case 0: if (m_lp_music_channel!=-1) Mix_HaltChannel(m_lp_music_channel);
				m_lp_music_channel=-1;
				return TGL_STATE_MAINMENU;
				break;
		case 4: return TGL_STATE_HIGHSCORES;
				break;
		case 5: return TGL_STATE_HIGHSCORES_TIMES;
				break;
		} // switch
	} // if 

	if (m_state_fading==0 || m_state_fading==2) m_state_fading_cycle++;
	if (m_state_fading==0 && m_state_fading_cycle>25) m_state_fading=1;

	if (m_state_fading==0 && m_lp_music_channel!=-1) Mix_Volume(m_lp_music_channel, int(m_player_profile->m_music_volume*(m_state_fading_cycle/25.0f)));
	if (m_state_fading==1 && m_lp_music_channel!=-1) Mix_Volume(m_lp_music_channel, m_player_profile->m_music_volume);
	if (m_state_fading==2 && m_lp_music_channel!=-1) Mix_Volume(m_lp_music_channel, int(m_player_profile->m_music_volume*((25-m_state_fading_cycle)/25.0f)));

	return TGL_STATE_HIGHSCORES_TIMES;
} /* TGLapp::highscores_times_cycle */ 


void TGLapp::highscores_times_draw(void)
{
	glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

	TGLinterface::draw();

	// show the "Best Times" highscores:
	{
		// ...
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
} /* TGLapp::highscores_times_draw */ 

