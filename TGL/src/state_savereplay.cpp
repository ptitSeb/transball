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



int TGLapp::savereplay_cycle(KEYBOARDSTATE *k)
{
	if (SDL_ShowCursor(SDL_QUERY)!=SDL_ENABLE) SDL_ShowCursor(SDL_ENABLE);
	if (m_state_cycle==0) {
		TGLinterface::reset();
		SDL_WarpMouse(210,352);
		TGLinterface::add_element(new TGLframe(100,32,440,276));
		TGLinterface::add_element(new TGLframe(100,340,440,32));
		TGLinterface::add_element(new TGLbutton("Save",m_font32,110,404,200,64,0));
		TGLinterface::add_element(new TGLbutton("Cancel",m_font32,330,404,200,64,1));

		// Find an initial name for the replay:
		{
			int i=0;
			FILE *fp;
			char tmp[255];
			bool found=false;

			do {
				if (i==0) {
					sprintf(m_replay_name,"replay.rpl");
				} else {
					sprintf(m_replay_name,"replay_%i.rpl",i);
				} // if 

				sprintf(tmp,"replays/%s",m_replay_name);
				fp=fopen(tmp,"r+");
				if (fp!=0) {
					fclose(fp);
					found=true;
				} else {
					found=false;
				} // if 
			} while(found);
		}
		m_replay_editing_position=strlen(m_replay_name);
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
			m_state_fading=2;
			m_state_fading_cycle=0;
			m_state_selection=ID;
		} // if 

		// Edit the string:
		string_editor_cycle(m_replay_name,&m_replay_editing_position,32,k);
	}

	if (m_state_fading==2 && m_state_fading_cycle>25) {
		switch(m_state_selection) {
		case 0:
				// save the replay
				// ...
				return TGL_STATE_POSTGAME;
				break;
		case 1:
				return TGL_STATE_POSTGAME;
				break;
		} // switch
	} // if 

	if (m_state_fading==0 || m_state_fading==2) m_state_fading_cycle++;
	if (m_state_fading==0 && m_state_fading_cycle>25) m_state_fading=1;

	return TGL_STATE_SAVEREPLAY;
} /* TGLapp::savereplay_cycle */ 

void TGLapp::savereplay_draw(void)
{
//	char buffer[255];

	glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

	TGLinterface::draw();

	// show the list of files:
	// ...

	// File name:
	{
		TGLinterface::print_left(m_replay_name,m_font16,108,366);
		// draw the cursor:
		{
			char tmp[255];
			int tdx=0,tdy=0;

			strcpy(tmp,m_replay_name);
			tmp[m_replay_editing_position]=0;
			TTF_SizeText(m_font16,tmp,&tdx,&tdy);

			{
				float f;
				f=float(0.5+0.3*sin((m_state_cycle)*0.3F));
				glColor4f(1,0,0,f);
				glBegin(GL_POLYGON);
				glVertex3f(float(108+tdx),348,0);
				glVertex3f(float(108+tdx+4),348,0);
				glVertex3f(float(108+tdx+4),364,0);
				glVertex3f(float(108+tdx),364,0);
				glEnd();

			}
		}
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
} /* TGLapp::savereplay_draw */ 

