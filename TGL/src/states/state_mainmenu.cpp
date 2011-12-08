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

#include "gl.h"
#include "glu.h"
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


int TGLapp::mainmenu_cycle(KEYBOARDSTATE *k)
{
	if (SDL_ShowCursor(SDL_QUERY)!=SDL_ENABLE) SDL_ShowCursor(SDL_ENABLE);

	if (m_state_cycle==0) {
		TGLInterfaceElement *e;

		m_mm_demo_state=0;
		m_mm_demo_timmer=0;

		TGLinterface::reset();
		SDL_WarpMouse(320,315);
		TGLinterface::add_element(new TGLbuttonTransparent("PLAY",m_ifont32,240,305,160,30,0));

		e=new TGLbuttonTransparent("OPTIONS",m_ifont32,150,350,160,30,2);
		TGLinterface::add_element(e);

		e=new TGLbuttonTransparent("EDITOR",m_ifont32,330,350,160,30,6);
		TGLinterface::add_element(e);

		e=new TGLbuttonTransparent("HIGH SCORES",m_ifont32,150,395,160,30,3);
		TGLinterface::add_element(e);
		TGLinterface::add_element(new TGLbuttonTransparent("REPLAYS",m_ifont32,330,395,160,30,4));
		TGLinterface::add_element(new TGLbuttonTransparent("QUIT",m_ifont32,240,440,160,30,5));
	} // if 

	{
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

		if (ID!=-1 && m_state_fading!=2) {
			m_state_fading=2;
			m_state_fading_cycle=0;
			m_state_selection=ID;
		} // if 
	} // if 


	if (m_state_fading==2 && m_state_fading_cycle>25) {
		SDL_ShowCursor(SDL_DISABLE);
		switch(m_state_selection) {
		case 0: 
				if (m_current_levelpack!=0) delete m_current_levelpack;
				{
					FILE *fp;
					fp=fopen(get_player_profile()->get_last_levelpack(),"rb");
					if (fp!=0) {
						m_current_levelpack=new LevelPack(fp,m_GLTM);
						fclose(fp);
						
						return TGL_STATE_LEVELPACKSCREEN;
					} else {
						return TGL_STATE_NONE;
					} // if 
				} 

				return TGL_STATE_MAINMENU;
				break;
		case 2: 
				return TGL_STATE_CONFIGURE;
				break;
		case 3: 
				return TGL_STATE_HIGHSCORES;
				break;
		case 4: 
				return TGL_STATE_REPLAYBROWSER;
				break;
		case 6: 
				return TGL_STATE_EDITOR;
				break;
		case 5: 
				return TGL_STATE_NONE;
				break;
		} // switch
	} // if 

	if (m_state_fading==0 || m_state_fading==2) m_state_fading_cycle++;
	if (m_state_fading==0 && m_state_fading_cycle>25) m_state_fading=1;

	if (m_mm_demo_state==0) m_mm_demo_timmer++;
	if (m_mm_demo_state==0 && m_mm_demo_timmer>100) {
		// load demo
		char tmp[256];

		if (m_mm_game!=0) delete m_mm_game;
		m_mm_game=0;
		if (m_mm_replay!=0) delete m_mm_replay;
		m_mm_replay=0;

		sprintf(tmp,"tutorials/demo%i.rpl",rand()%5);
		char *m_mm_demo_name=new char[strlen(tmp)+1];
		strcpy(m_mm_demo_name,tmp);

		FILE *fp;
		fp=fopen(m_mm_demo_name,"rb");
		if (fp!=0) {
			m_mm_replay=new TGLreplay(fp);
			fclose(fp);
			m_mm_game=new TGL(m_mm_replay->get_map(),
						  	  m_mm_replay->get_playership("default"),
							  m_mm_replay->get_initial_fuel(),
							  0,
							  0,m_GLTM);
			m_mm_game->reset();
		} // if 

		m_mm_demo_state=1;
		m_mm_demo_timmer=0;
	} // if 

	if (m_mm_demo_state==1 || m_mm_demo_state==2 || m_mm_demo_state==3) {
		if (m_mm_replay!=0) {
	
			m_mm_demo_timmer++;

			List<TGLobject> *l=m_mm_game->get_map()->get_objects("TGLobject");
			List<TGLobject> to_delete,to_add;
			TGLobject *o;
			bool retval;
			retval = m_mm_replay->execute_cycle(&m_lvc,l,&to_delete,&to_add);
			l->ExtractAll();
			delete l;

			while(!to_delete.EmptyP()) {
				o=to_delete.ExtractIni();
				m_mm_game->get_map()->remove_object(o);
				delete o;
			} // while 

			while(!to_add.EmptyP()) {
				o=to_add.ExtractIni();
				m_mm_game->get_map()->add_object(o);
				delete o;
			} // while 

			if (!m_mm_game->cycle(&m_lvc,m_GLTM,m_SFXM,0)) retval=false;
			
			if (!retval) {
				delete m_mm_game;
				delete m_mm_replay;
				m_mm_replay=0;

				m_mm_demo_state=0;
				m_mm_demo_timmer=0;
			} // if  
		}

		if (m_mm_demo_state==1 && m_mm_demo_timmer>100) {
			m_mm_demo_state=2;
			m_mm_demo_timmer=0;
		} // if 

	} // if 
	if (m_mm_demo_state==2 && m_mm_demo_timmer>(m_mm_replay->get_length()-500)) {
		// stop demo
		m_mm_demo_state=3;
		m_mm_demo_timmer=0;
	} // if 
	if (m_mm_demo_state==3 && m_mm_demo_timmer>100) {
		// stop demo
		m_mm_demo_state=0;
		m_mm_demo_timmer=0;
	} // if 


	return TGL_STATE_MAINMENU;
} /* TGLapp::mainmenu_cycle */ 


