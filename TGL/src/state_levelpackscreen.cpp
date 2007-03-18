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


int TGLapp::levelpackscreen_cycle(KEYBOARDSTATE *k)
{
	if (SDL_ShowCursor(SDL_QUERY)!=SDL_ENABLE) SDL_ShowCursor(SDL_ENABLE);
	if (m_state_cycle==0) {
		TGLInterfaceElement *e;
		m_selected_level=0;
		m_selected_ship=0;

		TGLinterface::reset();
		TGLinterface::add_element(new TGLbutton("Quit",m_font32,70,10,180,60,0));
		e=new TGLbutton("Change Level Pack",m_font32,270,10,320,60,1);
		e->m_enabled=false;
		TGLinterface::add_element(e);

		TGLinterface::add_element(new TGLframe(20,120,330,340));
		TGLinterface::add_element(new TGLframe(370,120,250,340));
		TGLinterface::add_element(new TGLText("Select a level:",m_font16,180,105,true));
		TGLinterface::add_element(new TGLText(m_current_levelpack->m_name,m_font32,180,150,true));
		TGLinterface::add_element(new TGLText("Select a ship:",m_font16,500,105,true));

		// Add the levels to the inferface:
		{
			int i;
			LevelPack_Level *level;
			char tmp[128];

			m_lp_first_level=m_player_profile->progress_in_levelpack(m_current_levelpack->m_id)-1;
			if (m_lp_first_level+3>m_current_levelpack->m_levels.Length()) m_lp_first_level=m_current_levelpack->m_levels.Length()-3;
			if (m_lp_first_level<0) m_lp_first_level=0;

			m_lp_level_uparrow=new TGLbutton(m_GLTM->get("interface/uparrow"),310,200,30,100,2);
			TGLinterface::add_element(m_lp_level_uparrow);
			if (m_lp_first_level==0) m_lp_level_uparrow->m_enabled=false;
			m_lp_level_downarrow=new TGLbutton(m_GLTM->get("interface/downarrow"),310,320,30,100,3);
			TGLinterface::add_element(m_lp_level_downarrow);
			if (m_lp_first_level+3>=m_current_levelpack->m_levels.Length()) m_lp_level_downarrow->m_enabled=false;

			for(i=0;i<3 && i<m_current_levelpack->m_levels.Length();i++) {
				level=m_current_levelpack->m_levels[i+m_lp_first_level];

				sprintf(tmp,"Level %i: %s",i+m_lp_first_level+1,level->m_name);
				m_lp_level_name[i]=new TGLText(tmp,m_font16,30,float(200+i*90),false);
				TGLinterface::add_element(m_lp_level_name[i]);
				{
					int time=m_player_profile->get_besttime(m_current_levelpack->m_id,i+m_lp_first_level,m_selected_ship);
					if (time==-1) {
						sprintf(tmp,"Best Time:: --:--:--");
					} else {
						int hunds=time%100;
						int secs=(time/100)%60;
						int mins=time/6000;
						sprintf(tmp,"Best Time:: %i:%.2i:%.2i",mins,secs,hunds);
					} // if 
				}
				m_lp_level_time[i]=new TGLText(tmp,m_font16,30,float(220+i*90),false);
				TGLinterface::add_element(m_lp_level_time[i]);
				m_lp_viewreplay_buttons[i]=new TGLbutton("View Replay",m_font16,180,float(210+i*90),120,20,10+i*2);
				TGLinterface::add_element(m_lp_viewreplay_buttons[i]);
				m_lp_viewreplay_buttons[i]->m_enabled=false;
				m_lp_play_buttons[i]=new TGLbutton("Play",m_font16,180,float(235+i*90),120,20,11+i*2);
				TGLinterface::add_element(m_lp_play_buttons[i]);
				if (m_player_profile->progress_in_levelpack(m_current_levelpack->m_id)<i+m_lp_first_level) {
					m_lp_play_buttons[i]->m_enabled=false;
				} else {
					SDL_WarpMouse(240,245+i*90);
					m_lp_play_buttons[i]->m_enabled=true;
				} // if 
				
			} // for
		}
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

		ID=TGLinterface::update_state(mouse_x,mouse_y,button);

		if (ID!=-1) {
			switch(ID) {
			case 0:
			case 1:
					m_state_fading=2;
					m_state_fading_cycle=0;
					m_state_selection=ID;
					break;
			case 2:
					// levels up
					{
						int i;
						LevelPack_Level *level;
						TGLInterfaceElement *old;
						char tmp[128];

						m_lp_first_level--;

						if (m_lp_first_level==0) m_lp_level_uparrow->m_enabled=false;
											else m_lp_level_uparrow->m_enabled=true;
						if (m_lp_first_level+3>=m_current_levelpack->m_levels.Length()) m_lp_level_downarrow->m_enabled=false;
																				   else m_lp_level_downarrow->m_enabled=true;

						for(i=0;i<3 && i<m_current_levelpack->m_levels.Length();i++) {
							level=m_current_levelpack->m_levels[i+m_lp_first_level];

							sprintf(tmp,"Level %i: %s",i+m_lp_first_level+1,level->m_name);
							old=m_lp_level_name[i];
							m_lp_level_name[i]=new TGLText(tmp,m_font16,30,float(200+i*90),false);
							TGLinterface::substitute_element(old,m_lp_level_name[i]);
							delete old;
							{
								int time=m_player_profile->get_besttime(m_current_levelpack->m_id,i+m_lp_first_level,m_selected_ship);
								if (time==-1) {
									sprintf(tmp,"Best Time:: --:--:--");
								} else {
									int hunds=time%100;
									int secs=(time/100)%60;
									int mins=time/6000;
									sprintf(tmp,"Best Time:: %i:%.2i:%.2i",mins,secs,hunds);
								} // if 
							}
							old=m_lp_level_time[i];
							m_lp_level_time[i]=new TGLText(tmp,m_font16,30,float(220+i*90),false);
							TGLinterface::substitute_element(old,m_lp_level_time[i]);
							delete old;
							m_lp_viewreplay_buttons[i]->m_enabled=false;
							if (m_player_profile->progress_in_levelpack(m_current_levelpack->m_id)<i+m_lp_first_level) {
								m_lp_play_buttons[i]->m_enabled=false;
							} else {
								m_lp_play_buttons[i]->m_enabled=true;
							} // if 
							
						} // for
					}
					break;
			case 3:
					// levels down
					{
						int i;
						LevelPack_Level *level;
						TGLInterfaceElement *old;
						char tmp[128];

						m_lp_first_level++;

						if (m_lp_first_level==0) m_lp_level_uparrow->m_enabled=false;
											else m_lp_level_uparrow->m_enabled=true;
						if (m_lp_first_level+3>=m_current_levelpack->m_levels.Length()) m_lp_level_downarrow->m_enabled=false;
																				   else m_lp_level_downarrow->m_enabled=true;

						for(i=0;i<3 && i<m_current_levelpack->m_levels.Length();i++) {
							level=m_current_levelpack->m_levels[i+m_lp_first_level];

							sprintf(tmp,"Level %i: %s",i+m_lp_first_level+1,level->m_name);
							old=m_lp_level_name[i];
							m_lp_level_name[i]=new TGLText(tmp,m_font16,30,float(200+i*90),false);
							TGLinterface::substitute_element(old,m_lp_level_name[i]);
							delete old;
							{
								int time=m_player_profile->get_besttime(m_current_levelpack->m_id,i+m_lp_first_level,m_selected_ship);
								if (time==-1) {
									sprintf(tmp,"Best Time:: --:--:--");
								} else {
									int hunds=time%100;
									int secs=(time/100)%60;
									int mins=time/6000;
									sprintf(tmp,"Best Time:: %i:%.2i:%.2i",mins,secs,hunds);
								} // if 
							}
							old=m_lp_level_time[i];
							m_lp_level_time[i]=new TGLText(tmp,m_font16,30,float(220+i*90),false);
							TGLinterface::substitute_element(old,m_lp_level_time[i]);
							delete old;
							m_lp_viewreplay_buttons[i]->m_enabled=false;
							if (m_player_profile->progress_in_levelpack(m_current_levelpack->m_id)<i+m_lp_first_level) {
								m_lp_play_buttons[i]->m_enabled=false;
							} else {
								m_lp_play_buttons[i]->m_enabled=true;
							} // if 
							
						} // for
					}
					break;
			case 10:
			case 12:
			case 14:
					// View replay:
					m_state_fading=2;
					m_state_fading_cycle=0;
					m_state_selection=ID;
					break;
			case 11:
			case 13:
			case 15:
					// Play:
					m_state_fading=2;
					m_state_fading_cycle=0;
					m_state_selection=ID;					
					break;
			} // switch
		} // if 
	}

	if (m_state_fading==2 && m_state_fading_cycle>25) {
		SDL_ShowCursor(SDL_DISABLE);
		switch(m_state_selection) {
		case 0: return TGL_STATE_MAINMENU;
				break;
		case 1: // return TGL_STATE_CHANGELEVELPACK
				break;
			case 10:
			case 12:
			case 14:
					// View replay:
					// ...
					break;
			case 11:
					// Play:
					m_selected_level = m_lp_first_level;
					return TGL_STATE_PREGAME;					
			case 13:
					// Play:
					m_selected_level = m_lp_first_level + 1;
					return TGL_STATE_PREGAME;					
			case 15:
					// Play:
					m_selected_level = m_lp_first_level + 2;
					return TGL_STATE_PREGAME;					
					break;
		} // switch
	} // if 

	if (m_state_fading==0 || m_state_fading==2) m_state_fading_cycle++;
	if (m_state_fading==0 && m_state_fading_cycle>25) m_state_fading=1;

	return TGL_STATE_LEVELPACKSCREEN;
} /* TGLapp::levelpackscreen_cycle */ 

void TGLapp::levelpackscreen_draw(void)
{
	glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

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
} /* TGLapp::levelpackscreen_draw */ 

