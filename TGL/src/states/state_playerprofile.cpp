#ifdef KITSCHY_DEBUG_MEMORY 
#include "debug_memorymanager.h"
#endif

#ifdef _WIN32
#include <windows.h>
#else
#include <stddef.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

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
#include "PlayerProfile.h"
#include "TGLinterface.h"

#include "LevelPack.h"


int TGLapp::playerprofile_cycle(KEYBOARDSTATE *k)
{
	bool check_for_profiles_to_load=false;

	if (SDL_ShowCursor(SDL_QUERY)!=SDL_ENABLE) SDL_ShowCursor(SDL_ENABLE);
	if (m_state_cycle==0) {
		TGLinterface::reset();
		SDL_WarpMouse(210,352);
		TGLinterface::add_element(new TGLframe(10,40,580,276));		
		m_profile_create_button=new TGLbutton("Create",m_font32,40,404,180,64,0);
		m_profile_select_button=new TGLbutton("Select",m_font32,230,404,180,64,1);
		TGLinterface::add_element(m_profile_create_button);
		TGLinterface::add_element(m_profile_select_button);
		TGLinterface::add_element(new TGLbutton("Cancel",m_font32,420,404,180,64,2));

		TGLinterface::add_element(new TGLText("Select your player profile",m_font32,320,20,true));

		m_profile_mouse_over_profile=-1;
		m_profile_profile_selected=-1;

		m_profile_create_button->m_enabled=false;
		m_profile_select_button->m_enabled=false;


		// Load the profile files:
		{
			char *tmp;
			int i;

			m_profile_first_profile=0;
			m_profile_profile_fullnames.Delete();
			m_profile_profile_names.Delete();
			m_profile_profile_info.Delete();


#ifdef _WIN32
			/* Find files: */
			WIN32_FIND_DATA finfo;
			HANDLE h;

			h = FindFirstFile("players/*.pp", &finfo);
			if (h != INVALID_HANDLE_VALUE) {
				tmp=new char[strlen(finfo.cFileName)+1];
				strcpy(tmp,finfo.cFileName);
				tmp[strlen(tmp)-3]=0;
				m_profile_profile_names.Add(tmp);

				while (FindNextFile(h, &finfo) == TRUE) {
					tmp=new char[strlen(finfo.cFileName)+1];
					strcpy(tmp,finfo.cFileName);
					tmp[strlen(tmp)-3]=0;
					m_profile_profile_names.Add(tmp);
				} /* while */
			} /* if */
#else
			DIR *dp;
			struct dirent *ep;
			char dname[256];
			sprintf(dname, "%splayers/",m_player_data_path);
			dp = opendir (dname);
			if (dp != NULL) {
				while (ep = readdir (dp)) {
					if (strlen(ep->d_name) > 4 &&
							ep->d_name[strlen(ep->d_name) - 3] == '.' &&
							ep->d_name[strlen(ep->d_name) - 2] == 'p' &&
							ep->d_name[strlen(ep->d_name) - 1] == 'p') {

						tmp=new char[strlen(ep->d_name)+1];
						strcpy(tmp,ep->d_name);
						tmp[strlen(tmp)-3]=0;
						m_profile_profile_names.Add(tmp);
					} /* if */

				} /* while */
				(void) closedir (dp);
			} /* if */
#endif

			for(i=0;i<m_profile_profile_names.Length();i++) {
				char *tmp;

				tmp=new char[strlen(m_profile_profile_names[i])+256];
				sprintf(tmp,"%splayers/%s.pp",m_player_data_path,m_profile_profile_names[i]);
				m_profile_profile_fullnames.Add(tmp);
				m_profile_profile_info.Add(0);
			} // for
			check_for_profiles_to_load=true;
		}

		// Find an initial name for the profile:
		if (m_profile_profile_names.Length()>0) {
			// select the last open profile:
			{
				char last[256];
				char tmp[256];
				sprintf(tmp,"%slastplayer",m_player_data_path);
				FILE *fp=fopen(tmp,"r+");
				if (fp!=NULL) {
					fscanf(fp,"%s",last);
					
					m_profile_name_inputframe=new TGLTextInputFrame(last,32,m_font16,100,340,440,32,0);
					TGLinterface::add_element(m_profile_name_inputframe);
					m_profile_name_inputframe->m_focus=true;
				} else {
					last[0]=0;

					m_profile_name_inputframe=new TGLTextInputFrame(m_profile_profile_names[0],32,m_font16,100,340,440,32,0);
					TGLinterface::add_element(m_profile_name_inputframe);
					m_profile_name_inputframe->m_focus=true;
				}
			}
			
		} else {		
			int i=0;
			FILE *fp;
			char tmp[255],tmp2[255];
			bool found=false;

			do {
				if (i==0) {
					sprintf(tmp2,"player");
				} else {
					sprintf(tmp2,"player_%i",i);
				} // if 

				sprintf(tmp,"%splayers/%s.pp",m_player_data_path,tmp2);
				fp=fopen(tmp,"rb");
				if (fp!=0) {
					fclose(fp);
					found=true;
				} else {
					found=false;
				} // if 
				i++;
			} while(found);

			m_profile_name_inputframe=new TGLTextInputFrame(tmp2,32,m_font16,100,340,440,32,0);
			TGLinterface::add_element(m_profile_name_inputframe);
			m_profile_name_inputframe->m_focus=true;
		} // if 

		m_profile_profile_uparrow=new TGLbutton(m_GLTM->get("interface/uparrow"),600,40,30,100,3);
		TGLinterface::add_element(m_profile_profile_uparrow);
		if (m_profile_first_profile==0) m_profile_profile_uparrow->m_enabled=false;
							 else m_profile_profile_uparrow->m_enabled=true;
		m_profile_profile_downarrow=new TGLbutton(m_GLTM->get("interface/downarrow"),600,100+40+76,30,100,4);
		TGLinterface::add_element(m_profile_profile_downarrow);
		if (m_profile_profile_names.Length()>PROFILESPERPAGE) m_profile_profile_downarrow->m_enabled=true;
														     else m_profile_profile_downarrow->m_enabled=false;

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
			tmp=m_mouse_click_button.ExtractIni();
			button=*tmp;
			delete tmp;
		} else {
			button_status=SDL_GetMouseState(&mouse_x,&mouse_y);
			button=0;
		} // if 

		if (k->key_press(SDLK_SPACE)) button=1;

		ID=TGLinterface::update_state(mouse_x,mouse_y,button,button_status,k);

		if ((ID==0 && m_profile_create_button->m_enabled) || (ID==1 && m_profile_select_button->m_enabled) || ID==2) {
			m_state_fading=2;
			m_state_fading_cycle=0;
			m_state_selection=ID;
		} // if 

		if (ID==3) {
			m_profile_first_profile--;
			if (m_profile_first_profile==0) m_profile_profile_uparrow->m_enabled=false;
								 else m_profile_profile_uparrow->m_enabled=true;
			if (m_profile_profile_names.Length()>m_profile_first_profile+PROFILESPERPAGE) m_profile_profile_downarrow->m_enabled=true;
																			  	   else m_profile_profile_downarrow->m_enabled=false;	
			check_for_profiles_to_load=true;
		} // if 

		if (ID==4) {
			m_profile_first_profile++;
			if (m_profile_first_profile==0) m_profile_profile_uparrow->m_enabled=false;
								 else m_profile_profile_uparrow->m_enabled=true;
			if (m_profile_profile_names.Length()>m_profile_first_profile+PROFILESPERPAGE) m_profile_profile_downarrow->m_enabled=true;
																			  	   else m_profile_profile_downarrow->m_enabled=false;
			check_for_profiles_to_load=true;
		} // if 

		// Check to see if the mouse is over a profile:
		m_profile_mouse_over_profile=-1;
		if (mouse_x>=20 && mouse_x<600) {
			if (mouse_y>=48) {
				int selected=(mouse_y-48)/22;

				if (selected>=0 && selected<(m_profile_profile_names.Length()-m_profile_first_profile) && selected<PROFILESPERPAGE) {
					m_profile_mouse_over_profile=selected+m_profile_first_profile;

					if (button!=0) {
						if (m_profile_profile_selected==selected+m_profile_first_profile) {
							m_state_fading=2;
							m_state_fading_cycle=0;
							m_state_selection=1;
						} else {
							m_profile_profile_selected=selected+m_profile_first_profile;
							strcpy(m_profile_name_inputframe->m_editing,m_profile_profile_names[m_profile_profile_selected]);
							m_profile_name_inputframe->m_editing_position=strlen(m_profile_name_inputframe->m_editing);
							m_profile_name_inputframe->m_enabled=true;
						} // if
					} // if
				} // if 
			} // if
		} // if 

		// Check if the name is a valid file:
		{
			FILE *fp;
			bool valid_new_profile_name=true;
			char tmp[256];

			sprintf(tmp,"%splayers/%s.pp",m_player_data_path,m_profile_name_inputframe->m_editing);

            fp=fopen(tmp,"r");
            if (fp!=0) {
                valid_new_profile_name=false;
                fclose(fp);

				m_profile_select_button->m_enabled=true;
			} else {
				m_profile_select_button->m_enabled=false;			
            } /* if */

			if (valid_new_profile_name) {
				fp=fopen(tmp,"w");
				if (fp!=0) {					
					fclose(fp);
					remove(tmp);
				} else {
					valid_new_profile_name=false;
				} /* if */
			} // if 

			if (valid_new_profile_name) m_profile_create_button->m_enabled=true;
								   else m_profile_create_button->m_enabled=false;
		}

	}

	if (m_state_fading==2 && m_state_fading_cycle>25) {
		switch(m_state_selection) {
		case 0:
				{
					m_player_profile=new PlayerProfile(m_profile_name_inputframe->m_editing);
					save_playerprofile();
					return TGL_STATE_INTRO;
				}
				break;
		case 1:
				{
					load_playerprofile(m_profile_name_inputframe->m_editing);
					return TGL_STATE_INTRO;
				}
				break;
		case 2:
				return TGL_STATE_NONE;
				break;
		} // switch
	} // if 

	if (m_state_fading==0 || m_state_fading==2) m_state_fading_cycle++;
	if (m_state_fading==0 && m_state_fading_cycle>25) m_state_fading=1;

	if (check_for_profiles_to_load) {
		int i,j;
		char *info,*name;

		for(j=0,i=m_profile_first_profile;i<m_profile_first_profile+PROFILESPERPAGE && i<m_profile_profile_names.Length();i++,j++) {
			name=m_profile_profile_names[i];
			info=m_profile_profile_info[i];

			if (info==0) {
				FILE *fp=fopen(m_profile_profile_fullnames[i],"rb");
				if (fp!=0) {
					PlayerProfile *pfl=new PlayerProfile(fp);
					fclose(fp);
					
					char *tmp=new char[256];
					{
						sprintf(tmp,"%i points %i ships",pfl->get_points(),pfl->m_ships.Length());
					}

					m_profile_profile_info.SetObj(i,tmp);
					
					delete pfl;
				} // if 
			} // if 
		} // for
	} // if 


	return TGL_STATE_PLAYERPROFILE;
} /* TGLapp::playerprofile_cycle */ 


