#ifdef KITSCHY_DEBUG_MEMORY 
#include "debug_memorymanager.h"
#endif

#ifdef _WIN32
#include "windows.h"
#else
#include <stddef.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <dirent.h>
#include "ctype.h"
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
#include "TGLinterface.h"

#include "LevelPack.h"
#include "PlayerProfile.h"

#include "TGLmap.h"
#include "TGLobject.h"
#include "TGLobject_ballstand.h"
#include "TGLobject_redlight.h"
#include "TGLobject_radar.h"
#include "TGLobject_pipevscreen.h"
#include "TGLobject_pipehscreen.h"
#include "TGLobject_enemy.h"
#include "TGLobject_laser_left.h"
#include "TGLobject_laser_right.h"
#include "TGLobject_laser_up.h"
#include "TGLobject_laser_down.h"
#include "TGLobject_laser_horizontal.h"
#include "TGLobject_laser_vertical.h"
#include "TGLobject_cannon_left.h"
#include "TGLobject_cannon_right.h"
#include "TGLobject_cannon_down.h"
#include "TGLobject_cannon_up.h"
#include "TGLobject_fastcannon_left.h"
#include "TGLobject_fastcannon_right.h"
#include "TGLobject_fastcannon_down.h"
#include "TGLobject_fastcannon_up.h"
#include "TGLobject_ball.h"
#include "TGLobject_spike_left.h"
#include "TGLobject_spike_right.h"
#include "TGLobject_fuelrecharge.h"
#include "TGLobject_techno_computer.h"
#include "TGLobject_directionalcannon_left.h"
#include "TGLobject_directionalcannon_right.h"
#include "TGLobject_directionalcannon_up.h"
#include "TGLobject_directionalcannon_down.h"
#include "TGLobject_ha_directionalcannon_left.h"
#include "TGLobject_ha_directionalcannon_right.h"
#include "TGLobject_ha_directionalcannon_up.h"
#include "TGLobject_ha_directionalcannon_down.h"
#include "TGLobject_fast_directionalcannon_left.h"
#include "TGLobject_fast_directionalcannon_right.h"
#include "TGLobject_fast_directionalcannon_up.h"
#include "TGLobject_fast_directionalcannon_down.h"
#include "TGLobject_tank.h"
#include "TGLobject_tank_turret.h"
#include "TGLobject_tank_cannon.h"
#include "TGLobject_big_tank.h"
#include "TGLobject_big_tank_cannon.h"
#include "TGLobject_leftdoor.h"
#include "TGLobject_rightdoor.h"
#include "TGLobject_button.h"

extern int SCREEN_X,SCREEN_Y;

#ifndef _WIN32
int filter_png(struct direct *entry)
{	
	if ((strcmp(entry->d_name, ".")== 0) ||
		(strcmp(entry->d_name, "..") == 0))
		return (0);
	
	/* Check for filename extensions */
	if (strlen(entry->d_name)>4 &&
		entry->d_name[strlen(entry->d_name)-1]=='g' &&
		entry->d_name[strlen(entry->d_name)-2]=='n' &&
		entry->d_name[strlen(entry->d_name)-3]=='p' &&
		entry->d_name[strlen(entry->d_name)-4]=='.') 
		return (1);
	else
		return(0);
}
#endif

int TGLapp::mapeditor_cycle(KEYBOARDSTATE *k)
{
//	int old_mouse_x = m_mouse_x;
//	int old_mouse_y = m_mouse_y;

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


	if (m_state_cycle == 0) {
		if (m_editor_level_editing==0) {
			FILE *fp = fopen("tmp.map","w+");
			if (fp!=0) {
				m_editor_level->m_map_data->save(fp,m_GLTM);
				fclose(fp);
				fp = fopen("tmp.map","r+");
				m_editor_level_editing = new TGLmap(fp,m_GLTM);
				fclose(fp);
			} else {
				m_editor_level_editing = new TGLmap(m_GLTM);
			} // if 
		}	

		// CREATE THE INTERFACE:
		// - 11 is the tile browser for the tile mode
		// - 14 is the tile browser for the object mode
		// - 17 is the tile browser for the smart mode

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
		
		TGLinterface::add_element(new TGLbutton("help",m_font16,550,10,80,20,16));
		m_editor_mode=1;

//		m_editor_focus_x=m_editor_level_editing->get_dx()/2;
//		m_editor_focus_y=m_editor_level_editing->get_dy()/2;
		m_editor_focus_x=0;
		m_editor_focus_y=STARFIELD;
		m_editor_zoom=2;
		m_editor_real_zoom=1.0f;
		m_editor_current_zoom=0.75f;

		m_editor_selected_tile=-1;
		m_editor_selected_object=-1;

		m_editor_insert_x=-1;
		m_editor_insert_y=-1;

		m_mb_object_under_pointer=0;

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
					m_editor_tiles.Add(m_GLTM->get(tmp));
//					m_editor_tiles_name.Add(new Symbol(tmp));
				}
				free(namelist);
			}
