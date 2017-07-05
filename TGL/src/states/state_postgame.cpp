#ifdef KITSCHY_DEBUG_MEMORY 
#include "debug_memorymanager.h"
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
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
#include "TGLobject_ship.h"
#include "TGLmap.h"
#include "TGL.h"
#include "TGLapp.h"
#include "TGLreplay.h"
#include "TGLinterface.h"

#include "LevelPack.h"
#include "PlayerProfile.h"



int TGLapp::postgame_cycle(KEYBOARDSTATE *k)
{
	int points_for_ship[12]={0,0,0,5,150,25,75,125,40,55,15,100};

	if (SDL_ShowCursor(SDL_QUERY)!=SDL_ENABLE) SDL_ShowCursor(SDL_ENABLE);
	if (m_state_cycle==0) {
		TGLInterfaceElement *e;
		int i;

		m_ship_unlocked=-1;

		TGLinterface::reset();
		if (m_game->get_game_result()!=1) SDL_WarpMouse(210,352);
									 else SDL_WarpMouse(430,352);
		TGLinterface::add_element(new TGLbutton("Retry",m_font32,110,320,200,64,0));
		e=new TGLbutton("Continue",m_font32,330,320,200,64,1);
		if (m_game->get_game_result()!=1) e->m_enabled=false;
		TGLinterface::add_element(e);
		TGLinterface::add_element(new TGLbutton("Back",m_font32,110,404,200,64,2));
		TGLinterface::add_element(new TGLbutton("Save Replay",m_font32,330,404,200,64,3));

		for(i=0;i<11 && m_ship_unlocked==-1;i++) {
			if (!m_player_profile->has_ship(i) && m_player_profile->get_points()>=points_for_ship[i]) {
				m_ship_unlocked=i;
				m_player_profile->m_ships.Add(new int(m_ship_unlocked));
			} // if 
		} // for
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
			tmp=m_mouse_click_button.ExtractIni();
			button=*tmp;
			delete tmp;
		} else {
			button_status=SDL_GetMouseState(&mouse_x,&mouse_y);
			button=0;
		} // if 

		if (k->key_press(SDLK_SPACE) || k->key_press(SDLK_RETURN)) button=1;

		ID=TGLinterface::update_state(mouse_x,mouse_y,button,button_status,k);

		if (ID!=-1) {
			m_state_fading=2;
			m_state_fading_cycle=0;
			m_state_selection=ID;
		} // if 
	}

	if (m_state_fading==2 && m_state_fading_cycle>25) {
		switch(m_state_selection) {
		case 0:
				SDL_ShowCursor(SDL_DISABLE);
				return TGL_STATE_PREGAME;
				break;
		case 1:
				m_selected_level++;
				if (m_selected_level>=m_current_levelpack->m_levels.Length()) {
					// ... level pack complete
					return TGL_STATE_MAINMENU;
				} else {
					return TGL_STATE_PREGAME;
				} // if 
				break;
		case 2:
				return TGL_STATE_LEVELPACKSCREEN;
				break;
		case 3:
				return TGL_STATE_SAVEREPLAY;
				break;
		} // switch
	} // if 

	if (m_state_fading==0 || m_state_fading==2) m_state_fading_cycle++;
	if (m_state_fading==0 && m_state_fading_cycle>25) m_state_fading=1;

	return TGL_STATE_POSTGAME;
} /* TGLapp::postgame_cycle */ 

void TGLapp::postgame_draw(void)
{
	char buffer[255];
	glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

	{
		int time=-1;
		int fuel_used = -1;
		int shots = -1;
		float max_speed = 0.0f;
		float average_speed = 0.0f;
		int enemies_killed = -1;
		float accuracy = 0.0f;

		if (m_game->get_game_result()==1) {
			TGLinterface::print_center("Congratulations!",m_font32,320,96);
			time=m_game_replay->get_length();
		} else {
			TGLinterface::print_center("Better luck next time...",m_font16,320,96);
		} // if 

		fuel_used = m_game_replay->get_fuel_used();
		shots = m_game_replay->get_n_shots();
		max_speed = m_game_replay->get_max_speed();
		average_speed = m_game_replay->get_average_speed();

		if (time!=-1) {
			int mins,secs,milis;
			milis=time*18;
			secs=milis/1000;
			milis=milis%1000;
			mins=secs/60;
			secs=secs%60;
			sprintf(buffer,"Time taken: %i:%.2i:%.2i",mins,secs,milis/10);
		} else {
			sprintf(buffer,"Time taken: --:--:--");
		} // if 
		TGLinterface::print_center(buffer,m_font16,320,125);

		sprintf(buffer,"Fuel used: %i    Shots: %i",fuel_used,shots);
		TGLinterface::print_center(buffer,m_font16,320,145);

		sprintf(buffer,"Max speed: %.2f    Average Speed: %.2f",max_speed,average_speed);
		TGLinterface::print_center(buffer,m_font16,320,165);

		sprintf(buffer,"Current score: %i",m_player_profile->get_points());
		TGLinterface::print_center(buffer,m_font16,320,200);
		
		if (m_ship_unlocked!=-1) {
			sprintf(buffer,"New ship unlocked: '%s'!",TGL::ship_names[m_ship_unlocked]);
		} // if 
		TGLinterface::print_center(buffer,m_font16,320,225);
	}

	TGLinterface::draw();

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
} /* TGLapp::postgame_draw */ 

