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

	m_font32=TTF_OpenFont("fonts/arial.ttf",32);
	m_font16=TTF_OpenFont("fonts/arial.ttf",16);

	m_current_levelpack=0;
	m_selected_level=0;
	m_selected_ship=0;

	m_state=TGL_STATE_GAMESTART;
	m_state_cycle=0;
	m_state_fading=0;
	m_state_fading_cycle=0;
	m_state_selection=0;

	m_game_state=0;
	m_game_fade_effect=-1;
	m_game_replay_mode=0;	
	m_game_replay=0;

	m_replay_name[0]=0;
	m_replay_editing_position=0;

	m_screen_dx=640;
	m_screen_dy=480;
	fullscreen=false;
	
	m_GLTM=new GLTManager();
	m_SFXM=new SFXManager();
	m_SFXM->cache("sfx");

	m_n_players=1;
	m_keys_configuration[0][KEY_THRUST]=SDLK_q;
	m_keys_configuration[0][KEY_SPECIAL]=SDLK_a;
	m_keys_configuration[0][KEY_LEFT]=SDLK_o;
	m_keys_configuration[0][KEY_RIGHT]=SDLK_p;
	m_keys_configuration[0][KEY_FIRE]=SDLK_SPACE;
	m_keys_configuration[0][KEY_ATTRACTOR]=SDLK_RETURN;
	m_keys_configuration[0][KEY_PAUSE]=SDLK_F1;
	m_keys_configuration[0][KEY_QUIT]=SDLK_ESCAPE;
	
	m_sfx_volume=MIX_MAX_VOLUME;
	m_music_volume=96;
	m_game=0;

	for(i=0;i<m_n_players;i++) {
		VirtualController *vc = new VirtualController();
		vc->reset();
		m_lvc.Add(vc);
	} // for 
	m_test_game=0;
	load_configuration();
} /* TGLapp::TGLapp */ 


TGLapp::~TGLapp()
{
	TTF_CloseFont(m_font32);
	TTF_CloseFont(m_font16);

	if (m_current_levelpack!=0) delete m_current_levelpack;
	m_current_levelpack=0;

	if (m_game!=0) delete m_game;
	m_game=0;

	if (m_game_replay!=0) delete m_game_replay;
	m_game_replay=0;

	delete m_GLTM;
	delete m_SFXM;

	save_configuration();
} /* TGLapp::TGLapp */ 


bool TGLapp::cycle(KEYBOARDSTATE *k)
{
	int old_state=m_state;

  
#ifdef __DEBUG_MESSAGES
	if (state_cycle==0) {
		output_debug_message("First Cycle started for state %i...\n",m_state);
	} /* if */ 
#endif

	switch(m_state) {
//	case TGL_STATE_INTRO:m_state=intro_cycle(k);
//								break;
//	case TGL_STATE_TITLE:m_state=title_cycle(k);
//								break;
	case TGL_STATE_GAMESTART:m_state=gamestart_cycle(k);
									break;
	case TGL_STATE_LEVELPACKSCREEN:m_state=levelpackscreen_cycle(k);
									break;
	case TGL_STATE_PREGAME:m_state=pregame_cycle(k);
							   break;
	case TGL_STATE_GAME:m_state=game_cycle(k);
							   break;
	case TGL_STATE_POSTGAME:m_state=postgame_cycle(k);
							   break;
	case TGL_STATE_SAVEREPLAY: m_state=savereplay_cycle(k);
							   break;
//	case TGL_STATE_GAMEOVER:m_state=gameover_cycle(k);
//								   break;
//	case TGL_STATE_ENDSEQUENCE:m_state=endsequence_cycle(k);
//								      break;
//	case TGL_STATE_INTERLEVEL:m_state=interlevel_cycle(k);
//									 break;
//	case TGL_STATE_HOWTOPLAY:m_state=howtoplay_cycle(k);
//									break;
//	case TGL_STATE_CREDITS  :m_state=credits_cycle(k);
//									break;
	default:return false;
	} /* switch */ 

	if (old_state==m_state) {
		m_state_cycle++;
	} else {
		m_state_cycle=0;
		m_state_fading=0;
		m_state_fading_cycle=0;

#ifdef __DEBUG_MESSAGES
		output_debug_message("State change: %i -> %i\n",old_state,m_state);
#endif
	} /* if */ 

	m_SFXM->next_cycle();

	return true;
} /* TGLapp::cycle */ 


