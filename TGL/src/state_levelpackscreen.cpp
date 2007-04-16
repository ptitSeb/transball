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


int TGLapp::levelpackscreen_cycle(KEYBOARDSTATE *k)
{
	bool m_recheck_interface=false;
	bool m_reload_tutorial=false;
/*
    if (m_lp_music_channel == -1) {
        m_lp_music_channel = Sound_play_continuous(m_SFXM->get("levelpack"), m_player_profile->m_music_volume);
    } // if 
*/

	if (m_game!=0) {
		delete m_game;
		m_game=0;
	} // if 

	if (SDL_ShowCursor(SDL_QUERY)!=SDL_ENABLE) SDL_ShowCursor(SDL_ENABLE);
	if (m_state_cycle==0) {
		TGLInterfaceElement *e;
		m_selected_level=0;
		m_lp_replay_mode=0;
		m_lp_replay_timmer=0;

		if (m_lp_tutorial_game!=0) {
			delete m_lp_tutorial_game;
			m_lp_tutorial_game=0;
		} // if 
		if (m_lp_tutorial_replay!=0) {
			delete m_lp_tutorial_replay;
			m_lp_tutorial_replay=0;
		} // if 

		// Make sure that the current player has the selected ship unlocked:
		if (!m_player_profile->m_ships.MemberP(&m_selected_ship)) {
			m_selected_ship=*(m_player_profile->m_ships[0]);
		} // if 

		TGLinterface::reset();
		TGLinterface::add_element(new TGLbutton("Back",m_font32,70,10,180,60,0));
		e=new TGLbutton("Change Level Pack",m_font32,270,10,320,60,1);
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

			for(i=0;i<3 && i<m_current_levelpack->m_levels.Length();i++) {
				int time;

				level=m_current_levelpack->m_levels[i+m_lp_first_level];

				sprintf(tmp,"Level %i: %s",i+m_lp_first_level+1,level->m_name);
				m_lp_level_name[i]=new TGLText(tmp,m_font16,30,float(200+i*90),false);
				TGLinterface::add_element(m_lp_level_name[i]);

				time=m_player_profile->get_besttime(m_current_levelpack->m_id,i+m_lp_first_level,m_selected_ship);
				if (time==-1) {
					sprintf(tmp,"Best Time:: --:--:--");
				} else {
					int hunds=(time/10)%100;
					int secs=(time/1000)%60;
					int mins=time/60000;
					sprintf(tmp,"Best Time:: %i:%.2i:%.2i",mins,secs,hunds);
				} // if 
				m_lp_level_time[i]=new TGLText(tmp,m_font16,30,float(220+i*90),false);
				TGLinterface::add_element(m_lp_level_time[i]);
				m_lp_viewreplay_buttons[i]=new TGLbutton("View Replay",m_font16,180,float(210+i*90),120,20,10+i*2);
				TGLinterface::add_element(m_lp_viewreplay_buttons[i]);
				if (time==-1) m_lp_viewreplay_buttons[i]->m_enabled=false;
					     else m_lp_viewreplay_buttons[i]->m_enabled=true;
				m_lp_play_buttons[i]=new TGLbutton("Play",m_font16,180,float(235+i*90),120,20,11+i*2);
				TGLinterface::add_element(m_lp_play_buttons[i]);
				if (m_player_profile->progress_in_levelpack(m_current_levelpack->m_id)<i+m_lp_first_level) {
					m_lp_play_buttons[i]->m_enabled=false;
				} else {
					SDL_WarpMouse(240,245+i*90);
					m_lp_play_buttons[i]->m_enabled=true;
				} // if 
				
			} // for

			m_lp_level_uparrow=new TGLbutton(m_GLTM->get("interface/uparrow"),310,200,30,100,2);
			TGLinterface::add_element(m_lp_level_uparrow);
			if (m_lp_first_level==0) m_lp_level_uparrow->m_enabled=false;
			m_lp_level_downarrow=new TGLbutton(m_GLTM->get("interface/downarrow"),310,320,30,100,3);
			TGLinterface::add_element(m_lp_level_downarrow);
			if (m_lp_first_level+3>=m_current_levelpack->m_levels.Length()) m_lp_level_downarrow->m_enabled=false;

		}

		// Add the ships to the interface:
		{
			m_lp_ship_leftarrow=new TGLbutton(m_GLTM->get("interface/leftarrow"),380,140,40,40,4);
			TGLinterface::add_element(m_lp_ship_leftarrow);
			if (m_player_profile->m_ships.Position(&m_selected_ship)==0) m_lp_ship_leftarrow->m_enabled=false;
																    else m_lp_ship_leftarrow->m_enabled=true;
			m_lp_ship_rightarrow=new TGLbutton(m_GLTM->get("interface/rightarrow"),570,140,40,40,5);
			TGLinterface::add_element(m_lp_ship_rightarrow);
			if (m_player_profile->m_ships.Position(&m_selected_ship)==m_player_profile->m_ships.Length()-1) m_lp_ship_rightarrow->m_enabled=false;
																									   else m_lp_ship_rightarrow->m_enabled=true;
		}

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

		if (m_lp_replay_mode==0) ID=TGLinterface::update_state(mouse_x,mouse_y,button,button_status,k);
					        else ID=-1;

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
						m_lp_first_level--;
						m_recheck_interface=true;
					}
					break;
			case 3:
					// levels down
					{
						m_lp_first_level++;
						m_recheck_interface=true;
					}
					break;
			case 4:
					{
						int pos=m_player_profile->m_ships.Position(&m_selected_ship);
						pos--;
						m_selected_ship=*(m_player_profile->m_ships[pos]);
						m_recheck_interface=true;
						m_reload_tutorial=true;
					}
					break;
			case 5:
					{
						int pos=m_player_profile->m_ships.Position(&m_selected_ship);
						pos++;
						m_selected_ship=*(m_player_profile->m_ships[pos]);
						m_recheck_interface=true;
						m_reload_tutorial=true;
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

	if (m_recheck_interface) {
		int i;
		LevelPack_Level *level;
		TGLInterfaceElement *old;
		char tmp[128];


		if (m_lp_first_level==0) m_lp_level_uparrow->m_enabled=false;
							else m_lp_level_uparrow->m_enabled=true;
		if (m_lp_first_level+3>=m_current_levelpack->m_levels.Length()) m_lp_level_downarrow->m_enabled=false;
																   else m_lp_level_downarrow->m_enabled=true;

		for(i=0;i<3 && i<m_current_levelpack->m_levels.Length();i++) {
			int time;
			level=m_current_levelpack->m_levels[i+m_lp_first_level];

			sprintf(tmp,"Level %i: %s",i+m_lp_first_level+1,level->m_name);
			old=m_lp_level_name[i];
			m_lp_level_name[i]=new TGLText(tmp,m_font16,30,float(200+i*90),false);
			TGLinterface::substitute_element(old,m_lp_level_name[i]);
			delete old;
			time=m_player_profile->get_besttime(m_current_levelpack->m_id,i+m_lp_first_level,m_selected_ship);
			if (time==-1) {
				sprintf(tmp,"Best Time:: --:--:--");
			} else {
				int hunds=(time/10)%100;
				int secs=(time/1000)%60;
				int mins=time/60000;
				sprintf(tmp,"Best Time:: %i:%.2i:%.2i",mins,secs,hunds);
			} // if 
			old=m_lp_level_time[i];
			m_lp_level_time[i]=new TGLText(tmp,m_font16,30,float(220+i*90),false);
			TGLinterface::substitute_element(old,m_lp_level_time[i]);
			delete old;
			if (time==-1) m_lp_viewreplay_buttons[i]->m_enabled=false;
				     else m_lp_viewreplay_buttons[i]->m_enabled=true;
			if (m_player_profile->progress_in_levelpack(m_current_levelpack->m_id)<i+m_lp_first_level) {
				m_lp_play_buttons[i]->m_enabled=false;
			} else {
				m_lp_play_buttons[i]->m_enabled=true;
			} // if 
		} // for

		int pos=m_player_profile->m_ships.Position(&m_selected_ship);
		if (pos==0) m_lp_ship_leftarrow->m_enabled=false;
			   else m_lp_ship_leftarrow->m_enabled=true;
		if (pos==m_player_profile->m_ships.Length()-1) m_lp_ship_rightarrow->m_enabled=false;
												  else m_lp_ship_rightarrow->m_enabled=true;											
	} // if 

	if (m_reload_tutorial || m_lp_tutorial_game==0) {
		char *ship_tutorial[]={"tutorial1-vp",
							   "tutorial1-xt",
							   "tutorial1-sr",
							   "tutorial1-nb",
							   "tutorial1-vb",
							   "tutorial1-dodg",
							   "tutorial1-gravis",
							   "tutorial1-acc",
							   "tutorial1-gyr",
							   "tutorial1-def",
							   "tutorial1-harp",
								};

		if (m_lp_tutorial_game!=0) {
			delete m_lp_tutorial_game;
			m_lp_tutorial_game=0;
		} // if 
		if (m_lp_tutorial_replay!=0) {
			delete m_lp_tutorial_replay;
			m_lp_tutorial_replay=0;
		} // if 

		if (ship_tutorial[m_selected_ship]!=0) {
			char *m_lp_replay_name=0;
			m_lp_replay_name=new char[strlen(ship_tutorial[m_selected_ship])+15];
			sprintf(m_lp_replay_name,"tutorials/%s.rpl",ship_tutorial[m_selected_ship]);

			FILE *fp=fopen(m_lp_replay_name,"rb");
			if (fp!=0) {
				m_lp_tutorial_replay=new TGLreplay(fp);
				fclose(fp);
			} // if 
			delete []m_lp_replay_name;
		} // if 
	
		
	} // if 


	if (m_lp_tutorial_replay!=0) {
		if (m_lp_tutorial_game==0) {
			m_lp_tutorial_game=new TGL(m_lp_tutorial_replay->get_map(),
									   m_lp_tutorial_replay->get_playership("default"),
									   m_lp_tutorial_replay->get_initial_fuel(),
									   0,
									   0,m_GLTM);
		} // if 

		List<TGLobject> *l=m_lp_tutorial_game->get_map()->get_objects("TGLobject");
		List<TGLobject> to_delete,to_add;
		TGLobject *o;
		bool retval;
		retval = m_lp_tutorial_replay->execute_cycle(&m_lvc,l,&to_delete,&to_add);
		l->ExtractAll();
		delete l;

		while(!to_delete.EmptyP()) {
			o=to_delete.ExtractIni();
			m_lp_tutorial_game->get_map()->remove_object(o);
			delete o;
		} // while 

		while(!to_add.EmptyP()) {
			o=to_add.ExtractIni();
			m_lp_tutorial_game->get_map()->add_object(o);
			delete o;
		} // while 

		if (m_lp_replay_mode==2) {
			if (!m_lp_tutorial_game->cycle(&m_lvc,m_GLTM,m_SFXM,m_player_profile->m_sfx_volume)) retval=false;
		} else {
			if (!m_lp_tutorial_game->cycle(&m_lvc,m_GLTM,m_SFXM,0)) retval=false;
		} // if
		
		if (!retval) {
			delete m_lp_tutorial_game;
			m_lp_tutorial_game=0;
			delete m_lp_tutorial_replay;
			m_lp_tutorial_replay=0;
		} // if  
	} // if 

	if (k->key_press(SDLK_f)) {
		if (m_lp_replay_mode==0) {
			m_lp_replay_mode=1;
			m_lp_replay_timmer=0;
		} else if (m_lp_replay_mode==2) {
			m_lp_replay_mode=3;
			m_lp_replay_timmer=0;
		} // if 
	} // if 

	if (m_lp_replay_mode==1) {
		m_lp_replay_timmer++;
		if (m_lp_replay_timmer>=25) {
			m_lp_replay_mode=2;
			m_lp_replay_timmer=0;
		} // if 
	} // if 
	if (m_lp_replay_mode==3) {
		m_lp_replay_timmer++;
		if (m_lp_replay_timmer>=25) {
			m_lp_replay_mode=0;
			m_lp_replay_timmer=0;
		} // if 
	} // if 

	if (m_state_fading==2 && m_state_fading_cycle>25) {
		SDL_ShowCursor(SDL_DISABLE);
		switch(m_state_selection) {
		case 0: if (m_lp_music_channel!=-1) Mix_HaltChannel(m_lp_music_channel);
				m_lp_music_channel=-1;
				return TGL_STATE_MAINMENU;
				break;
		case 1: return TGL_STATE_LEVELPACKBROWSER;
				break;
			case 10:
					// View replay:
					{
						// m_lp_first_level

						m_game_replay_mode=2;
						{
							FILE *fp;
							char tmp[256];
							sprintf(tmp,"players/%s/%s-level-%i-%i.rpl",
										m_player_profile->m_name,m_current_levelpack->m_id,m_lp_first_level,
										m_selected_ship);
							fp=fopen(tmp,"rb");
							m_game_replay=new TGLreplay(fp);
							m_game_replay->rewind();
							fclose(fp);
						}
						char *map_name=m_current_levelpack->m_levels[m_lp_first_level]->m_map;
						m_game=new TGL(map_name,m_selected_ship,m_current_levelpack->m_levels[m_lp_first_level]->m_initial_fuel,m_player_profile->m_sfx_volume,m_player_profile->m_music_volume,m_GLTM);

						m_game_state=0;
						m_game_state_cycle=0;

						m_game_previous_state=TGL_STATE_LEVELPACKSCREEN;
						m_game_reinit_previous_state=false;
						return TGL_STATE_GAME;
					}
					break;
			case 12:
					// View replay:
					{
						// m_lp_first_level

						m_game_replay_mode=2;
						{
							FILE *fp;
							char tmp[256];
							sprintf(tmp,"players/%s/%s-level-%i-%i.rpl",
										m_player_profile->m_name,m_current_levelpack->m_id,m_lp_first_level+1,
										m_selected_ship);
							fp=fopen(tmp,"rb");
							m_game_replay=new TGLreplay(fp);
							m_game_replay->rewind();
							fclose(fp);
						}
						char *map_name=m_current_levelpack->m_levels[m_lp_first_level+1]->m_map;
						m_game=new TGL(map_name,m_selected_ship,m_current_levelpack->m_levels[m_lp_first_level+1]->m_initial_fuel,m_player_profile->m_sfx_volume,m_player_profile->m_music_volume,m_GLTM);

						m_game_state=0;
						m_game_state_cycle=0;

						m_game_previous_state=TGL_STATE_LEVELPACKSCREEN;
						m_game_reinit_previous_state=false;
						return TGL_STATE_GAME;
					}
					break;
			case 14:
					// View replay:
					{
						// m_lp_first_level

						m_game_replay_mode=2;
						{
							FILE *fp;
							char tmp[256];
							sprintf(tmp,"players/%s/%s-level-%i-%i.rpl",
										m_player_profile->m_name,m_current_levelpack->m_id,m_lp_first_level+2,
										m_selected_ship);
							fp=fopen(tmp,"rb");
							m_game_replay=new TGLreplay(fp);
							m_game_replay->rewind();
							fclose(fp);
						}
						char *map_name=m_current_levelpack->m_levels[m_lp_first_level+2]->m_map;
						m_game=new TGL(map_name,m_selected_ship,m_current_levelpack->m_levels[m_lp_first_level+2]->m_initial_fuel,m_player_profile->m_sfx_volume,m_player_profile->m_music_volume,m_GLTM);

						m_game_state=0;
						m_game_state_cycle=0;

						m_game_previous_state=TGL_STATE_LEVELPACKSCREEN;
						m_game_reinit_previous_state=false;
						return TGL_STATE_GAME;
					}
					break;
			case 11:
					// Play:
					m_selected_level = m_lp_first_level;
					if (m_lp_music_channel!=-1) Mix_HaltChannel(m_lp_music_channel);
					m_lp_music_channel=-1;
					return TGL_STATE_PREGAME;					
			case 13:
					// Play:
					m_selected_level = m_lp_first_level + 1;
					if (m_lp_music_channel!=-1) Mix_HaltChannel(m_lp_music_channel);
					m_lp_music_channel=-1;
					return TGL_STATE_PREGAME;					
			case 15:
					// Play:
					m_selected_level = m_lp_first_level + 2;
					if (m_lp_music_channel!=-1) Mix_HaltChannel(m_lp_music_channel);
					m_lp_music_channel=-1;
					return TGL_STATE_PREGAME;					
					break;
		} // switch
	} // if 

	if (m_state_fading==0 || m_state_fading==2) m_state_fading_cycle++;
	if (m_state_fading==0 && m_state_fading_cycle>25) m_state_fading=1;

	if (m_state_fading==0 && m_lp_music_channel!=-1) Mix_Volume(m_lp_music_channel, int(m_player_profile->m_music_volume*(m_state_fading_cycle/25.0f)));
	if (m_state_fading==1 && m_lp_music_channel!=-1) Mix_Volume(m_lp_music_channel, m_player_profile->m_music_volume);
	if (m_state_fading==2 && m_lp_music_channel!=-1) Mix_Volume(m_lp_music_channel, int(m_player_profile->m_music_volume*((25-m_state_fading_cycle)/25.0f)));

	return TGL_STATE_LEVELPACKSCREEN;
} /* TGLapp::levelpackscreen_cycle */ 

