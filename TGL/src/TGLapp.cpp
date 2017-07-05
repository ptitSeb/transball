#ifdef KITSCHY_DEBUG_MEMORY 
#include "debug_memorymanager.h"
#endif

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <pthread.h>

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



/* Frames per second counter: */ 
extern int frames_per_sec;
extern int frames_per_sec_tmp;
extern int init_time;
extern bool show_fps;
extern bool fullscreen;
extern int current_cycle;

TGLapp::TGLapp()
{
	int i;
	
#ifdef _WIN32
	strcpy(m_player_data_path,"");
#else

#ifdef __DEBUG_MESSAGES
	{
		char tmp[256];
		output_debug_message("CWD %s\n",getcwd(tmp,256));
	}
#endif
	sprintf(m_player_data_path,"%s/.transballGL/",getenv("HOME"));
	system("mkdir ~/.transballGL");
	system("mkdir ~/.transballGL/players");
	system("mkdir ~/.transballGL/other-players");
	system("mkdir ~/.transballGL/replays");
	system("mkdir ~/.transballGL/maps");
#endif

	m_font32=TTF_OpenFont("fonts/arial.ttf",32);
	m_font16=TTF_OpenFont("fonts/arial.ttf",16);
	m_ifont32=TTF_OpenFont("fonts/impact.ttf",32);

	m_current_levelpack=0;
	m_selected_level=0;
	m_selected_ship=0;

	m_mm_game=0;
	m_mm_replay=0;

	m_lp_tutorial_game=0;
	m_lp_tutorial_replay=0;
	m_lp_music_channel=-1;

	m_state=TGL_STATE_PLAYERPROFILE;
	m_previous_state = m_state;
	m_state_cycle=0;
	m_state_fading=0;
	m_state_fading_cycle=0;
	m_state_selection=0;

	m_game_paused = false;
	m_game_state=0;
	m_game_fade_effect=-1;
	m_game_replay_mode=0;	
	m_game_replay=0;
	m_game_previous_state=-1;
	m_game_reinit_previous_state=true;

	m_replay_name_inputframe=0;
	m_replay_save_button=0;
	m_sr_first_replay=0;
	m_replay_rename_button=0;
	m_replay_play_button=0;
	m_replay_delete_button=0;
	m_rb_mouse_over_replay=-1;
	m_rb_replay_selected=-1;

	m_lpb_select_button=0;
	m_lpb_lp_uparrow=0;
	m_lpb_lp_downarrow=0;
	m_lpb_first_lp=0;
	m_lpb_mouse_over_lp=-1;
	m_lpb_lp_selected=-1;

	m_configure_fullscreen=0;
	m_configure_window=0;
	m_configure_music_volume=0;
	m_configure_sfx_volume=0;

	m_profile_first_profile=0;

	m_highscores_level_pack=0;
	m_highscores_first_ship=0;
	m_highscores_first_level=0;

	m_editor_levelpack=0;
	m_editor_level=0;

	m_screen_dx=640;
	m_screen_dy=480;
	fullscreen=false;
	
	m_GLTM=new GLTManager();
	m_SFXM=new SFXManager();
	m_SFXM->cache("sfx");

	m_player_profile=0;

	m_game=0;

	for(i=0;i<MAXLOCAL_PLAYERS;i++) {
		VirtualController *vc = new VirtualController();
		vc->reset();
		m_lvc.Add(vc);
	} // for 
	
} /* TGLapp::TGLapp */ 


TGLapp::~TGLapp()
{
	TGLinterface::clear_print_cache();
	m_GLTM->clear();

	TTF_CloseFont(m_font32);
	TTF_CloseFont(m_font16);
	TTF_CloseFont(m_ifont32);

	save_playerprofile();
	
	// save which was the last open player profile:
	if (m_player_profile!=NULL) {
		char tmp[256];
		sprintf(tmp,"%slastplayer",m_player_data_path);
		FILE *fp=fopen(tmp,"w+");
		if (fp!=NULL) {
			fprintf(fp, m_player_profile->m_name);
			fclose(fp);
		}
	}

	if (m_mm_game!=0) delete m_mm_game;
	m_mm_game=0;
	if (m_mm_replay!=0) delete m_mm_replay;
	m_mm_replay=0;

	if (m_lp_music_channel!=-1) Mix_HaltChannel(m_lp_music_channel);

	if (m_player_profile!=0) delete m_player_profile;
	m_player_profile=0;

	if (m_current_levelpack!=0) delete m_current_levelpack;
	m_current_levelpack=0;

	if (m_game!=0) delete m_game;
	m_game=0;

	if (m_lp_tutorial_game!=0) {
		delete m_lp_tutorial_game;
		m_lp_tutorial_game=0;
	} // if 
	if (m_lp_tutorial_replay!=0) {
		delete m_lp_tutorial_replay;
		m_lp_tutorial_replay=0;
	} // if 

	if (m_game_replay!=0) delete m_game_replay;
	m_game_replay=0;

	if (m_editor_levelpack!=0) delete m_editor_levelpack;
	m_editor_levelpack=0;

	m_editor_tiles.ExtractAll();
	m_editor_object_tiles.ExtractAll();
	m_editor_smart_tiles.ExtractAll();
	m_editor_smart_tile_palette.ExtractAll();

	delete m_GLTM;
	delete m_SFXM;

} /* TGLapp::TGLapp */ 