void TGLapp::mainmenu_draw(void)
{
	glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

	if (m_mm_game!=0) {

		switch(m_mm_demo_state) {
		case 0:
				m_GLTM->get_smooth("interface/gl")->draw(1,1,1,1,320,240,0,0,1);
				m_GLTM->get_smooth("interface/gl-inverted")->draw(1,1,1,1,320,240,0,0,1);
				m_GLTM->get_smooth("interface/transball")->draw(1,1,1,1,320,120,0,0,0.5f);
				break;
		case 1:
				{
					float f=m_mm_demo_timmer/100.0f;

					if (f>1) f=1;
					if (f<0) f=0;
					m_mm_game->draw(m_GLTM);
					m_GLTM->get_smooth("interface/gl")->draw(1,1,1,1*(1-f),320,240,0,0,1);
					m_GLTM->get_smooth("interface/gl-inverted")->draw(1,1,1,1*(1-f)+0.85f*f,320,240,0,0,1);
					m_GLTM->get_smooth("interface/transball")->draw(1,1,1,1,320,120,0,0,0.5f);
				}
				break;
		case 2:
				m_mm_game->draw(m_GLTM);
				m_GLTM->get_smooth("interface/gl-inverted")->draw(1,1,1,0.85f,320,240,0,0,1);
				m_GLTM->get_smooth("interface/transball")->draw(1,1,1,1,320,120,0,0,0.5f);
				break;
		case 3:
				{
					float f=(100-m_mm_demo_timmer)/100.0f;

					if (f>1) f=1;
					if (f<0) f=0;
					m_mm_game->draw(m_GLTM);
					m_GLTM->get_smooth("interface/gl")->draw(1,1,1,1*(1-f),320,240,0,0,1);
					m_GLTM->get_smooth("interface/gl-inverted")->draw(1,1,1,1*(1-f)+0.85f*f,320,240,0,0,1);
					m_GLTM->get_smooth("interface/transball")->draw(1,1,1,1,320,120,0,0,0.5f);
				}
				break;
		} // switch
	} else {
		m_GLTM->get_smooth("interface/gl")->draw(1,1,1,1,320,240,0,0,1);
		m_GLTM->get_smooth("interface/gl-inverted")->draw(1,1,1,1,320,240,0,0,1);
		m_GLTM->get_smooth("interface/transball")->draw(1,1,1,1,320,120,0,0,0.5f);
	} // if 


	if (m_state_cycle<50) {
		TGLinterface::draw(m_state_cycle/50.0f);
	} else {
		TGLinterface::draw();
	} // if 

	switch(m_state_fading) {
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

} /* TGLapp::mainmenu_draw */ 

