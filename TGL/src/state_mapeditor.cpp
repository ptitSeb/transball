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

extern int SCREEN_X,SCREEN_Y;

int TGLapp::mapeditor_cycle(KEYBOARDSTATE *k)
{
	int old_mouse_x = m_mouse_x;
	int old_mouse_y = m_mouse_y;

	if (SDL_ShowCursor(SDL_QUERY)!=SDL_ENABLE) SDL_ShowCursor(SDL_ENABLE);

	SDL_GetMouseState(&m_mouse_x,&m_mouse_y);
	if (!m_mouse_click_x.EmptyP()) {
		m_mouse_button = *(m_mouse_click_button[0]);
		m_mouse_click_x.Delete();
		m_mouse_click_y.Delete();
		m_mouse_click_button.Delete();
	} else {
		m_mouse_button=0;
	} // if 


	if (m_state_cycle == 0){
		// CREATE THE INTERFACE:
		// - 11 is the tile browser for the tile mode

		TGLinterface::reset();
		TGLinterface::add_element(new TGLbutton("accept",m_font16,10,450,80,20,1));
		TGLinterface::add_element(new TGLbutton("cancel",m_font16,100,450,80,20,2));

		TGLinterface::add_element(new TGLbutton("load",m_font16,420,450,80,20,12));
		TGLinterface::add_element(new TGLbutton("save copy",m_font16,510,450,120,20,13));

		TGLinterface::add_element(new TGLbutton(m_GLTM->get("interface/leftarrow"),10,10,20,20,4));
		TGLinterface::add_element(new TGLbutton(m_GLTM->get("interface/rightarrow"),70,10,20,20,5));
		TGLinterface::add_element(new TGLbutton(m_GLTM->get("interface/leftarrow"),10,40,20,20,6));
		TGLinterface::add_element(new TGLbutton(m_GLTM->get("interface/rightarrow"),70,40,20,20,7));

		TGLinterface::add_element(new TGLbutton("in",m_font16,10,80,35,20,9));
		TGLinterface::add_element(new TGLbutton("out",m_font16,55,80,35,20,10));

		
		TGLinterface::add_element(new TGLbutton("background",m_font16,10,110,80,20,8));

		TGLinterface::add_element(new TGLbutton("mode",m_font16,10,140,80,20,3));
		m_editor_mode=0;

//		m_editor_focus_x=m_editor_level->m_map_data->get_dx()/2;
//		m_editor_focus_y=m_editor_level->m_map_data->get_dy()/2;
		m_editor_focus_x=0;
		m_editor_focus_y=STARFIELD;
		m_editor_zoom=2;
		m_editor_real_zoom=1.0f;
		m_editor_current_zoom=0.75f;

		m_editor_selected_tile=-1;

		m_editor_insert_x=-1;
		m_editor_insert_y=-1;

		if (m_editor_tiles.EmptyP()) {
			// Load all the tiles from "graphics\foreground"
			char *path="graphics/foreground";
#ifdef _WIN32
			/* Find files: */ 
			WIN32_FIND_DATA finfo;
			HANDLE h;
			char pattern[256],tmp[256];
			sprintf(pattern,"%s/*.*",path);

			h=FindFirstFile(pattern,&finfo);
			if (h!=INVALID_HANDLE_VALUE) {
				strcpy(tmp,finfo.cFileName);
				while(FindNextFile(h,&finfo)==TRUE) {
					if (finfo.cFileName[strlen(finfo.cFileName)-1]=='g' &&
						finfo.cFileName[strlen(finfo.cFileName)-2]=='n' &&
						finfo.cFileName[strlen(finfo.cFileName)-3]=='p' &&
						finfo.cFileName[strlen(finfo.cFileName)-4]=='.') {
						sprintf(tmp,"foreground/%s",finfo.cFileName);
						tmp[strlen(tmp)-4]=0;	// remove extension
						m_editor_tiles.Add(m_GLTM->get(tmp));
//						m_editor_tiles_name.Add(new Symbol(tmp));
					} /* if */ 
				} /* while */ 
			} /* if */ 
#else
			struct dirent **namelist;
			char tmp[512];
			int i,n;

			// get alpha sorted list of ".png" filenames from "path"
			if ((n=scandir(path, &namelist, filter_png, alphasort)) > 0) {
				for (i=0; i<n; i++) {
					snprintf(tmp, 512, "foreground/%s", namelist[i]->d_name);
					tmp[strlen(tmp)-4]=0;	// remove extension
					m_editor_tiles.Add(new GLTile(tmp));
//					m_editor_tiles_name.Add(new Symbol(tmp));
				}
				free(namelist);
			}
#endif  
		} // if 

	} else {
		// Create the variable part of the interface depending on the selected mode:
		switch(m_editor_mode) {
		case 0: TGLinterface::remove_element(11);
				break;

		case 1: // TILES
				{
					TGLTileBrowser *tb = (TGLTileBrowser *)TGLinterface::get(11);
					if (tb==0) {
						GLTile *t;
						tb = new TGLTileBrowser(10,180,70,256,11);
						TGLinterface::add_element(tb);

						m_editor_tiles.Rewind();
						while(m_editor_tiles.Iterate(t)) tb->addEntry(t);
					} // if

					m_editor_selected_tile = tb->getSelected();
				}

				break;

		case 2: TGLinterface::remove_element(11);
				break;

		case 3: TGLinterface::remove_element(11);
				break;

		} // switch

		// Insert position:
		{
			m_editor_insert_x = int((m_mouse_x-(SCREEN_X/2))/m_editor_current_zoom + (m_editor_focus_x));
			m_editor_insert_y = int((m_mouse_y-(SCREEN_Y/2))/m_editor_current_zoom + (m_editor_focus_y));

			m_editor_insert_x = (m_editor_insert_x/32)*32;
			m_editor_insert_y = ((m_editor_insert_y-STARFIELD)/32)*32;

			if (m_editor_insert_x<0 || m_editor_insert_y<0 ||
				m_editor_insert_x>=m_editor_level->m_map_data->get_dx() ||
				m_editor_insert_y>=m_editor_level->m_map_data->get_dy()) {
				m_editor_insert_x = -1;
				m_editor_insert_y = -1;
			} // if
		}

		// Zoom:
		if (k->key_press(SDLK_PAGEDOWN)) {
			m_editor_zoom--;
			if (m_editor_zoom<0) m_editor_zoom=0;
			m_editor_real_zoom = 4;
			for(int i=0;i<m_editor_zoom;i++) m_editor_real_zoom/=2.0f;
		} // if

		if (k->key_press(SDLK_PAGEUP)) {
			m_editor_zoom++;
			if (m_editor_zoom>6) m_editor_zoom=6;
			m_editor_real_zoom = 4;
			for(int i=0;i<m_editor_zoom;i++) m_editor_real_zoom/=2.0f;
		} // if

		m_editor_current_zoom = (m_editor_current_zoom*7 + m_editor_real_zoom)/8.0f;

		// Move around:
		if (k->keyboard[SDLK_LEFT] || m_mouse_x<8) {
			m_editor_focus_x-=int(8/m_editor_real_zoom);
			if (m_editor_focus_x<0) m_editor_focus_x=0;
		} // if
		if (k->keyboard[SDLK_RIGHT] || m_mouse_x>=SCREEN_X-8) {
			m_editor_focus_x+=int(8/m_editor_real_zoom);
			if (m_editor_focus_x>m_editor_level->m_map_data->get_dx()) m_editor_focus_x=m_editor_level->m_map_data->get_dx();
		} // if
		if (k->keyboard[SDLK_UP] || m_mouse_y<8) {
			m_editor_focus_y-=int(8/m_editor_real_zoom);
			if (m_editor_focus_y<0) m_editor_focus_y=0;
		} // if
		if (k->keyboard[SDLK_DOWN] || m_mouse_y>=SCREEN_Y-8) {
			m_editor_focus_y+=int(8/m_editor_real_zoom);	
			if (m_editor_focus_y>m_editor_level->m_map_data->get_dy()) m_editor_focus_y=m_editor_level->m_map_data->get_dy();		
		} // if

//		if (m_editor_focus_x*m_editor_real_zoom < (SCREEN_X/2)) m_editor_focus_x = int((SCREEN_X/2)/m_editor_real_zoom);
//		if (m_editor_focus_y*m_editor_real_zoom < (SCREEN_Y/2)) m_editor_focus_y = int((SCREEN_Y/2)/m_editor_real_zoom);

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
				if (m_editor_focus_x>m_editor_level->m_map_data->get_dx()) m_editor_focus_x=m_editor_level->m_map_data->get_dx();
				if (m_editor_focus_y>m_editor_level->m_map_data->get_dy()) m_editor_focus_y=m_editor_level->m_map_data->get_dy();
				break;
		case 5: if (m_editor_level->m_map_data->m_fg_dx<128) m_editor_level->m_map_data->resize(m_editor_level->m_map_data->m_fg_dx+1,m_editor_level->m_map_data->m_fg_dy,m_GLTM);
				if (m_editor_focus_x>m_editor_level->m_map_data->get_dx()) m_editor_focus_x=m_editor_level->m_map_data->get_dx();
				if (m_editor_focus_y>m_editor_level->m_map_data->get_dy()) m_editor_focus_y=m_editor_level->m_map_data->get_dy();
				break;
		case 6: if (m_editor_level->m_map_data->m_fg_dy>16) m_editor_level->m_map_data->resize(m_editor_level->m_map_data->m_fg_dx,m_editor_level->m_map_data->m_fg_dy-1,m_GLTM);
				if (m_editor_focus_x>m_editor_level->m_map_data->get_dx()) m_editor_focus_x=m_editor_level->m_map_data->get_dx();
				if (m_editor_focus_y>m_editor_level->m_map_data->get_dy()) m_editor_focus_y=m_editor_level->m_map_data->get_dy();
				break;
		case 7: if (m_editor_level->m_map_data->m_fg_dy<128) m_editor_level->m_map_data->resize(m_editor_level->m_map_data->m_fg_dx,m_editor_level->m_map_data->m_fg_dy+1,m_GLTM);
				if (m_editor_focus_x>m_editor_level->m_map_data->get_dx()) m_editor_focus_x=m_editor_level->m_map_data->get_dx();
				if (m_editor_focus_y>m_editor_level->m_map_data->get_dy()) m_editor_focus_y=m_editor_level->m_map_data->get_dy();
				break;
		case 8: {
					int bg = m_editor_level->m_map_data->m_bg_code+1;
					if (bg>4) bg=0;
					m_editor_level->m_map_data->set_background(bg,m_GLTM);
				}
				break;
		case 9:
				{
					m_editor_zoom--;
					if (m_editor_zoom<0) m_editor_zoom=0;
					m_editor_real_zoom = 4;
					for(int i=0;i<m_editor_zoom;i++) m_editor_real_zoom/=2.0f;
				}
				break;
		case 10:
				{
					m_editor_zoom++;
					if (m_editor_zoom>6) m_editor_zoom=6;
					m_editor_real_zoom = 4;
					for(int i=0;i<m_editor_zoom;i++) m_editor_real_zoom/=2.0f;
				}
				break;		

		case 12:

				break;

		case 13:
				break;
		} // switch

		// Button presse (but not over any interface element);
		if (ID==-1 && m_mouse_button==1) {
			switch(m_editor_mode) {
			case 0:
					break;
			case 1:
					{
						if (m_editor_selected_tile!=-1 &&
							m_editor_insert_x!=-1) {
							int offs = m_editor_insert_x/32 + (m_editor_insert_y/32)*m_editor_level->m_map_data->m_fg_dx;
							m_editor_level->m_map_data->m_fg[offs]=m_editor_tiles[m_editor_selected_tile];
						} // if
					}
					break;
			case 2:
					break;
			case 3:
					break;
			} // if
		} // if

		// Right button presse (but not over any interface element);
		if ((ID==-1 && m_mouse_button==3) || k->key_press(SDLK_DELETE)) {
			switch(m_editor_mode) {
			case 0:
					break;
			case 1:
					{
						if (m_editor_selected_tile!=-1 &&
							m_editor_insert_x!=-1) {
							int offs = m_editor_insert_x/32 + (m_editor_insert_y/32)*m_editor_level->m_map_data->m_fg_dx;
							m_editor_level->m_map_data->m_fg[offs]=0;
						} // if
					}
					break;
			case 2:
					break;
			case 3:
					break;
			} // if
		} // if
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

		m->draw(m_editor_focus_x,m_editor_focus_y,640,480,m_GLTM,m_editor_current_zoom);
	} // if 

	// Draw extras:
	{
		char tmp[80];

		sprintf(tmp,"%i",m_editor_level->m_map_data->m_fg_dx);
		TGLinterface::print_center(tmp,m_font16,50,32);
		sprintf(tmp,"%i",m_editor_level->m_map_data->m_fg_dy);
		TGLinterface::print_center(tmp,m_font16,50,62);	
	}

	switch(m_editor_mode) {
	case 0: TGLinterface::print_center("intelligent",m_font16,50,180);
			break;

	case 1: TGLinterface::print_center("tiles",m_font16,50,180);

			if (m_editor_insert_x!=-1) {
				float x,y;

				x = ((m_editor_insert_x-m_editor_focus_x)*m_editor_current_zoom) + (SCREEN_X/2);
				y = ((m_editor_insert_y+STARFIELD-m_editor_focus_y)*m_editor_current_zoom) + (SCREEN_Y/2);
			
				glPushMatrix();
				glTranslatef(x,y,0);
				glBegin(GL_LINES);
				glColor4f(1,1,1,1);
				glVertex3f(0,0,0);
				glVertex3f(32*m_editor_current_zoom,0,0);
				glVertex3f(0,32*m_editor_current_zoom,0);
				glVertex3f(32*m_editor_current_zoom,32*m_editor_current_zoom,0);
				glVertex3f(0,0,0);
				glVertex3f(0,32*m_editor_current_zoom,0);
				glVertex3f(32*m_editor_current_zoom,0,0);
				glVertex3f(32*m_editor_current_zoom,32*m_editor_current_zoom,0);
				glEnd();
				glPopMatrix();
			} // if

			break;

	case 2: TGLinterface::print_center("objects",m_font16,50,180);
			break;

	case 3: TGLinterface::print_center("properties",m_font16,50,180);
			break;
	} // switch
	

	TGLinterface::draw();

} /* TheGooniesApp::mapeditor_draw */ 