void TGLapp::draw(int SCREEN_X,int SCREEN_Y)
{
	float lightpos[4]={0,0,-1000,0};
	float tmpls[4]={1.0F,1.0F,1.0F,1.0};
	float tmpld[4]={1.0F,1.0F,1.0F,1.0};
	float tmpla[4]={1.0F,1.0F,1.0F,1.0};
    float ratio;

	m_screen_dx=SCREEN_X;
	m_screen_dy=SCREEN_Y;

#ifdef __DEBUG_MESSAGES
	if (state_cycle==0) {
		output_debug_message("First Drawing cycle started for state %i...\n",state);
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
//	case TGL_STATE_INTRO:intro_draw();
//								break;
//	case TGL_STATE_TITLE:title_draw();
//								break;
	case TGL_STATE_GAMESTART:gamestart_draw();
									break;
	case TGL_STATE_LEVELPACKSCREEN:levelpackscreen_draw();
									break;
	case TGL_STATE_PREGAME:pregame_draw();
							   break;
	case TGL_STATE_GAME:game_draw();
							   break;
	case TGL_STATE_POSTGAME:postgame_draw();
							   break;
	case TGL_STATE_SAVEREPLAY: savereplay_draw();
							   break;
//	case TGL_STATE_GAMEOVER:gameover_draw();
//								   break;
//	case TGL_STATE_ENDSEQUENCE:endsequence_draw();
//								      break;
//	case TGL_STATE_INTERLEVEL:interlevel_draw();
//									 break;
//	case TGL_STATE_HOWTOPLAY: howtoplay_draw();
//									 break;
//	case TGL_STATE_CREDITS:   credits_draw();
//									 break;
	} // switch 
 	

	if (show_fps) {
		char tmp[80];
		sprintf(tmp,"video mem: %.4gmb - fps: %i",float(GLTile::get_memory_used())/float(1024*1024),frames_per_sec);
//		font_print_c(320,460,0,0,0.5f,"font",tmp,-2);
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


void TGLapp::save_configuration(void)
{
	int i;
	FILE *fp;
	char cfg[255];
#ifdef _WIN32
	sprintf(cfg, "TGL.cfg");
#else
	snprintf(cfg, 255, "%s/.TGL.cfg", getenv("HOME"));
#endif //_WIN32

	fp=fopen(cfg,"w");
	if (fullscreen) fprintf(fp,"fullscreen\n");
			   else fprintf(fp,"windowed\n");
	fprintf(fp,"%i %i\n",m_sfx_volume,m_music_volume);
	for(i=0;i<8;i++) fprintf(fp,"%i ",m_keys_configuration[0][i]);
	fprintf(fp,"\n");
	fclose(fp);
} /* TGLapp::save_configuration */ 


void TGLapp::load_configuration(void)
{
	FILE *fp;
	char cfg[255];
#ifdef _WIN32
	sprintf(cfg, "TGL.cfg");
#else
	snprintf(cfg, 255, "%s/.TGL.cfg", getenv("HOME"));
#endif //_WIN32

	fp=fopen(cfg,"r");
	if (fp==0) {
		save_configuration();
	} else {
		char tmp_s[80];
		int i;

		// fullscreen / windowed
		fscanf(fp,"%s",tmp_s);
		if (strcmp(tmp_s,"fullscreen")==0) fullscreen=true;
									  else fullscreen=false;
		// volumes
		fscanf(fp,"%i %i",&m_sfx_volume,&m_music_volume);

		// keyboard
		for(i=0;i<8;i++) fscanf(fp,"%i",&(m_keys_configuration[0][i]));

		fclose(fp);
	} // if 
} /* TGLapp::load_configuration */ 


void TGLapp::MouseClick(int mousex,int mousey) 
{
	m_mouse_click_x.Add(new int(mousex));
	m_mouse_click_y.Add(new int(mousey));
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