void TGLapp::playerprofile_draw(void)
{

	glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

	TGLinterface::draw();


	if (m_profile_profile_selected!=-1) {
		float f;
		f=float(0.7+0.2*sin((m_state_cycle)*0.3F));		
		glColor4f(0.5f,0.5f,1,0.7f);
		glBegin(GL_POLYGON);
		glVertex3f(15,float(48+m_profile_profile_selected*22),0);
		glVertex3f(585,float(48+m_profile_profile_selected*22),0);
		glVertex3f(585,float(68+m_profile_profile_selected*22),0);
		glVertex3f(15,float(68+m_profile_profile_selected*22),0);
		glEnd();
	} // if 


	if (m_profile_mouse_over_profile!=-1) {
		float f;
		f=float(0.7+0.2*sin((m_state_cycle)*0.3F));
		glColor4f(0.5f,1,0.5f,f);
		glBegin(GL_POLYGON);
		glVertex3f(15,float(48+m_profile_mouse_over_profile*22),0);
		glVertex3f(585,float(48+m_profile_mouse_over_profile*22),0);
		glVertex3f(585,float(68+m_profile_mouse_over_profile*22),0);
		glVertex3f(15,float(68+m_profile_mouse_over_profile*22),0);
		glEnd();
	} // if 

	// show the list of files:
	{
		int i,j;
		char *info;		

		for(j=0,i=m_profile_first_profile;i<m_profile_first_profile+PROFILESPERPAGE && i<m_profile_profile_names.Length();i++,j++) {
			{
				int old[4];
				bool old_scissor=false;

				glGetIntegerv(GL_SCISSOR_BOX,old);
				if (glIsEnabled(GL_SCISSOR_TEST)) old_scissor=true;
				glEnable(GL_SCISSOR_TEST);
				glScissor(10,0,200,480);
				TGLinterface::print_left(m_profile_profile_names[i],m_font16,20,float(68+j*22));
				glScissor(old[0],old[1],old[2],old[3]);
				if (!old_scissor) glDisable(GL_SCISSOR_TEST);
			}

			TGLinterface::print_left("-",m_font16,220,float(68+j*22));			

			info=m_profile_profile_info[i];
			if (info!=0) {

				TGLinterface::print_left(info,m_font16,250,float(68+j*22));

			} // if 
		} // for
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
} /* TGLapp::playerprofile_draw */ 

