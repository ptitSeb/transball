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



int TGLapp::editor_cycle(KEYBOARDSTATE *k)
{
	int old_mouse_x = m_mouse_x;
	int old_mouse_y = m_mouse_y;

	if (SDL_ShowCursor(SDL_QUERY)!=SDL_ENABLE) SDL_ShowCursor(SDL_ENABLE);

	SDL_GetMouseState(&m_mouse_x,&m_mouse_y);
	if (!m_mouse_click_x.EmptyP()) {
		m_mouse_button=*(m_mouse_click_button[0]);
		m_mouse_click_x.Delete();
		m_mouse_click_y.Delete();
		m_mouse_click_button.Delete();
	} else {
		m_mouse_button=0;
	} // if 

	if (m_state_cycle == 0){

		if (m_editor_levelpack == 0) {
			m_editor_levelpack = new LevelPack();
			m_editor_levelpack->m_creator_nickname=new char[strlen(m_player_profile->m_name)+1];
			strcpy(m_editor_levelpack->m_creator_nickname,m_player_profile->m_name);
			m_editor_levelpack->m_id=new char[3];
			strcpy(m_editor_levelpack->m_id,"id");
			m_editor_levelpack->m_name=new char[8];
			strcpy(m_editor_levelpack->m_name,"no name");
			m_editor_levelpack->m_description=new char[15];
			strcpy(m_editor_levelpack->m_description,"no description");
		} // if
		if (m_previous_state != TGL_STATE_MAPEDITOR) m_editor_level = 0;

		// CREATE THE INTERFACE:
		TGLinterface::reset();
		TGLinterface::add_element(new TGLbutton("Back",m_font32,40,420,160,48,1));
		TGLinterface::add_element(new TGLbutton("Load",m_font32,240,420,160,48,2));
		TGLinterface::add_element(new TGLbutton("Save",m_font32,440,420,160,48,3));
		TGLinterface::add_element(new TGLText("Transball GL Level Editor",m_font32,320,16,true));
		TGLinterface::add_element(new TGLframe(24,206,416,206));
		TGLinterface::add_element(new TGLBrowser(m_font16,32,214,400,190,11));

		TGLinterface::add_element(new TGLText("Pack ID:",m_font16,8,75,false));
		TGLinterface::add_element(new TGLTextInputFrame(m_editor_levelpack->m_id,16,m_font16,90,55,220,32,12));
		TGLinterface::add_element(new TGLText("Pack Name:",m_font16,8,115,false));
		TGLinterface::add_element(new TGLTextInputFrame(m_editor_levelpack->m_name,32,m_font16,90,95,220,32,13));
		TGLinterface::add_element(new TGLText("Pack",m_font16,8,155,false));
		TGLinterface::add_element(new TGLText("Description:",m_font16,8,170,false));
		TGLinterface::add_element(new TGLTextInputFrame(m_editor_levelpack->m_description,256,m_font16,90,135,220,56,14));

		TGLinterface::add_element(new TGLText("Level Name:",m_font16,328,115,false));
		TGLinterface::add_element(new TGLTextInputFrame("",32,m_font16,410,95,220,32,16));
		TGLinterface::add_element(new TGLText("Level",m_font16,328,155,false));
		TGLinterface::add_element(new TGLText("Description:",m_font16,328,170,false));
		TGLinterface::add_element(new TGLTextInputFrame("",256,m_font16,410,135,220,56,17));


		TGLinterface::add_element(new TGLbutton("Edit",m_font32,480,215,128,48,10));
		TGLinterface::add_element(new TGLbutton("Add level",m_font16,480,315,128,24,4));
		TGLinterface::add_element(new TGLbutton("Delete level",m_font16,480,345,128,24,5));
		TGLinterface::add_element(new TGLbutton("Up",m_font16,480,375,60,24,6));
		TGLinterface::add_element(new TGLbutton("Down",m_font16,548,375,60,24,7));

		TGLinterface::disable(5);
		TGLinterface::disable(6);
		TGLinterface::disable(7);
		TGLinterface::disable(8);
		TGLinterface::disable(10);

		TGLinterface::disable(15);

		TGLinterface::disable(16);
		TGLinterface::disable(17);

	} else {

		// UPDATE THE INTERFACE:
		if (m_editor_level == 0) {
			TGLinterface::disable(5);
			TGLinterface::disable(6);
			TGLinterface::disable(7);
			TGLinterface::disable(8);
			TGLinterface::disable(10);
		} else {
			TGLinterface::enable(5);
			if (m_editor_levelpack->getLevelPosition(m_editor_level)>0) TGLinterface::enable(6);
																   else TGLinterface::disable(6);
			if (m_editor_levelpack->getLevelPosition(m_editor_level)<(m_editor_levelpack->getNLevels()-1)) TGLinterface::enable(7);
																									  else TGLinterface::disable(7);
			TGLinterface::enable(8);
			TGLinterface::enable(10);
		} // if 

		// Synchronize text edit boxes with stored data:
		{
			TGLTextInputFrame *ti = (TGLTextInputFrame *)TGLinterface::get(12);

			delete []m_editor_levelpack->m_id;
			m_editor_levelpack->m_id=new char[strlen(ti->m_editing)+1];
			strcpy(m_editor_levelpack->m_id,ti->m_editing);

			ti = (TGLTextInputFrame *)TGLinterface::get(13);
			delete []m_editor_levelpack->m_name;
			m_editor_levelpack->m_name=new char[strlen(ti->m_editing)+1];
			strcpy(m_editor_levelpack->m_name,ti->m_editing);

			ti = (TGLTextInputFrame *)TGLinterface::get(14);
			delete []m_editor_levelpack->m_description;
			m_editor_levelpack->m_description=new char[strlen(ti->m_editing)+1];
			strcpy(m_editor_levelpack->m_description,ti->m_editing);
		}
		if (m_editor_level==0) {
			TGLinterface::disable(16);
			TGLinterface::disable(17);
		} else {
			if (TGLinterface::is_enabled(16)) {
				TGLTextInputFrame *ti = (TGLTextInputFrame *)TGLinterface::get(16);

				delete []m_editor_level->m_name;
				m_editor_level->m_name=new char[strlen(ti->m_editing)+1];
				strcpy(m_editor_level->m_name,ti->m_editing);

				ti = (TGLTextInputFrame *)TGLinterface::get(17);
				delete []m_editor_level->m_description;
				m_editor_level->m_description=new char[strlen(ti->m_editing)+1];
				strcpy(m_editor_level->m_description,ti->m_editing);

			} else {
				TGLinterface::enable(16);
				TGLinterface::enable(17);
			
				TGLTextInputFrame *ti = (TGLTextInputFrame *)TGLinterface::get(16);
				strcpy(ti->m_editing,m_editor_level->m_name);
				ti->m_editing_position=strlen(ti->m_editing);

				ti = (TGLTextInputFrame *)TGLinterface::get(17);
				strcpy(ti->m_editing,m_editor_level->m_description);
				ti->m_editing_position=strlen(ti->m_editing);

			} // if
		} //

		// Update the list of names in the browser:
		{
			TGLBrowser *browser = (TGLBrowser *)TGLinterface::get(11);
			if (browser!=0) {
				int i;
				int nl = m_editor_levelpack->getNLevels();
				
				browser->clear();
				for(i=0;i<nl;i++) {
					browser->addEntry(m_editor_levelpack->getLevel(i)->m_name);
				} // for

				if (m_editor_level==0) browser->setSelected(-1);
								  else browser->setSelected(m_editor_levelpack->getLevelPosition(m_editor_level));
			} // if 
		}

	} // if 

	{
		int button_status=0;
		int ID=-1;
		button_status=SDL_GetMouseState(&m_mouse_x,&m_mouse_y);

		if (k->key_press(SDLK_SPACE) || k->key_press(SDLK_RETURN)) m_mouse_button=1;

		ID = TGLinterface::update_state(m_mouse_x,m_mouse_y,m_mouse_button,button_status,k);

		switch(ID) {
		case 1:	// BACK:
				return TGL_STATE_MAINMENU;
				break;

		case 2: // LOAD
				TGLinterface::add_element(new TGLConfirmation("Discard current level pack?",m_font16,320,200,106));
				break;
		case 106:
				return TGL_STATE_LOADLEVELPACK;
				break;

		case 3: // SAVE
				{
					char filename[80];
					FILE *fp;

					sprintf(filename,"maps/%s.lp",m_editor_levelpack->m_id);

					fp=fopen(filename,"r");
					if (fp!=0) {
						TGLinterface::add_element(new TGLConfirmation("Overwrite?",m_font16,320,200,105));
						fclose(fp);
					} else {
						// Save:
						fp=fopen(filename,"w");
						if (fp!=0) {
							m_editor_levelpack->save(fp,m_GLTM);
							fclose(fp);
						} else {
							TGLinterface::add_element(new TGLConfirmation("Error saving level pack",m_font16,320,200,-1));
						} // if 
					} // if 
				}					
				break;
		case 105: // SAVE (Without check for overwrite)
				{
					char filename[80];
					FILE *fp;

					sprintf(filename,"maps/%s.lp",m_editor_levelpack->m_id);

					// Save:
					fp=fopen(filename,"w");
					if (fp!=0) {
						m_editor_levelpack->save(fp,m_GLTM);
						fclose(fp);
					} else {
						TGLinterface::add_element(new TGLConfirmation("Error saving level pack",m_font16,320,200,-1));
					} // if 
				}	
				break;

		case 4: // ADD MAP:
				{
					char tmp[80];
					LevelPack_Level *l=new LevelPack_Level(m_GLTM);
					int i=1;
					do{
						sprintf(tmp,"level-%i",i);
						i++;
					}while(m_editor_levelpack->getLevel(tmp)!=0);
					l->setName(tmp);
					l->m_description=new char[1];
					strcpy(l->m_description,"");
					m_editor_levelpack->m_levels.Add(l);
				}
				break;

		case 5: // DELETE MAP:
				TGLinterface::add_element(new TGLConfirmation("Delete level?",m_font16,320,200,104));
				break;

		case 6: // UP:
				{
					int pos = m_editor_levelpack->getLevelPosition(m_editor_level);
					LevelPack_Level *l;
					if (pos>0) {
						l = m_editor_levelpack->getLevel(pos-1);
						m_editor_levelpack->m_levels.SetObj(pos-1,m_editor_level);
						m_editor_levelpack->m_levels.SetObj(pos,l);
					} // if 
				}
				break;

		case 7: // DOWN:
				{
					int pos = m_editor_levelpack->getLevelPosition(m_editor_level);
					LevelPack_Level *l;
					if (pos<(m_editor_levelpack->getNLevels()-1)) {
						l = m_editor_levelpack->getLevel(pos+1);
						m_editor_levelpack->m_levels.SetObj(pos+1,m_editor_level);
						m_editor_levelpack->m_levels.SetObj(pos,l);
					} // if 
				}
				break;

		case 10: // EDIT
				return TGL_STATE_MAPEDITOR;
				break;

		case 11: // LEVEL SELECTED:
				{
					TGLBrowser *browser = (TGLBrowser *)TGLinterface::get(11);
					if (browser!=0) {
						int i = browser->getSelected();
						if (i>=0) {
							m_editor_level = m_editor_levelpack->getLevel(i);
							TGLinterface::disable(16);
							TGLinterface::disable(17);
						} // if 
					} // if 
				} 
				break;
		case 104: // DELETE MAP:
				{
					m_editor_levelpack->m_levels.DeleteElement(m_editor_level);
					delete m_editor_level;
					m_editor_level = 0;
					TGLinterface::disable(16);
					TGLinterface::disable(17);
				}
				break;

		} // switch
	}


    return TGL_STATE_EDITOR;
} /* TGLapp::editor_cycle */ 

void TGLapp::editor_draw(void)
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

	TGLinterface::draw();

} /* TGLapp::editor_draw */ 

