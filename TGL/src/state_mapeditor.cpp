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


int TGLapp::mapeditor_cycle(KEYBOARDSTATE *k)
{
	int old_mouse_x = m_mouse_x;
	int old_mouse_y = m_mouse_y;

	if (SDL_ShowCursor(SDL_QUERY)!=SDL_ENABLE) SDL_ShowCursor(SDL_ENABLE);

	SDL_GetMouseState(&m_mouse_x,&m_mouse_y);
	if (!m_mouse_click_x.EmptyP()) {
		m_mouse_click_x.Delete();
		m_mouse_click_y.Delete();
		m_mouse_button=1;
	} else {
		m_mouse_button=0;
	} // if 


	if (m_state_cycle == 0){
		// CREATE THE INTERFACE:
		TGLinterface::reset();
		TGLinterface::add_element(new TGLbutton("accept",m_font16,10,450,80,20,1));
		TGLinterface::add_element(new TGLbutton("cancel",m_font16,100,450,80,20,2));

		TGLinterface::add_element(new TGLbutton(m_GLTM->get("interface/leftarrow"),10,10,20,20,4));
		TGLinterface::add_element(new TGLbutton(m_GLTM->get("interface/rightarrow"),70,10,20,20,5));
		TGLinterface::add_element(new TGLbutton(m_GLTM->get("interface/leftarrow"),10,40,20,20,6));
		TGLinterface::add_element(new TGLbutton(m_GLTM->get("interface/rightarrow"),70,40,20,20,7));
		TGLinterface::add_element(new TGLbutton("background",m_font16,10,80,80,20,8));

		TGLinterface::add_element(new TGLbutton("mode",m_font16,10,110,80,20,3));
		m_editor_mode=0;

		m_editor_focus_x=0;
		m_editor_focus_y=0;
		m_editor_zoom=1;
		m_editor_old_zoom=1;
		m_editor_zoom_timmer=0;

	} else {
		// Create the variable part of the interface depending on the selected mode:
		switch(m_editor_mode) {
		case 0: 
				break;

		case 1: 
				break;

		case 2: 
				break;

		case 3: 
				break;

		} // switch


	} // if 

	{
		int button_status=0;
		int ID=-1;
		button_status=SDL_GetMouseState(&m_mouse_x,&m_mouse_y);

		if (k->key_press(SDLK_SPACE) || k->key_press(SDLK_RETURN)) m_mouse_button=1;

		ID = TGLinterface::update_state(m_mouse_x,m_mouse_y,m_mouse_button,button_status,k);

		switch(ID) {
		case 1:	// ACCEPT:
				return TGL_STATE_EDITOR;
				break;
		case 2:	// CANCEL:
				return TGL_STATE_EDITOR;
				break;

		case 3:	// MODE
				m_editor_mode++;
				if (m_editor_mode>3) m_editor_mode=0;
				break;

		case 4: if (m_editor_level->m_map_data->m_fg_dx>16) m_editor_level->m_map_data->resize(m_editor_level->m_map_data->m_fg_dx-1,m_editor_level->m_map_data->m_fg_dy,m_GLTM);
				break;
		case 5: if (m_editor_level->m_map_data->m_fg_dx<128) m_editor_level->m_map_data->resize(m_editor_level->m_map_data->m_fg_dx+1,m_editor_level->m_map_data->m_fg_dy,m_GLTM);
				break;
		case 6: if (m_editor_level->m_map_data->m_fg_dy>16) m_editor_level->m_map_data->resize(m_editor_level->m_map_data->m_fg_dx,m_editor_level->m_map_data->m_fg_dy-1,m_GLTM);
				break;
		case 7: if (m_editor_level->m_map_data->m_fg_dy<128) m_editor_level->m_map_data->resize(m_editor_level->m_map_data->m_fg_dx,m_editor_level->m_map_data->m_fg_dy+1,m_GLTM);
				break;
		case 8: {
					int bg = m_editor_level->m_map_data->m_bg_code+1;
					if (bg>4) bg=0;
					m_editor_level->m_map_data->set_background(bg,m_GLTM);
				}
				break;

		} // switch
	}


    return TGL_STATE_MAPEDITOR;
} /* TheGooniesApp::mapeditor_cycle */ 

void TGLapp::mapeditor_draw(void)
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

	// Draw level:
	if (m_editor_level!=0) {
		TGLmap *m = m_editor_level->m_map_data;

		m->draw(m_editor_focus_x,m_editor_focus_y,640,480,m_GLTM);
	} // if 

	// Draw extras:
	{
		char tmp[80];

		sprintf(tmp,"%i",m_editor_level->m_map_data->m_fg_dx);
		TGLinterface::print_center(tmp,m_font16,50,30);
		sprintf(tmp,"%i",m_editor_level->m_map_data->m_fg_dy);
		TGLinterface::print_center(tmp,m_font16,50,60);
	}
	switch(m_editor_mode) {
	case 0: TGLinterface::print_center("intelligent",m_font16,50,150);
			break;

	case 1: TGLinterface::print_center("tiles",m_font16,50,150);
			break;

	case 2: TGLinterface::print_center("objects",m_font16,50,150);
			break;

	case 3: TGLinterface::print_center("properties",m_font16,50,150);
			break;
	} // switch

	
	


	TGLinterface::draw();

} /* TheGooniesApp::mapeditor_draw */ 

