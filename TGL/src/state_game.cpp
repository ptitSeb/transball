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

#include "LevelPack.h"
#include "PlayerProfile.h"



int TGLapp::game_cycle(KEYBOARDSTATE *k)
{
	int i;
	VirtualController *m_vc;
	if (SDL_ShowCursor(SDL_QUERY)!=SDL_DISABLE) SDL_ShowCursor(SDL_DISABLE);


	switch(m_game_replay_mode) {
	case 1:
			i=0;
			m_lvc.Rewind();
			while(m_lvc.Iterate(m_vc)) {
				m_vc->new_cycle();
				if (k->keyboard[m_player_profile->m_keys_configuration[i][KEY_THRUST]]) m_vc->m_joystick[VC_UP]=true;
																 else m_vc->m_joystick[VC_UP]=false;
				if (k->keyboard[m_player_profile->m_keys_configuration[i][KEY_SPECIAL]]) m_vc->m_joystick[VC_DOWN]=true;
																  else m_vc->m_joystick[VC_DOWN]=false;
				if (k->keyboard[m_player_profile->m_keys_configuration[i][KEY_LEFT]]) m_vc->m_joystick[VC_LEFT]=true;
															   else m_vc->m_joystick[VC_LEFT]=false;
				if (k->keyboard[m_player_profile->m_keys_configuration[i][KEY_RIGHT]]) m_vc->m_joystick[VC_RIGHT]=true;
																else m_vc->m_joystick[VC_RIGHT]=false;
				if (k->keyboard[m_player_profile->m_keys_configuration[i][KEY_FIRE]]) m_vc->m_button[0]=true;
															   else m_vc->m_button[0]=false;
				if (k->keyboard[m_player_profile->m_keys_configuration[i][KEY_ATTRACTOR]]) m_vc->m_button[1]=true;
																	else m_vc->m_button[1]=false;
				if (k->keyboard[m_player_profile->m_keys_configuration[i][KEY_PAUSE]]) m_vc->m_pause=true;
																 else m_vc->m_pause=false;
				if (k->keyboard[m_player_profile->m_keys_configuration[i][KEY_QUIT]]) m_vc->m_quit=true;
																else m_vc->m_quit=false;
				i=0;
			} // while 

			{
				List<TGLobject> *l=m_game->get_map()->get_objects("TGLobject");
				m_game_replay->store_cycle(&m_lvc,l);
				l->ExtractAll();
				delete l;
			}
			break;
	case 2:
			{
				List<TGLobject> *l=m_game->get_map()->get_objects("TGLobject");
				List<TGLobject> to_delete,to_add;
				TGLobject *o;
				bool retval;
				retval = m_game_replay->execute_cycle(&m_lvc,l,&to_delete,&to_add);
				l->ExtractAll();
				delete l;

				while(!to_delete.EmptyP()) {
					o=to_delete.ExtractIni();
					m_game->get_map()->remove_object(o);
					delete o;
				} // while 

				while(!to_add.EmptyP()) {
					o=to_add.ExtractIni();
					m_game->get_map()->add_object(o);
					delete o;
				} // while 

				if (!retval) {
					m_game_state=3;
					m_game_state_cycle=0;
				} // if  
				i=0;
				m_lvc.Rewind();
				while(m_lvc.Iterate(m_vc)) {
					m_vc->new_cycle();
					if (k->keyboard[m_player_profile->m_keys_configuration[i][KEY_QUIT]]) m_vc->m_quit=true;
																	else m_vc->m_quit=false;
				} // while

			}
			break;
	} // switch 
			
	m_lvc.Rewind();
	while(m_lvc.Iterate(m_vc)) {
		if (m_vc->m_quit) m_game_state=3;
	} // while 



	switch(m_game_state) {
	case 0:	// Appearing
			m_game_state_cycle++;
			if (m_game_state_cycle>50) {
				m_game_state=1;
				m_game_state_cycle=0;
			} // if 
			
			if (!m_game->cycle(&m_lvc,m_GLTM,m_SFXM,m_player_profile->m_sfx_volume)) {
				m_game_state=2;
				m_game_state_cycle=0;
			} // if 
			
			break;
	case 1:	// playing
			if (!m_game->cycle(&m_lvc,m_GLTM,m_SFXM,m_player_profile->m_sfx_volume)) {
				if (m_game->get_game_result()==2) m_game_state=2;
										     else m_game_state=3;
				m_game_state_cycle=0;
			} // if 
			break;
	case 2:	// Just waiting some time for showing the ship explosion
			m_game->cycle(&m_lvc,m_GLTM,m_SFXM,m_player_profile->m_sfx_volume);
			m_game_state_cycle++;
			if (m_game_state_cycle>100) {
				m_game_state=3;
				m_game_state_cycle=0;
			} // if 
			break;
	case 3:	// Disappearing
			m_game->cycle(&m_lvc,m_GLTM,m_SFXM,m_player_profile->m_sfx_volume);
			m_game_state_cycle++;
			if (m_game_state_cycle>50) {
				if (m_game_replay_mode==1 && m_game->get_game_result()==1) {
					// Update player profile!
					m_player_profile->level_completed(m_current_levelpack->m_id,m_selected_level,m_game_replay);
				} // if 
				return m_game_previous_state;
			} // if 
			break;
	} // switch


	return TGL_STATE_GAME;
} /* TGLapp::game_cycle */ 

void TGLapp::game_draw(void)
{
	glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

	switch(m_game_state) {
	case 0:	
			{
				float f=0;
				f=abs(int(50-m_game_state_cycle))/50.0F;

				m_game->draw(m_GLTM);

				if (m_game_fade_effect==-1) m_game_fade_effect=rand()%3;
				switch(m_game_fade_effect) {
				case 0: fade_in_alpha(f);
						break;
				case 1: fade_in_squares(f,64);
						break;
				case 2: fade_in_triangles(f,32);
						break;
				} // switch
			}
			break;
	case 1:
	case 2:
			m_game_fade_effect=-1;
			m_game->draw(m_GLTM);
			break;
	case 3:
			{
				float f=0;
				f=abs(int(m_game_state_cycle))/50.0F;

				m_game->draw(m_GLTM);

				if (m_game_fade_effect==-1) m_game_fade_effect=rand()%3;
				switch(m_game_fade_effect) {
				case 0: fade_in_alpha(f);
						break;
				case 1: fade_in_squares(f,64);
						break;
				case 2: fade_in_triangles(f,32);
						break;
				} // switch
			}
			break;
	} // switch

} /* TGLapp::game_draw */ 