void TGLapp::levelpackscreen_draw(void)
{
	float replay_full_factor=0;

	glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

	TGLinterface::draw();
	
	// Draw Selected ship:
	{
		char *ship_tiles[]={"objects/ship-vpanther-1",
							"objects/ship-xterminator-1",
							"objects/ship-srunner-1",
							"objects/ship-nblaster-1",
							"objects/ship-vbeam-1",
							"objects/ship-dodger-1",
							"objects/ship-gravis-1",
							"objects/ship-accura-1",
							"objects/ship-gyrus",
							"objects/ship-dflecter-1",
							"objects/ship-harpoon",
							};
		char *ship_names[]={"V-Panther",
							"X-Terminator",
							"Shadow Runner",
							"Nitro Blaster",
							"Vipper Beam",
							"Dodger K7",
							"Gravis T8",
							"Accura T5",
							"Gyrus-P",
							"D-Flecter",
							"C-Harpoon",
							};

		int i,s;
		GLTile *t;

		for(i=-1;i<2;i++) {
			s=m_player_profile->m_ships.Position(&m_selected_ship)+i;
			if (s>=0 && s<m_player_profile->m_ships.Length()) {
				t=m_GLTM->get(ship_tiles[*(m_player_profile->m_ships[s])]);

				if (i==0) {
					t->draw(495,160,0,0,1);
				} else {
					t->draw(1,1,1,0.33f,float(495+i*48),160,0,0,0.66f);
				} // if 

			} // if
		} // for

		TGLinterface::print_center(ship_names[m_selected_ship],m_font32,495,220);
	}

	switch(m_lp_replay_mode) {
	case 0:
			replay_full_factor=0;
			break;
	case 1:
			{
				float f=0;
				f=abs(m_lp_replay_timmer)/25.0F;
				fade_in_alpha(f);
				replay_full_factor=f;
			}
			break;
	case 2: fade_in_alpha(1);
			replay_full_factor=1;
			break;
	case 3:
			{
				float f=0;
				f=abs(25-m_lp_replay_timmer)/25.0F;
				fade_in_alpha(f);
				replay_full_factor=f;
			}
			break;
	} // switch

	// Draw tutorial:
	{
		if (m_lp_tutorial_game!=0) {
			int old[4];

			glGetIntegerv(GL_VIEWPORT,old);
			glViewport(int(380*(1-replay_full_factor)),int(80*(1-replay_full_factor)), 
					   int(640*replay_full_factor+230*(1-replay_full_factor)),int(480*replay_full_factor+172*(1-replay_full_factor)));
			m_lp_tutorial_game->draw(m_GLTM);
			glViewport(old[0],old[1],old[2],old[3]);

			{
				float f=0.6f+0.4f*float(sin(m_state_cycle*0.1));

				if (m_lp_replay_mode==0 || m_lp_replay_mode==1) TGLinterface::print_center("Press F for fullscreen",m_font16,320*replay_full_factor+495*(1-replay_full_factor),20*replay_full_factor+250*(1-replay_full_factor),1,1,1,f);
				if (m_lp_replay_mode==2 || m_lp_replay_mode==3) TGLinterface::print_center("Press F for windowed",m_font16,320*replay_full_factor+495*(1-replay_full_factor),20*replay_full_factor+250*(1-replay_full_factor),1,1,1,f);
			}

			{
				int i,j;
				float y;
				char buffer[128];
				char *tmp=m_lp_tutorial_replay->get_text();
				if (tmp!=0) {
					i=0;
					y=445*replay_full_factor+425*(1-replay_full_factor);
					while(tmp[i]!=0) {
						for(j=0;tmp[i]!=0 && tmp[i]!='/';i++,j++) buffer[j]=tmp[i];
						buffer[j]=0;
						if (tmp[i]=='/') i++;
						TGLinterface::print_center(buffer,m_font16,320*replay_full_factor+495*(1-replay_full_factor),y);
						y+=20;
					} // while

				} // if 
			} 
		} else {
			TGLinterface::print_center("No tutorial available",m_font16,320*replay_full_factor+495*(1-replay_full_factor),240*replay_full_factor+300*(1-replay_full_factor));
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
} /* TGLapp::levelpackscreen_draw */ 