bool TGLapp::cycle(KEYBOARDSTATE *k)
{
	int old_state=m_state;

  
#ifdef __DEBUG_MESSAGES
	if (m_state_cycle==0) {
		output_debug_message("First Cycle started for state %i...\n",m_state);
	} /* if */ 
#endif

	switch(m_state) {
	case TGL_STATE_PLAYERPROFILE:m_state=playerprofile_cycle(k);
								break;
	case TGL_STATE_INTRO:m_state=intro_cycle(k);
								break;
	case TGL_STATE_MAINMENU:m_state=mainmenu_cycle(k);
									break;
	case TGL_STATE_CONFIGURE:m_state=configure_cycle(k);
							 break;
	case TGL_STATE_LEVELPACKSCREEN:m_state=levelpackscreen_cycle(k);
									break;
	case TGL_STATE_LEVELPACKBROWSER:m_state=levelpackbrowser_cycle(k);
									break;
	case TGL_STATE_PREGAME:m_state=pregame_cycle(k);
							   break;
	case TGL_STATE_GAME:m_state=game_cycle(k);
							   break;
	case TGL_STATE_POSTGAME:m_state=postgame_cycle(k);
							   break;
	case TGL_STATE_SAVEREPLAY: m_state=savereplay_cycle(k);
							   break;
	case TGL_STATE_REPLAYBROWSER: m_state=replaybrowser_cycle(k);
							   break;
	case TGL_STATE_HIGHSCORES: m_state=highscores_cycle(k);
							   break;
	case TGL_STATE_HIGHSCORES_TIMES: m_state=highscores_times_cycle(k);
							   break;
	case TGL_STATE_EDITOR: m_state=editor_cycle(k);
							   break;
	case TGL_STATE_MAPEDITOR: m_state=mapeditor_cycle(k);
							   break;
	case TGL_STATE_LOADLEVELPACK: m_state=loadlevelpack_cycle(k);
							   break;
	case TGL_STATE_LOADMAP: m_state=loadmap_cycle(k);
							   break;
	case TGL_STATE_SAVEMAP: m_state=savemap_cycle(k);
							   break;
	default:return false;
	} /* switch */ 

	if (old_state==m_state) {
		m_state_cycle++;
	} else {
		if (old_state==TGL_STATE_GAME) {
			if (m_game_reinit_previous_state) {
				m_state_cycle=0;
			} else {
				m_state_cycle=1;
			} // if 
			m_game_reinit_previous_state=true;
		} else {
			m_state_cycle=0;
		} // if 

		m_state_fading=0;
		m_state_fading_cycle=0;

#ifdef __DEBUG_MESSAGES
		output_debug_message("State change: %i -> %i\n",old_state,m_state);
#endif
	} /* if */ 

	m_SFXM->next_cycle();

	if (m_player_profile!=0) fullscreen=m_player_profile->m_fullscreen;

	m_previous_state = old_state;

	return true;
} /* TGLapp::cycle */ 


void TGLapp::draw(int SCREEN_X,int SCREEN_Y)
{
	float lightpos[4]={0,0,-1000,0};
	float tmpls[4]={1.0F,1.0F,1.0F,1.0};
	float tmpld[4]={1.0F,1.0F,1.0F,1.0};
	float tmpla[4]={1.0F,1.0F,1.0F,1.0};
    float ratio;

	// If no CYCLE has been executed for this state, do not redraw:
	if (m_state_cycle==0) return;

	m_screen_dx=SCREEN_X;
	m_screen_dy=SCREEN_Y;

#ifdef __DEBUG_MESSAGES
	if (m_state_cycle==0) {
		output_debug_message("First Drawing cycle started for state %i...\n",m_state);
	} /* if */ 
#endif

	/* Enable Lights, etc.: */ 	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0,GL_POSITION,lightpos);
	glLightfv(GL_LIGHT0,GL_AMBIENT,tmpla);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,tmpld);
	glLightfv(GL_LIGHT0,GL_SPECULAR,tmpls);

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel( GL_SMOOTH );
	glDisable( GL_CULL_FACE );
	glDisable( GL_SCISSOR_TEST );  
	glDisable( GL_DEPTH_TEST );
	glPolygonMode(GL_FRONT,GL_FILL);
	
    glClearColor(0,0,0,0.0);
    glViewport(0,0,SCREEN_X,SCREEN_Y);
	ratio=(float)SCREEN_X/float(SCREEN_Y);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective( 30.0, ratio, 1.0, 10240.0 );
	gluLookAt(320,240,PERSPECTIVE_DISTANCE ,320,240,0, 0,-1,0);	/* for 640x480 better */ 

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	switch(m_state) {
	case TGL_STATE_PLAYERPROFILE:playerprofile_draw();
								 break;
	case TGL_STATE_INTRO:intro_draw();
								break;
	case TGL_STATE_MAINMENU:mainmenu_draw();
									break;
	case TGL_STATE_CONFIGURE:configure_draw();
									break;
	case TGL_STATE_LEVELPACKSCREEN:levelpackscreen_draw();
									break;
	case TGL_STATE_LEVELPACKBROWSER:levelpackbrowser_draw();
									break;
	case TGL_STATE_PREGAME:pregame_draw();
							   break;
	case TGL_STATE_GAME:game_draw();
							   break;
	case TGL_STATE_POSTGAME:postgame_draw();
							   break;
	case TGL_STATE_SAVEREPLAY: savereplay_draw();
							   break;
	case TGL_STATE_REPLAYBROWSER: replaybrowser_draw();
							   break;
	case TGL_STATE_HIGHSCORES: highscores_draw();
							   break;
	case TGL_STATE_HIGHSCORES_TIMES: highscores_times_draw();
							   break;
	case TGL_STATE_EDITOR: editor_draw();
							   break;
	case TGL_STATE_MAPEDITOR: mapeditor_draw();
							   break;
	case TGL_STATE_LOADLEVELPACK: loadlevelpack_draw();
							   break;
	case TGL_STATE_LOADMAP: loadmap_draw();
							   break;
	case TGL_STATE_SAVEMAP: savemap_draw();
							   break;
	} // switch 
 	

	if (show_fps) {
		char tmp[80];
		sprintf(tmp,"video mem: %.4gmb - fps: %i",float(GLTile::get_memory_used())/float(1024*1024),frames_per_sec);
		TGLinterface::print_center(tmp,m_font16,320,460);
	} /* if */ 

	glDisable(GL_BLEND);

	SDL_GL_SwapBuffers();
} /* TGLapp::draw */ 