#endif  
		} // if 

		if (m_editor_object_tiles.EmptyP()) {
			
			m_editor_object_tiles.Add(m_GLTM->get("objects/pipe-ball-stand1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/red-light1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/red-light1-snow"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/pipe-vertical-screen1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/pipe-horizontal-screen1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/radar1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/laser-left1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/laser-right1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/laser-up1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/laser-down1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/cannon-rock-left1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/cannon-rock-right1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/cannon-rock-down1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/cannon-rock-up1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/spike-left"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/spike-right"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/fuel-recharge"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/cannon-techno-left1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/cannon-techno-right1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/cannon-techno-down1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/cannon-techno-up1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/fastcannon-techno-left1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/fastcannon-techno-right1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/fastcannon-techno-down1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/fastcannon-techno-up1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/wall-techno-computer1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/directionalcannon-left"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/directionalcannon-right"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/directionalcannon-down"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/directionalcannon-up"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/ha-directionalcannon-left"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/ha-directionalcannon-right"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/ha-directionalcannon-down"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/ha-directionalcannon-up"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/grey-tank"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/red-tank"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/green-tank"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/big-tank"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/door-left"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/door-right"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/button-red-left1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/button-red-right1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/button-red-down1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/button-red-up1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/button-purple-left1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/button-purple-right1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/button-purple-down1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/button-purple-up1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/button-blue-left1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/button-blue-right1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/button-blue-down1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/button-blue-up1"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/fast-directionalcannon-left"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/fast-directionalcannon-right"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/fast-directionalcannon-down"));
			m_editor_object_tiles.Add(m_GLTM->get("objects/fast-directionalcannon-up"));			
		} // if 
		
		
		if (m_editor_smart_tiles.EmptyP()) {
			m_editor_smart_tiles.Add(m_GLTM->get("foreground/wall1"));
			m_editor_smart_tiles.Add(m_GLTM->get("foreground/wall-snow1"));
			m_editor_smart_tiles.Add(m_GLTM->get("foreground/wall-techno1"));
			m_editor_smart_tiles.Add(m_GLTM->get("foreground/pipe-vertical1"));
		}

	} else {
		// Create the variable part of the interface depending on the selected mode:
		switch(m_editor_mode) {
		case 1: // TILES
				TGLinterface::remove_element(14);
				TGLinterface::remove_element(15);
				TGLinterface::remove_element(17);
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

		case 2: // OBJECTS
				TGLinterface::remove_element(11);
				TGLinterface::remove_element(15);
				TGLinterface::remove_element(17);
				{
					TGLTileBrowser *tb = (TGLTileBrowser *)TGLinterface::get(14);
					if (tb==0) {
						GLTile *t;
						tb = new TGLTileBrowser(10,180,100,256,14);
						TGLinterface::add_element(tb);

						m_editor_object_tiles.Rewind();
						while(m_editor_object_tiles.Iterate(t)) tb->addEntry(t);
					} // if

					m_editor_selected_object = tb->getSelected();
				}

				// Check if the pointer is over an object (for the delete pointer):
				{
					int mouse_x,mouse_y;

					mouse_x = int((m_mouse_x-(SCREEN_X/2))/m_editor_current_zoom + (m_editor_focus_x));
					mouse_y = int((m_mouse_y-(SCREEN_Y/2))/m_editor_current_zoom + (m_editor_focus_y));

					if (mouse_x<0 || mouse_y<0 ||
						mouse_x>=m_editor_level_editing->get_dx() ||
						mouse_y>=m_editor_level_editing->get_dy()) {
						mouse_x = -1;
						mouse_y = -1;
					} // if

					m_mb_object_under_pointer = m_editor_level_editing->collision_with_object(float(mouse_x),float(mouse_y));

				}

				break;

		case 3: TGLinterface::remove_element(11);
				TGLinterface::remove_element(14);
				TGLinterface::remove_element(15);
				TGLinterface::remove_element(17);
				
				// Check if the pointer is over an object (for the delete pointer):
				{
					int mouse_x,mouse_y;

					mouse_x = int((m_mouse_x-(SCREEN_X/2))/m_editor_current_zoom + (m_editor_focus_x));
					mouse_y = int((m_mouse_y-(SCREEN_Y/2))/m_editor_current_zoom + (m_editor_focus_y));

					if (mouse_x<0 || mouse_y<0 ||
						mouse_x>=m_editor_level_editing->get_dx() ||
						mouse_y>=m_editor_level_editing->get_dy()) {
						mouse_x = -1;
						mouse_y = -1;
					} // if

					m_mb_object_under_pointer = m_editor_level_editing->collision_with_object(float(mouse_x),float(mouse_y));
				}
				break;
				
		case 4: // SMART MODE
				TGLinterface::remove_element(11);
				TGLinterface::remove_element(14);
				TGLinterface::remove_element(15);

				{
					TGLTileBrowser *tb = (TGLTileBrowser *)TGLinterface::get(17);
					if (tb==0) {
						GLTile *t;
						tb = new TGLTileBrowser(10,180,70,256,17);
						TGLinterface::add_element(tb);
						
						m_editor_smart_tiles.Rewind();
						while(m_editor_smart_tiles.Iterate(t)) tb->addEntry(t);
					} // if
					
					m_editor_selected_tile = tb->getSelected();
				}
				break;

		} // switch

		// Insert position:
		{
			m_editor_insert_x = int((m_mouse_x-(SCREEN_X/2))/m_editor_current_zoom + (m_editor_focus_x));
			m_editor_insert_y = int((m_mouse_y-(SCREEN_Y/2))/m_editor_current_zoom + (m_editor_focus_y));

			m_editor_insert_x = (m_editor_insert_x/32)*32;
			m_editor_insert_y = ((m_editor_insert_y-STARFIELD)/32)*32;

			if (m_editor_insert_x<0 || m_editor_insert_y<0 ||
				m_editor_insert_x>=m_editor_level_editing->get_dx() ||
				m_editor_insert_y>=m_editor_level_editing->get_dy()) {
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

		if (k->key_press(SDLK_q)) {
			if (m_editor_mode==1 && m_editor_selected_tile>0) {
				TGLTileBrowser *tb = (TGLTileBrowser *)TGLinterface::get(11);
				if (tb!=NULL) {
					m_editor_selected_tile--;
					tb->setSelected(m_editor_selected_tile);
				}
			} else if (m_editor_mode==2 && m_editor_selected_object>0) {
				TGLTileBrowser *tb = (TGLTileBrowser *)TGLinterface::get(14);
				if (tb!=NULL) {
					m_editor_selected_object--;
					tb->setSelected(m_editor_selected_object);				
				}
			}
		}
		if (k->key_press(SDLK_a)) {
			if (m_editor_mode==1) {
				TGLTileBrowser *tb = (TGLTileBrowser *)TGLinterface::get(11);
				if (tb!=NULL && m_editor_selected_tile < tb->getNEntries()-1) {
					m_editor_selected_tile++;
					tb->setSelected(m_editor_selected_tile);
				}
			} else if (m_editor_mode==2) {
				TGLTileBrowser *tb = (TGLTileBrowser *)TGLinterface::get(14);
				if (tb!=NULL && m_editor_selected_object < tb->getNEntries()-1) {
					m_editor_selected_object++;
					tb->setSelected(m_editor_selected_object);
				}
			}			
		}

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
				{
					FILE *fp = fopen("tmp.map","w+");
					if (fp!=0) {
						m_editor_level_editing->save(fp,m_GLTM);
						fclose(fp);
						delete m_editor_level_editing;
						m_editor_level_editing=0;
						fp = fopen("tmp.map","r+");
						delete m_editor_level->m_map_data;
						m_editor_level->m_map_data = new TGLmap(fp,m_GLTM);
						fclose(fp);
					} // if 
				}	
				return TGL_STATE_EDITOR;
				break;
		case 2:	// CANCEL:
				delete m_editor_level_editing;
				m_editor_level_editing=0;
				return TGL_STATE_EDITOR;
				break;

		case 3:	// MODE
				m_editor_mode++;
				if (m_editor_mode>4) m_editor_mode=1;
				break;

		case 4: if (m_editor_level_editing->m_fg_dx>16) m_editor_level_editing->resize(m_editor_level_editing->m_fg_dx-1,m_editor_level_editing->m_fg_dy,m_GLTM);
				if (m_editor_focus_x>m_editor_level_editing->get_dx()) m_editor_focus_x=m_editor_level_editing->get_dx();
				if (m_editor_focus_y>m_editor_level_editing->get_dy()) m_editor_focus_y=m_editor_level_editing->get_dy();
				break;
		case 5: if (m_editor_level_editing->m_fg_dx<128) m_editor_level_editing->resize(m_editor_level_editing->m_fg_dx+1,m_editor_level_editing->m_fg_dy,m_GLTM);
				if (m_editor_focus_x>m_editor_level_editing->get_dx()) m_editor_focus_x=m_editor_level_editing->get_dx();
				if (m_editor_focus_y>m_editor_level_editing->get_dy()) m_editor_focus_y=m_editor_level_editing->get_dy();
				break;
		case 6: if (m_editor_level_editing->m_fg_dy>16) m_editor_level_editing->resize(m_editor_level_editing->m_fg_dx,m_editor_level_editing->m_fg_dy-1,m_GLTM);
				if (m_editor_focus_x>m_editor_level_editing->get_dx()) m_editor_focus_x=m_editor_level_editing->get_dx();
				if (m_editor_focus_y>m_editor_level_editing->get_dy()) m_editor_focus_y=m_editor_level_editing->get_dy();
				break;
		case 7: if (m_editor_level_editing->m_fg_dy<128) m_editor_level_editing->resize(m_editor_level_editing->m_fg_dx,m_editor_level_editing->m_fg_dy+1,m_GLTM);
				if (m_editor_focus_x>m_editor_level_editing->get_dx()) m_editor_focus_x=m_editor_level_editing->get_dx();
				if (m_editor_focus_y>m_editor_level_editing->get_dy()) m_editor_focus_y=m_editor_level_editing->get_dy();
				break;
		case 8: {
					int bg = m_editor_level_editing->m_bg_code+1;
					if (bg>4) bg=0;
					m_editor_level_editing->set_background(bg,m_GLTM);
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

		case 12: // LOAD
				TGLinterface::add_element(new TGLConfirmation("Discard current map?",m_font16,320,200,112,true));
				break;
		case 112:
				return TGL_STATE_LOADMAP;
				break;

		case 13:
				return TGL_STATE_SAVEMAP;
				break;
		case 16:
				TGLinterface::add_element(new TGLConfirmation("Map Editor Help:\n\n"
															  "Cursors - navigate the map\n"
															  "SPACE - Insert Tile/Object\n"
															  "DELETE - Delete Tile/Object\n"
															  "q/a - Previous/Next Tile/Object",m_font16,320,200,-1,false));
				break;
		} // switch

		// Move around:
		if (/*(ID==-1 && m_mouse_button==SDL_BUTTON_WHEELLEFT) || */k->keyboard[SDLK_LEFT] || m_mouse_x<8) {
			m_editor_focus_x-=int(8/m_editor_real_zoom);
			if (m_editor_focus_x<0) m_editor_focus_x=0;
		} // if
		if (/*(ID==-1 && m_mouse_button==SDL_BUTTON_WHEELRIGHT) || */k->keyboard[SDLK_RIGHT] || m_mouse_x>=SCREEN_X-8) {
			m_editor_focus_x+=int(8/m_editor_real_zoom);
			if (m_editor_focus_x>m_editor_level_editing->get_dx()) m_editor_focus_x=m_editor_level_editing->get_dx();
		} // if
		if ((ID==-1 && m_mouse_button==SDL_BUTTON_WHEELUP) || k->keyboard[SDLK_UP] || m_mouse_y<8) {
			m_editor_focus_y-=int(8/m_editor_real_zoom);
			if (m_editor_focus_y<0) m_editor_focus_y=0;
		} // if
		if ((ID==-1 && m_mouse_button==SDL_BUTTON_WHEELDOWN) || k->keyboard[SDLK_DOWN] || m_mouse_y>=SCREEN_Y-8) {
			m_editor_focus_y+=int(8/m_editor_real_zoom);	
			if (m_editor_focus_y>m_editor_level_editing->get_dy()) m_editor_focus_y=m_editor_level_editing->get_dy();		
		} // if		
		
		// Button pressed (but not over any interface element);
		if (ID==-1 && m_mouse_button==1) {
			switch(m_editor_mode) {
			case 1:
					{
						if (m_editor_selected_tile!=-1 &&
							m_editor_insert_x!=-1) {
							int offs = m_editor_insert_x/32 + (m_editor_insert_y/32)*m_editor_level_editing->m_fg_dx;
							m_editor_level_editing->m_fg[offs]=m_editor_tiles[m_editor_selected_tile];
						} // if
					}
					break;
			case 2:
				
					if (m_editor_selected_object==0) {
						m_editor_level_editing->add_object(new TGLobject_ballstand(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
					} else if (m_editor_selected_object==1) {
						m_editor_level_editing->add_object(new TGLobject_redlight(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,0));
					} else if (m_editor_selected_object==2) {
						m_editor_level_editing->add_object(new TGLobject_redlight(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,1));
					} else if (m_editor_selected_object==3) {
						m_editor_level_editing->add_object(new TGLobject_pipevscreen(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
					} else if (m_editor_selected_object==4) {
						m_editor_level_editing->add_object(new TGLobject_pipehscreen(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
					} else if (m_editor_selected_object==5) {
						m_editor_level_editing->add_object(new TGLobject_radar(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
					} else if (m_editor_selected_object==6) {
						m_editor_level_editing->add_object(new TGLobject_laser_left(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
						m_editor_level_editing->create_laser_objects();
					} else if (m_editor_selected_object==7) {
						m_editor_level_editing->add_object(new TGLobject_laser_right(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
						m_editor_level_editing->create_laser_objects();
					} else if (m_editor_selected_object==8) {
						m_editor_level_editing->add_object(new TGLobject_laser_up(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
						m_editor_level_editing->create_laser_objects();
					} else if (m_editor_selected_object==9) {
						m_editor_level_editing->add_object(new TGLobject_laser_down(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
						m_editor_level_editing->create_laser_objects();

					} else if (m_editor_selected_object==10) {
						m_editor_level_editing->add_object(new TGLobject_cannon_left(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,0));
					} else if (m_editor_selected_object==11) {
						m_editor_level_editing->add_object(new TGLobject_cannon_right(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,0));
					} else if (m_editor_selected_object==12) {
						m_editor_level_editing->add_object(new TGLobject_cannon_down(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,0));
					} else if (m_editor_selected_object==13) {
						m_editor_level_editing->add_object(new TGLobject_cannon_up(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,0));
					
					} else if (m_editor_selected_object==14) {
						m_editor_level_editing->add_object(new TGLobject_spike_left(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
					} else if (m_editor_selected_object==15) {
						m_editor_level_editing->add_object(new TGLobject_spike_right(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
					} else if (m_editor_selected_object==16) {
						m_editor_level_editing->add_object(new TGLobject_fuelrecharge(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));

					} else if (m_editor_selected_object==17) {
						m_editor_level_editing->add_object(new TGLobject_cannon_left(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,1));
					} else if (m_editor_selected_object==18) {
						m_editor_level_editing->add_object(new TGLobject_cannon_right(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,1));
					} else if (m_editor_selected_object==19) {
						m_editor_level_editing->add_object(new TGLobject_cannon_down(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,1));
					} else if (m_editor_selected_object==20) {
						m_editor_level_editing->add_object(new TGLobject_cannon_up(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,1));

					} else if (m_editor_selected_object==21) {
						m_editor_level_editing->add_object(new TGLobject_fastcannon_left(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
					} else if (m_editor_selected_object==22) {
						m_editor_level_editing->add_object(new TGLobject_fastcannon_right(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
					} else if (m_editor_selected_object==23) {
						m_editor_level_editing->add_object(new TGLobject_fastcannon_down(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
					} else if (m_editor_selected_object==24) {
						m_editor_level_editing->add_object(new TGLobject_fastcannon_up(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));

					} else if (m_editor_selected_object==25) {
						m_editor_level_editing->add_object(new TGLobject_techno_computer(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));

					} else if (m_editor_selected_object==26) {
						m_editor_level_editing->add_object(new TGLobject_directionalcannon_left(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
					} else if (m_editor_selected_object==27) {
						m_editor_level_editing->add_object(new TGLobject_directionalcannon_right(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
					} else if (m_editor_selected_object==28) {
						m_editor_level_editing->add_object(new TGLobject_directionalcannon_down(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
					} else if (m_editor_selected_object==29) {
						m_editor_level_editing->add_object(new TGLobject_directionalcannon_up(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));

					} else if (m_editor_selected_object==30) {
						m_editor_level_editing->add_object(new TGLobject_ha_directionalcannon_left(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
					} else if (m_editor_selected_object==31) {
						m_editor_level_editing->add_object(new TGLobject_ha_directionalcannon_right(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
					} else if (m_editor_selected_object==32) {
						m_editor_level_editing->add_object(new TGLobject_ha_directionalcannon_down(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
					} else if (m_editor_selected_object==33) {
						m_editor_level_editing->add_object(new TGLobject_ha_directionalcannon_up(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));

					} else if (m_editor_selected_object==34) {
						TGLobject_enemy *tank,*turret,*cannon;
						tank=new TGLobject_tank(float(m_editor_insert_x+32),float(m_editor_insert_y+45+STARFIELD),0,0);
						turret=new TGLobject_tank_turret(float(m_editor_insert_x+34),float(m_editor_insert_y+28+STARFIELD),0,tank,0);
						cannon=new TGLobject_tank_cannon(float(m_editor_insert_x+34),float(m_editor_insert_y+20+STARFIELD),0,turret);
						m_editor_level_editing->add_object(cannon);
						m_editor_level_editing->add_object(turret);
						m_editor_level_editing->add_object(tank);
						tank->exclude_for_collision(turret);
						tank->exclude_for_collision(cannon);
						turret->exclude_for_collision(tank);
						turret->exclude_for_collision(cannon);
						cannon->exclude_for_collision(tank);
						cannon->exclude_for_collision(turret);
					} else if (m_editor_selected_object==35) {
						TGLobject_enemy *tank,*turret,*cannon;
						tank=new TGLobject_tank(float(m_editor_insert_x+32),float(m_editor_insert_y+45+STARFIELD),0,1);
						turret=new TGLobject_tank_turret(float(m_editor_insert_x+34),float(m_editor_insert_y+28+STARFIELD),0,tank,1);
						cannon=new TGLobject_tank_cannon(float(m_editor_insert_x+34),float(m_editor_insert_y+20+STARFIELD),0,turret);
						m_editor_level_editing->add_object(cannon);
						m_editor_level_editing->add_object(turret);
						m_editor_level_editing->add_object(tank);
						tank->exclude_for_collision(turret);
						tank->exclude_for_collision(cannon);
						turret->exclude_for_collision(tank);
						turret->exclude_for_collision(cannon);
						cannon->exclude_for_collision(tank);
						cannon->exclude_for_collision(turret);
					} else if (m_editor_selected_object==36) {
						TGLobject_enemy *tank,*turret,*cannon;
						tank=new TGLobject_tank(float(m_editor_insert_x+32),float(m_editor_insert_y+45+STARFIELD),0,2);
						turret=new TGLobject_tank_turret(float(m_editor_insert_x+34),float(m_editor_insert_y+28+STARFIELD),0,tank,2);
						cannon=new TGLobject_tank_cannon(float(m_editor_insert_x+34),float(m_editor_insert_y+20+STARFIELD),0,turret);
						m_editor_level_editing->add_object(cannon);
						m_editor_level_editing->add_object(turret);
						m_editor_level_editing->add_object(tank);
						tank->exclude_for_collision(turret);
						tank->exclude_for_collision(cannon);
						turret->exclude_for_collision(tank);
						turret->exclude_for_collision(cannon);
						cannon->exclude_for_collision(tank);
						cannon->exclude_for_collision(turret);
					} else if (m_editor_selected_object==37) {
						TGLobject_enemy *tank,*turret,*cannon;
						tank=new TGLobject_big_tank(float(m_editor_insert_x+32),float(m_editor_insert_y+45+STARFIELD),0);
						turret=new TGLobject_tank_turret(float(m_editor_insert_x+32),float(m_editor_insert_y+19+STARFIELD),0,tank,3);
						cannon=new TGLobject_tank_cannon(float(m_editor_insert_x+32),float(m_editor_insert_y+11+STARFIELD),0,turret);
						m_editor_level_editing->add_object(cannon);
						m_editor_level_editing->add_object(turret);
						m_editor_level_editing->add_object(tank);
						tank->exclude_for_collision(turret);
						tank->exclude_for_collision(cannon);
						turret->exclude_for_collision(tank);
						turret->exclude_for_collision(cannon);
						cannon->exclude_for_collision(tank);
						cannon->exclude_for_collision(turret);

					} else if (m_editor_selected_object==38) {
						m_editor_level_editing->add_object(new TGLobject_leftdoor(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,0,0));
					} else if (m_editor_selected_object==39) {
						m_editor_level_editing->add_object(new TGLobject_rightdoor(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,0,0));

					} else if (m_editor_selected_object==40) {
						m_editor_level_editing->add_object(new TGLobject_button(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,0,0));
					} else if (m_editor_selected_object==41) {
						m_editor_level_editing->add_object(new TGLobject_button(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,0,1));
					} else if (m_editor_selected_object==42) {
						m_editor_level_editing->add_object(new TGLobject_button(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,0,3));
					} else if (m_editor_selected_object==43) {
						m_editor_level_editing->add_object(new TGLobject_button(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,0,2));

					} else if (m_editor_selected_object==44) {
						m_editor_level_editing->add_object(new TGLobject_button(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,0,4));
					} else if (m_editor_selected_object==45) {
						m_editor_level_editing->add_object(new TGLobject_button(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,0,5));
					} else if (m_editor_selected_object==46) {
						m_editor_level_editing->add_object(new TGLobject_button(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,0,7));
					} else if (m_editor_selected_object==47) {
						m_editor_level_editing->add_object(new TGLobject_button(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,0,6));

					} else if (m_editor_selected_object==48) {
						m_editor_level_editing->add_object(new TGLobject_button(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,0,8));
					} else if (m_editor_selected_object==49) {
						m_editor_level_editing->add_object(new TGLobject_button(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,0,9));
					} else if (m_editor_selected_object==50) {
						m_editor_level_editing->add_object(new TGLobject_button(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,0,11));
					} else if (m_editor_selected_object==51) {
						m_editor_level_editing->add_object(new TGLobject_button(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0,0,10));

					} else if (m_editor_selected_object==52) {
						m_editor_level_editing->add_object(new TGLobject_fast_directionalcannon_left(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
					} else if (m_editor_selected_object==53) {
						m_editor_level_editing->add_object(new TGLobject_fast_directionalcannon_right(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
					} else if (m_editor_selected_object==54) {
						m_editor_level_editing->add_object(new TGLobject_fast_directionalcannon_down(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
					} else if (m_editor_selected_object==55) {
						m_editor_level_editing->add_object(new TGLobject_fast_directionalcannon_up(float(m_editor_insert_x),float(m_editor_insert_y+STARFIELD),0));
					} // if 

					break;
			case 3:
					if (m_mb_object_under_pointer!=0) {
						if (m_mb_object_under_pointer->is_a("TGLobject_button")) {
							if (k->keyboard[SDLK_1]) {
								m_mb_object_under_pointer->set_animation_offset(m_mb_object_under_pointer->get_animation_offset()+1);
							} // if 
							if (k->keyboard[SDLK_2]) {
								int e = ((TGLobject_button *)m_mb_object_under_pointer)->get_event();
								((TGLobject_button *)m_mb_object_under_pointer)->set_event(e+1);
							} // if 

						} else if (m_mb_object_under_pointer->is_a("TGLobject_leftdoor")) {
							if (k->keyboard[SDLK_1]) {
								int e = ((TGLobject_leftdoor *)m_mb_object_under_pointer)->get_action();
								((TGLobject_leftdoor *)m_mb_object_under_pointer)->set_action(e+1);
							} // if 
							if (k->keyboard[SDLK_2]) {
								m_mb_object_under_pointer->set_state(1-m_mb_object_under_pointer->get_state());
							} // if 

						} else if (m_mb_object_under_pointer->is_a("TGLobject_rightdoor")) {
							if (k->keyboard[SDLK_1]) {
								int e = ((TGLobject_rightdoor *)m_mb_object_under_pointer)->get_action();
								((TGLobject_rightdoor *)m_mb_object_under_pointer)->set_action(e+1);
							} // if 
							if (k->keyboard[SDLK_2]) {
								m_mb_object_under_pointer->set_state(1-m_mb_object_under_pointer->get_state());
							} // if 

						} else {
							if (k->keyboard[SDLK_1]) {
								m_mb_object_under_pointer->set_animation_offset(m_mb_object_under_pointer->get_animation_offset()+1);
							} // if 							
						} // if
					} // if

					break;
			case 4: // SMART MODE CLICK:
				{
					if (m_editor_selected_tile!=-1 &&
						m_editor_insert_x!=-1) {
						int offs = m_editor_insert_x/32 + (m_editor_insert_y/32)*m_editor_level_editing->m_fg_dx;
						
						// compute the smar ttile:
						// ...
						
						m_editor_level_editing->m_fg[offs]=m_editor_tiles[m_editor_selected_tile];
					} // if
				}
					break;
			} // if
		} // if

		// Right button pressed (but not over any interface element);
		if ((ID==-1 && m_mouse_button==3) || k->key_press(SDLK_DELETE)  || k->key_press(SDLK_BACKSPACE)) {
			switch(m_editor_mode) {
			case 1:
					{
						if (m_editor_selected_tile!=-1 &&
							m_editor_insert_x!=-1) {
							int offs = m_editor_insert_x/32 + (m_editor_insert_y/32)*m_editor_level_editing->m_fg_dx;
							m_editor_level_editing->m_fg[offs]=0;
						} // if
					}
					break;
			case 2:
					if (m_mb_object_under_pointer!=0) {
						if (m_mb_object_under_pointer->is_a("TGLobject_tank")) {
							List<TGLobject> *l = m_editor_level_editing->get_objects("TGLobject_tank_cannon");
							TGLobject *o;

							l->Rewind();
							while(l->Iterate(o)) {
								if (((TGLobject_tank_turret *)((TGLobject_tank_cannon *)o)->get_turret())->get_tank()==m_mb_object_under_pointer) {
									m_editor_level_editing->remove_object(o);
								} // if 
							} // while 
							l->ExtractAll();
							delete l;

							l = m_editor_level_editing->get_objects("TGLobject_tank_turret");
							l->Rewind();
							while(l->Iterate(o)) {
								if (((TGLobject_tank_turret *)o)->get_tank()==m_mb_object_under_pointer) {
									m_editor_level_editing->remove_object(o);
								} // if 
							} // while 
							l->ExtractAll();
							delete l;

							m_editor_level_editing->remove_object(m_mb_object_under_pointer);
							m_mb_object_under_pointer=0;

						} else if (m_mb_object_under_pointer->is_a("TGLobject_tank_turret")) {
							List<TGLobject> *l = m_editor_level_editing->get_objects("TGLobject_tank_cannon");
							TGLobject *o;

							l->Rewind();
							while(l->Iterate(o)) {
								if (((TGLobject_tank_cannon *)o)->get_turret()==m_mb_object_under_pointer) {
									m_editor_level_editing->remove_object(o);
								} // if 
							} // while 
							l->ExtractAll();
							delete l;

							m_editor_level_editing->remove_object(((TGLobject_tank_turret *)m_mb_object_under_pointer)->get_tank());
							m_editor_level_editing->remove_object(m_mb_object_under_pointer);
							m_mb_object_under_pointer=0;		
							
						} else if (m_mb_object_under_pointer->is_a("TGLobject_tank_cannon")) {
							List<TGLobject> *l = m_editor_level_editing->get_objects("TGLobject_tank_turret");
							TGLobject *o;

							l->Rewind();
							while(l->Iterate(o)) {
								if (o==((TGLobject_tank_cannon *)m_mb_object_under_pointer)->get_turret()) {
									m_editor_level_editing->remove_object(((TGLobject_tank_turret *)o)->get_tank());
									m_editor_level_editing->remove_object(o);
								} // if 
							} // while 
							l->ExtractAll();
							delete l;

							m_editor_level_editing->remove_object(m_mb_object_under_pointer);
							m_mb_object_under_pointer=0;

						} else if (m_mb_object_under_pointer->is_a("TGLobject_big_tank")) {
							List<TGLobject> *l = m_editor_level_editing->get_objects("TGLobject_tank_cannon");
							TGLobject *o;

							l->Rewind();
							while(l->Iterate(o)) {
								if (((TGLobject_tank_turret *)((TGLobject_tank_cannon *)o)->get_turret())->get_tank()==m_mb_object_under_pointer) {
									m_editor_level_editing->remove_object(o);
								} // if 
							} // while 
							l->ExtractAll();
							delete l;

							l = m_editor_level_editing->get_objects("TGLobject_tank_turret");
							l->Rewind();
							while(l->Iterate(o)) {
								if (((TGLobject_tank_turret *)o)->get_tank()==m_mb_object_under_pointer) {
									m_editor_level_editing->remove_object(o);
								} // if 
							} // while 
							l->ExtractAll();
							delete l;

							m_editor_level_editing->remove_object(m_mb_object_under_pointer);
							m_mb_object_under_pointer=0;

						} else {
							m_editor_level_editing->remove_object(m_mb_object_under_pointer);
							m_mb_object_under_pointer=0;
						} // if 
						
						m_editor_level_editing->create_laser_objects();

					} // if 
					 
					break;
			case 3:
					if (m_mb_object_under_pointer!=0) {
						if (m_mb_object_under_pointer->is_a("TGLobject_button")) {
							if (k->keyboard[SDLK_1]) {
								m_mb_object_under_pointer->set_animation_offset(m_mb_object_under_pointer->get_animation_offset()-1);
							} // if 
							if (k->keyboard[SDLK_2]) {
								int e = ((TGLobject_button *)m_mb_object_under_pointer)->get_event();
								((TGLobject_button *)m_mb_object_under_pointer)->set_event(e-1);
							} // if 

						} else if (m_mb_object_under_pointer->is_a("TGLobject_leftdoor")) {
							if (k->keyboard[SDLK_1]) {
								int e = ((TGLobject_leftdoor *)m_mb_object_under_pointer)->get_action();
								((TGLobject_leftdoor *)m_mb_object_under_pointer)->set_action(e-1);
							} // if 
							if (k->keyboard[SDLK_2]) {
								m_mb_object_under_pointer->set_state(1-m_mb_object_under_pointer->get_state());
							} // if 

						} else if (m_mb_object_under_pointer->is_a("TGLobject_rightdoor")) {
							if (k->keyboard[SDLK_1]) {
								int e = ((TGLobject_rightdoor *)m_mb_object_under_pointer)->get_action();
								((TGLobject_rightdoor *)m_mb_object_under_pointer)->set_action(e-1);
							} // if 
							if (k->keyboard[SDLK_2]) {
								m_mb_object_under_pointer->set_state(1-m_mb_object_under_pointer->get_state());
							} // if 

						} else {
							if (k->keyboard[SDLK_1]) {
								m_mb_object_under_pointer->set_animation_offset(m_mb_object_under_pointer->get_animation_offset()-1);
							} // if 							
						} // if
					} // if
					break;
					
			case 4: // SMART MODE CLICK:
					{
						if (m_editor_selected_tile!=-1 &&
							m_editor_insert_x!=-1) {
							int offs = m_editor_insert_x/32 + (m_editor_insert_y/32)*m_editor_level_editing->m_fg_dx;
							m_editor_level_editing->m_fg[offs]=0;
						} // if
					}
					break;
			} // if
		} // if
	}

	if (m_editor_level_editing!=0) m_editor_level_editing->editor_cycle(m_GLTM);

    return TGL_STATE_MAPEDITOR;
} /* TheGooniesApp::mapeditor_cycle */ 



void TGLapp::mapeditor_draw(void)
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

	// Draw level:
	if (m_editor_level!=0) {
		TGLmap *m = m_editor_level_editing;

		m->draw(m_editor_focus_x,m_editor_focus_y,640,480,m_GLTM,m_editor_current_zoom);
	} // if 

	// Draw extras:
	{
		char tmp[80];

		sprintf(tmp,"%i",m_editor_level_editing->m_fg_dx);
		TGLinterface::print_center(tmp,m_font16,50,32);
		sprintf(tmp,"%i",m_editor_level_editing->m_fg_dy);
		TGLinterface::print_center(tmp,m_font16,50,62);	
	}

	switch(m_editor_mode) {
	case 1: TGLinterface::print_center("tiles",m_font16,50,180);

			if (m_editor_insert_x!=-1 && !TGLinterface::mouse_over_element(m_mouse_x,m_mouse_y)) {
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

			if (m_editor_insert_x!=-1 && m_editor_selected_object!=-1 && !TGLinterface::mouse_over_element(m_mouse_x,m_mouse_y)) {
				float x,y;
				GLTile *t = m_editor_object_tiles[m_editor_selected_object];

				x = ((m_editor_insert_x-m_editor_focus_x)*m_editor_current_zoom) + (SCREEN_X/2);
				y = ((m_editor_insert_y+STARFIELD-m_editor_focus_y)*m_editor_current_zoom) + (SCREEN_Y/2);
			
				glPushMatrix();
				glTranslatef(x,y,0);
				glBegin(GL_LINES);
				glColor4f(1,1,1,1);
				glVertex3f(0,0,0);
				glVertex3f((t->get_dx())*m_editor_current_zoom,0,0);
				glVertex3f(0,(t->get_dy())*m_editor_current_zoom,0);
				glVertex3f((t->get_dx())*m_editor_current_zoom,(t->get_dy())*m_editor_current_zoom,0);
				glVertex3f(0,0,0);
				glVertex3f(0,(t->get_dy())*m_editor_current_zoom,0);
				glVertex3f((t->get_dx())*m_editor_current_zoom,0,0);
				glVertex3f((t->get_dx())*m_editor_current_zoom,(t->get_dy())*m_editor_current_zoom,0);
				glEnd();
				glPopMatrix();
			} // if

			if (m_mb_object_under_pointer!=0 && !TGLinterface::mouse_over_element(m_mouse_x,m_mouse_y)) {
				float x,y;
				GLTile *t = m_mb_object_under_pointer->get_last_tile();

				x = m_mb_object_under_pointer->get_x() - t->get_hot_x();
				y = m_mb_object_under_pointer->get_y() - t->get_hot_y();

				x = ((x-m_editor_focus_x)*m_editor_current_zoom) + (SCREEN_X/2);
				y = ((y-m_editor_focus_y)*m_editor_current_zoom) + (SCREEN_Y/2);
			
				glPushMatrix();
				glTranslatef(x,y,0);
				glBegin(GL_LINES);
				glColor4f(1,0,0,1);
				glVertex3f(0,0,0);
				glVertex3f((t->get_dx())*m_editor_current_zoom,0,0);
				glVertex3f(0,(t->get_dy())*m_editor_current_zoom,0);
				glVertex3f((t->get_dx())*m_editor_current_zoom,(t->get_dy())*m_editor_current_zoom,0);
				glVertex3f(0,0,0);
				glVertex3f(0,(t->get_dy())*m_editor_current_zoom,0);
				glVertex3f((t->get_dx())*m_editor_current_zoom,0,0);
				glVertex3f((t->get_dx())*m_editor_current_zoom,(t->get_dy())*m_editor_current_zoom,0);
				glEnd();
				glPopMatrix();
			} // if 
		
			break;

	case 3: TGLinterface::print_center("properties",m_font16,50,180);

			if (m_mb_object_under_pointer!=0) {
				float x,y;
				GLTile *t = m_mb_object_under_pointer->get_last_tile();

				x = m_mb_object_under_pointer->get_x() - t->get_hot_x();
				y = m_mb_object_under_pointer->get_y() - t->get_hot_y();

				x = ((x-m_editor_focus_x)*m_editor_current_zoom) + (SCREEN_X/2);
				y = ((y-m_editor_focus_y)*m_editor_current_zoom) + (SCREEN_Y/2);
			
				glPushMatrix();
				glTranslatef(x,y,0);
				glBegin(GL_LINES);
				glColor4f(1,0,0,1);
				glVertex3f(0,0,0);
				glVertex3f((t->get_dx())*m_editor_current_zoom,0,0);
				glVertex3f(0,(t->get_dy())*m_editor_current_zoom,0);
				glVertex3f((t->get_dx())*m_editor_current_zoom,(t->get_dy())*m_editor_current_zoom,0);
				glVertex3f(0,0,0);
				glVertex3f(0,(t->get_dy())*m_editor_current_zoom,0);
				glVertex3f((t->get_dx())*m_editor_current_zoom,0,0);
				glVertex3f((t->get_dx())*m_editor_current_zoom,(t->get_dy())*m_editor_current_zoom,0);
				glEnd();
				glPopMatrix();
			} // if 

			{
				List<TGLobject> *l;
				TGLobject *o;

				l=m_editor_level_editing->get_objects("TGLobject");
				l->Rewind();
				while(l->Iterate(o)) {
					float r=1,g=1,b=1;
					float x,y;
					GLTile *t = o->get_last_tile();

					x = o->get_x() - t->get_hot_x();
					y = o->get_y() - t->get_hot_y();

					x = ((x-m_editor_focus_x)*m_editor_current_zoom) + (SCREEN_X/2);
					y = ((y-m_editor_focus_y)*m_editor_current_zoom) + (SCREEN_Y/2);

					if (m_mb_object_under_pointer==o) {
						r = 1;
						g = 0;
						b = 0;
					} else {
						r = 1;
						g = 1;
						b = 1;
					} // if 

					if (o->is_a("TGLobject_button")) {
						char tmp[80];
						sprintf(tmp,"(1) AO: %i",o->get_animation_offset());
						TGLinterface::print_left(tmp,m_font16,x,y,r,g,b,1.0f);
						sprintf(tmp,"(2) ID: %i",((TGLobject_button *)o)->get_event());
						TGLinterface::print_left(tmp,m_font16,x,y+16,r,g,b,1.0f);
					} else if (o->is_a("TGLobject_leftdoor")) {
						char tmp[80];
						sprintf(tmp,"(1) ID: %i",((TGLobject_leftdoor *)o)->get_action());
						TGLinterface::print_left(tmp,m_font16,x,y,r,g,b,1.0f);
						sprintf(tmp,"(2) IS: %i",o->get_state());
						TGLinterface::print_left(tmp,m_font16,x,y+16,r,g,b,1.0f);
					} else if (o->is_a("TGLobject_rightdoor")) {
						char tmp[80];
						sprintf(tmp,"(1) ID: %i",((TGLobject_rightdoor *)o)->get_action());
						TGLinterface::print_left(tmp,m_font16,x,y,r,g,b,1.0f);
						sprintf(tmp,"(2) IS: %i",o->get_state());
						TGLinterface::print_left(tmp,m_font16,x,y+16,r,g,b,1.0f);
					} else {
						char tmp[80];
						sprintf(tmp,"(1) AO: %i",o->get_animation_offset());
						TGLinterface::print_left(tmp,m_font16,x,y,r,g,b,1.0f);
					} // if 
				} // while
				l->ExtractAll();
				delete l;
			}
			break;
			
	case 4: TGLinterface::print_center("smart",m_font16,50,180);
			
			if (m_editor_insert_x!=-1 && !TGLinterface::mouse_over_element(m_mouse_x,m_mouse_y)) {
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
			
	} // switch
	

	TGLinterface::draw();

} /* TGLapp::mapeditor_draw */ 