int TGLapp::screen_x(int x)
{
	return ((x*m_screen_dx)/640);
} /* TGLapp::screen_x */ 


int TGLapp::screen_y(int y)
{
	return ((y*m_screen_dy)/480);
} /* TGLapp::screen_y */ 


void TGLapp::save_playerprofile(void)
{
	FILE *fp;
	char tmp[256];

	if (m_player_profile!=0) {
		sprintf(tmp,"%splayers/%s.pp",m_player_data_path,m_player_profile->m_name);
		fp=fopen(tmp,"w+");
		if (fp!=0) {
			m_player_profile->save(fp);
			fclose(fp);
		} // if 
	} // if 
} /* TGLapp::save_playerprofile */ 


void TGLapp::load_playerprofile(char *name) 
{
	FILE *fp;
	char tmp[256];

	sprintf(tmp,"%splayers/%s.pp",m_player_data_path,name);
	fp=fopen(tmp,"rb");
	if (fp!=0) {
		m_player_profile=new PlayerProfile(fp);
		fclose(fp);
	} else {
		m_player_profile=new PlayerProfile("default");
	} // if 
} /* TGLapp::load_configuration */ 


void TGLapp::MouseClick(int mousex,int mousey,int button) 
{
	m_mouse_click_x.Add(new int(mousex));
	m_mouse_click_y.Add(new int(mousey));
	m_mouse_click_button.Add(new int(button));
} /* TGLapp::MouseClick */ 



void TGLapp::string_editor_cycle(char *editing,unsigned int *editing_position,unsigned int max_length,KEYBOARDSTATE *k)
{
	SDL_keysym *ke;

	while(!k->keyevents.EmptyP()) {
		ke=k->keyevents.ExtractIni();
		if (ke->unicode>=0x20 && ke->unicode<=0x7E && strlen(editing)<max_length) {
			if ((*editing_position)<strlen(editing)) {
				for(unsigned int i=strlen(editing)+1;i>(*editing_position);i--) editing[i]=editing[i-1];
				editing[(*editing_position)]=(char)ke->unicode;
				(*editing_position)++;
			} else {
				editing[(*editing_position)]=(char)ke->unicode;
				(*editing_position)++;
				editing[(*editing_position)]=0;
			} /* if */ 
		} /* if */ 

		if (ke->sym==SDLK_BACKSPACE) {
			if ((*editing_position)>0) {
				strcpy(editing+(*editing_position)-1,editing+(*editing_position));
				(*editing_position)--;
			} /* if */ 
		} /* if */ 

		if (ke->sym==SDLK_DELETE) {
			if ((*editing_position)<strlen(editing)) {
				strcpy(editing+(*editing_position),editing+(*editing_position)+1);
			} /* if */ 
		} /* if */ 		

		if (ke->sym==SDLK_LEFT) {
			if ((*editing_position)>0) {
				(*editing_position)--;
			} /* if */ 
		} /* if */ 
		if (ke->sym==SDLK_RIGHT) {
			if ((*editing_position)<strlen(editing)) {
				(*editing_position)++;
			} /* if */ 
		} /* if */ 

		if (ke->sym==SDLK_HOME) (*editing_position)=0;
		if (ke->sym==SDLK_END) (*editing_position)=strlen(editing);
		
		delete ke;
	} /* while */ 
} /* TGLapp::string_editor_cycle */ 