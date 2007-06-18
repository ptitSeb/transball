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
#include "TGLreplay.h"
#include "TGLinterface.h"

#include "LevelPack.h"
#include "PlayerProfile.h"


int TGLapp::highscores_times_cycle(KEYBOARDSTATE *k)
{
	bool recompute_table = false;
	if (SDL_ShowCursor(SDL_QUERY)!=SDL_ENABLE) SDL_ShowCursor(SDL_ENABLE);

	if (m_state_cycle==0) {
	
		// compute the highscore matrix:
		{
			// Load the list of level packs:
			{
				char *tmp;

				m_highscores_lp_names.Delete();
				m_highscores_level_pack=0;

#ifdef _WIN32
				/* Find files: */
				WIN32_FIND_DATA finfo;
				HANDLE h;

				h = FindFirstFile("maps/*.lp", &finfo);
				if (h != INVALID_HANDLE_VALUE) {
					tmp=new char[strlen(finfo.cFileName)+1];
					strcpy(tmp,finfo.cFileName);
					m_highscores_lp_names.Add(tmp);

					while (FindNextFile(h, &finfo) == TRUE) {
						tmp=new char[strlen(finfo.cFileName)+1];
						strcpy(tmp,finfo.cFileName);
						m_highscores_lp_names.Add(tmp);
					} /* while */
				} /* if */
#else
				DIR *dp;
				struct dirent *ep;

				dp = opendir ("maps");
				if (dp != NULL) {
					while (ep = readdir (dp)) {
						if (strlen(ep->d_name) > 4 &&
								ep->d_name[strlen(ep->d_name) - 3] == '.' &&
								ep->d_name[strlen(ep->d_name) - 2] == 'l' &&
								ep->d_name[strlen(ep->d_name) - 1] == 'p') {

							tmp=new char[strlen(ep->d_name)+1];
							strcpy(tmp,ep->d_name);
							m_highscores_lp_names.Add(tmp);
						} /* if */

					} /* while */
					(void) closedir (dp);
				} /* if */
#endif
			}

			m_highscores_level_pack=m_highscores_lp_names[0];

			recompute_table=true;

		}

		m_highscores_first_ship=0;
		m_highscores_first_level=0;

		TGLinterface::reset();
		TGLinterface::add_element(new TGLbutton("Back",m_font32,10,10,160,50,0));
		TGLinterface::add_element(new TGLframe(10,70,580,325));
		TGLinterface::add_element(new TGLText("Top Times:",m_font32,410,20,true));
		TGLinterface::add_element(new TGLText("---",m_font16,410,50,true,-10));

		TGLinterface::add_element(new TGLbutton("Best Players",m_font32,10,410,200,50,4));
		TGLinterface::add_element(new TGLbutton("Best Times",m_font32,230,410,200,50,5));
		TGLinterface::enable(4);
		TGLinterface::disable(5);

		TGLinterface::add_element(new TGLbutton(m_GLTM->get("interface/uparrow"),600,70,30,100,2));
		TGLinterface::disable(2);
		TGLinterface::add_element(new TGLbutton(m_GLTM->get("interface/downarrow"),600,290,30,100,3));
		TGLinterface::disable(3);

		TGLinterface::add_element(new TGLbutton(m_GLTM->get("interface/leftarrow"),280,20,40,40,6));
		TGLinterface::disable(6);
		TGLinterface::add_element(new TGLbutton(m_GLTM->get("interface/rightarrow"),500,20,40,40,7));
		if (m_highscores_lp_names.PositionRef(m_highscores_level_pack)>=m_highscores_lp_names.Length()-1) TGLinterface::disable(7);
																		 						     else TGLinterface::enable(7);

		TGLinterface::add_element(new TGLbutton(m_GLTM->get("interface/leftarrow"),300,85,40,40,8));
		TGLinterface::disable(8);
		TGLinterface::add_element(new TGLbutton(m_GLTM->get("interface/rightarrow"),480,85,40,40,9));
		TGLinterface::enable(9);

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
			button=1;
		} else {
			button_status=SDL_GetMouseState(&mouse_x,&mouse_y);
			button=0;
		} // if 

		if (k->key_press(SDLK_SPACE) || k->key_press(SDLK_RETURN)) button=1;

		ID=TGLinterface::update_state(mouse_x,mouse_y,button,button_status,k);

		if (ID!=-1) {
			switch(ID) {
			case 0:
					m_state_fading=2;
					m_state_fading_cycle=0;
					m_state_selection=ID;
					break;

					// UP / DOWN
			case 2: m_highscores_first_level--;
					if (m_highscores_first_level<=0) TGLinterface::disable(2);
												else TGLinterface::enable(2);
					if (m_highscores_first_level>=m_highscores_times_names[0].Length()-3) TGLinterface::disable(3);
																					 else TGLinterface::enable(3);
					break;
			case 3: m_highscores_first_level++;
					if (m_highscores_first_level<=0) TGLinterface::disable(2);
												else TGLinterface::enable(2);
					if (m_highscores_first_level>=m_highscores_times_names[0].Length()-3) TGLinterface::disable(3);
																					 else TGLinterface::enable(3);					
					break;
			case 4:
					m_state_fading=2;
					m_state_fading_cycle=0;
					m_state_selection=ID;
					TGLinterface::disable(4);
					TGLinterface::disable(5);
					break;
			case 5:
					break;

					// CHANGE LEVEL PACK:
			case 6:	m_highscores_level_pack = m_highscores_lp_names[m_highscores_lp_names.PositionRef(m_highscores_level_pack)-1];
					if (m_highscores_lp_names.PositionRef(m_highscores_level_pack)<=0) TGLinterface::disable(6);
																		 		  else TGLinterface::enable(6);
					if (m_highscores_lp_names.PositionRef(m_highscores_level_pack)>=m_highscores_lp_names.Length()-1) TGLinterface::disable(7);
																		 										 else TGLinterface::enable(7);
					recompute_table = true;
					break;
			case 7:	m_highscores_level_pack = m_highscores_lp_names[m_highscores_lp_names.PositionRef(m_highscores_level_pack)+1];
					if (m_highscores_lp_names.PositionRef(m_highscores_level_pack)<=0) TGLinterface::disable(6);
																		 		  else TGLinterface::enable(6);
					if (m_highscores_lp_names.PositionRef(m_highscores_level_pack)>=m_highscores_lp_names.Length()-1) TGLinterface::disable(7);
																		 										 else TGLinterface::enable(7);
					recompute_table = true;
					break;

					// BROWSE SHIPS:
			case 8:	m_highscores_first_ship--;
					if (m_highscores_first_ship<=0) TGLinterface::disable(8);
									   		   else TGLinterface::enable(8);
					if (m_highscores_first_ship>=N_SHIPS-1) TGLinterface::disable(9);
													   else TGLinterface::enable(9);
					break;
			case 9:	m_highscores_first_ship++;
					if (m_highscores_first_ship<=0) TGLinterface::disable(8);
									   		   else TGLinterface::enable(8);
					if (m_highscores_first_ship>=N_SHIPS-1) TGLinterface::disable(9);
													   else TGLinterface::enable(9);
					break;

			} // switch
		} // if 
	}

	if (recompute_table) {
		LevelPack *lp;
		int i;
		for(i=0;i<N_SHIPS+1;i++) {
			m_highscores_times_names[i].Delete();
			m_highscores_times_time[i].Delete();
		} // for

		// Load the level pack
		{
			char *tmp=new char[strlen(m_highscores_level_pack)+9];
			sprintf(tmp,"maps/%s",m_highscores_level_pack);
			{
				FILE *fp;
				fp=fopen(tmp,"rb");
				if (fp!=0) {
					lp=new LevelPack(fp);
					fclose(fp);
				} else {
					lp=0;
				} // if 
			}
		}

		{
			TGLInterfaceElement *e;
			char *tmp;
			tmp = new char[strlen(lp->m_name)+1];
			strcpy(tmp,lp->m_name);
			e = TGLinterface::get(-10);
			TGLinterface::substitute_element(e,new TGLText(tmp,m_font16,410,50,true,-10));
			delete e;
		}

		// Initialize the table:
		if (lp!=0) {
			int j;

			for(i=0;i<N_SHIPS+1;i++) {
				for(j=0;j<lp->m_levels.Length();j++) {
					m_highscores_times_names[i].Add(new List<char>);
					m_highscores_times_time[i].Add(new List<int>);
				} // for
			} // for

		} // if

		// Load player by player and fill the table:
		{
			char *tmp;
			char buf[256];

				char *short_ship_names[]={"V-P",
										  "X-T",
										  "SR",
										  "NB",
										  "VB",
										  "DDG",
										  "GRV",
										  "ACC",
									  	  "GYR",
										  "D-F",
										  "C-H",
									};


			m_highscores_names.Delete();

#ifdef _WIN32
			/* Find files: */
			WIN32_FIND_DATA finfo;
			HANDLE h;

			h = FindFirstFile("players/*.pp", &finfo);
			if (h != INVALID_HANDLE_VALUE) {
				tmp=new char[strlen(finfo.cFileName)+1];
				strcpy(tmp,finfo.cFileName);
				tmp[strlen(tmp)-3]=0;
				m_highscores_names.Add(tmp);

				while (FindNextFile(h, &finfo) == TRUE) {
					tmp=new char[strlen(finfo.cFileName)+1];
					strcpy(tmp,finfo.cFileName);
					tmp[strlen(tmp)-3]=0;
					m_highscores_names.Add(tmp);
				} /* while */
			} /* if */
#else
			DIR *dp;
			struct dirent *ep;

			dp = opendir ("players");
			if (dp != NULL) {
				while (ep = readdir (dp)) {
					if (strlen(ep->d_name) > 4 &&
							ep->d_name[strlen(ep->d_name) - 3] == '.' &&
							ep->d_name[strlen(ep->d_name) - 2] == 'p' &&
							ep->d_name[strlen(ep->d_name) - 1] == 'p') {

						tmp=new char[strlen(ep->d_name)+1];
						strcpy(tmp,ep->d_name);
						tmp[strlen(tmp)-3]=0;
						m_highscores_names.Add(tmp);
					} /* if */

				} /* while */
				(void) closedir (dp);
			} /* if */
#endif

			m_highscores_names.Rewind();
			while(m_highscores_names.Iterate(tmp)) {

				sprintf(buf,"players/%s.pp",tmp);
				FILE *fp=fopen(buf,"rb");
				if (fp!=0) {
					PlayerProfileLevelResult *lr;
					PlayerProfileLPProgress *lpp;
					PlayerProfile *pfl=new PlayerProfile(fp);
					char *tmp;
					fclose(fp);

					pfl->m_progress.Rewind();
					while(pfl->m_progress.Iterate(lpp)) {
						if (strcmp(lpp->m_levelpack_id,lp->m_id)==0) {
							lpp->m_results.Rewind();
							while(lpp->m_results.Iterate(lr)) {
								// Insert new time:
								tmp = new char[strlen(pfl->m_name)+1];
								strcpy(tmp,pfl->m_name);
								m_highscores_times_names[lr->m_ship][lr->m_level]->Add(tmp);
								m_highscores_times_time[lr->m_ship][lr->m_level]->Add(new int(lr->m_best_time));

								tmp = new char[strlen(pfl->m_name)+7];
								sprintf(tmp,"%s [%s]",pfl->m_name,short_ship_names[lr->m_ship]);
								m_highscores_times_names[N_SHIPS][lr->m_level]->Add(tmp);
								m_highscores_times_time[N_SHIPS][lr->m_level]->Add(new int(lr->m_best_time));

								// Sort the list:
								{
									// Bubble sort:
									bool changes = false;
									bool exchange = false;
									int i,j,*iptr;
									char *cptr;

									do {
										changes = false;
										j = 0;
										for(i=j;i<m_highscores_times_names[lr->m_ship][lr->m_level]->Length()-1;i++) {
											exchange = false;
											if (*(m_highscores_times_time[lr->m_ship][lr->m_level]->operator[](i+1))<*(m_highscores_times_time[lr->m_ship][lr->m_level]->operator[](i))) exchange = true;

											if (exchange) {
												cptr = m_highscores_times_names[lr->m_ship][lr->m_level]->operator[](i+1);
												m_highscores_times_names[lr->m_ship][lr->m_level]->SetObj(i+1,m_highscores_times_names[lr->m_ship][lr->m_level]->operator[](i));
												m_highscores_times_names[lr->m_ship][lr->m_level]->SetObj(i,cptr);
												
												iptr = m_highscores_times_time[lr->m_ship][lr->m_level]->operator[](i+1);
												m_highscores_times_time[lr->m_ship][lr->m_level]->SetObj(i+1,m_highscores_times_time[lr->m_ship][lr->m_level]->operator[](i));
												m_highscores_times_time[lr->m_ship][lr->m_level]->SetObj(i,iptr);

												changes = true;
											} // if

										} // for
										j++;
									}while(changes);
								}

								{
									// Bubble sort:
									bool changes = false;
									bool exchange = false;
									int i,j,*iptr;
									char *cptr;

									do {
										changes = false;
										j = 0;
										for(i=j;i<m_highscores_times_names[N_SHIPS][lr->m_level]->Length()-1;i++) {
											exchange = false;
											if (*(m_highscores_times_time[N_SHIPS][lr->m_level]->operator[](i+1))<*(m_highscores_times_time[N_SHIPS][lr->m_level]->operator[](i))) exchange = true;

											if (exchange) {
												cptr = m_highscores_times_names[N_SHIPS][lr->m_level]->operator[](i+1);
												m_highscores_times_names[N_SHIPS][lr->m_level]->SetObj(i+1,m_highscores_times_names[N_SHIPS][lr->m_level]->operator[](i));
												m_highscores_times_names[N_SHIPS][lr->m_level]->SetObj(i,cptr);
												
												iptr = m_highscores_times_time[N_SHIPS][lr->m_level]->operator[](i+1);
												m_highscores_times_time[N_SHIPS][lr->m_level]->SetObj(i+1,m_highscores_times_time[N_SHIPS][lr->m_level]->operator[](i));
												m_highscores_times_time[N_SHIPS][lr->m_level]->SetObj(i,iptr);

												changes = true;
											} // if

										} // for
										j++;
									}while(changes);
								}

							} // while 
						} // if 
					} // while
				
					delete pfl;
				} else {
				} // if 
			} // while
		}

		if (lp!=0) delete lp;
		recompute_table=false;
		m_highscores_first_level=0;
		if (m_highscores_first_level<=0) TGLinterface::disable(2);
									else TGLinterface::enable(2);
		if (m_highscores_first_level>=m_highscores_times_names[0].Length()-3) TGLinterface::disable(3);
																		 else TGLinterface::enable(3);
	} // if


	if (m_state_fading==2 && m_state_fading_cycle>25) {
		SDL_ShowCursor(SDL_DISABLE);
		switch(m_state_selection) {
		case 0: if (m_lp_music_channel!=-1) Mix_HaltChannel(m_lp_music_channel);
				m_lp_music_channel=-1;
				return TGL_STATE_MAINMENU;
				break;
		case 4: return TGL_STATE_HIGHSCORES;
				break;
		case 5: return TGL_STATE_HIGHSCORES_TIMES;
				break;
		} // switch
	} // if 

	if (m_state_fading==0 || m_state_fading==2) m_state_fading_cycle++;
	if (m_state_fading==0 && m_state_fading_cycle>25) m_state_fading=1;

	if (m_state_fading==0 && m_lp_music_channel!=-1) Mix_Volume(m_lp_music_channel, int(m_player_profile->m_music_volume*(m_state_fading_cycle/25.0f)));
	if (m_state_fading==1 && m_lp_music_channel!=-1) Mix_Volume(m_lp_music_channel, m_player_profile->m_music_volume);
	if (m_state_fading==2 && m_lp_music_channel!=-1) Mix_Volume(m_lp_music_channel, int(m_player_profile->m_music_volume*((25-m_state_fading_cycle)/25.0f)));

	return TGL_STATE_HIGHSCORES_TIMES;
} /* TGLapp::highscores_times_cycle */ 


void TGLapp::highscores_times_draw(void)
{
	glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

	TGLinterface::draw();

	// show the "Best Times" highscores:
	{
		int j,k;
		int ship,level;
		int NLEVELS = 3, NRESULTS = 3;
		int start_y = 95;
		char tmp[256];

		for(j=0;j<NLEVELS && j+m_highscores_first_level<m_highscores_times_names[0].Length();j++) {
			level=j+m_highscores_first_level;
			
			sprintf(tmp,"Level %i",level+1);
			TGLinterface::print_left(tmp,m_font16,15,float(start_y+60+j*80));

			glColor3f(1,1,1);			
			glBegin(GL_LINES);
			glVertex3f(30,float(start_y+35+j*80),0);
			glVertex3f(560,float(start_y+35+j*80),0);

			glEnd();
		} // for

		{
			ship=m_highscores_first_ship;
			{
				char *ship_tiles[]={"objects/ship-vpanther-1",
									"objects/ship-xterminator-1",
									"objects/ship-srunner-1",
									"objects/ship-nblaster-1",
									"objects/ship-vbeam-1",
									"objects/ship-dodger-1",
									"objects/ship-gravis-1",
									"objects/ship-accura-1",
									"objects/ship-gyrus",
									"objects/ship-dflecter-1",
									"objects/ship-harpoon",
									};
				char *ship_names[]={"V-Panther",
									"X-Terminator",
									"Shadow Runner",
									"Nitro Blaster",
									"Vipper Beam",
									"Dodger K7",
									"Gravis T8",
									"Accura T5",
									"Gyrus-P",
									"D-Flecter",
									"C-Harpoon",
									};

				GLTile *t;

				{
					t=m_GLTM->get(ship_tiles[ship]);

					t->draw(float(150+260),float(start_y),0,0,0.6f);

					TGLinterface::print_center(ship_names[ship],m_font16,float(150+260),float(start_y+30));
	
					TGLinterface::print_center("Any ship",m_font16,float(150),float(start_y+30));
				} // if
			}

			for(j=0;j<NLEVELS && j+m_highscores_first_level<m_highscores_times_names[0].Length();j++) {
				level=j+m_highscores_first_level;
				
				for(k=0;k<NRESULTS;k++) {
					if (k<m_highscores_times_names[N_SHIPS][level]->Length()) {
						int days,hours,mins,secs,milis;
						milis=*(m_highscores_times_time[N_SHIPS][level]->operator [](k));
						if (milis<0) {
							sprintf(tmp,"(%i) - %s - ---",k+1,m_highscores_times_names[N_SHIPS][level]->operator [](k));
						} else {
							secs=milis/1000;
							milis=milis%1000;
							mins=secs/60;
							secs=secs%60;
							hours=mins/60;
							mins=mins%60;
							days=hours/24;
							hours=hours%24;
							if (days>0) {
								sprintf(tmp,"(%i) - %id %ih %im:%.2is:%.2i - %s",k+1,days,hours,mins,secs,milis/10,m_highscores_times_names[N_SHIPS][level]->operator [](k));
							} else {
								if (hours>0) {
									sprintf(tmp,"(%i) - %ih %im:%.2is:%.2i - %s",k+1,hours,mins,secs,milis/10,m_highscores_times_names[N_SHIPS][level]->operator [](k));
								} else {
									sprintf(tmp,"(%i) - %im:%.2is:%.2i - %s",k+1,mins,secs,milis/10,m_highscores_times_names[N_SHIPS][level]->operator [](k));
								} // if 
							} // if 
						} // if

						if (strcmp(m_highscores_times_names[N_SHIPS][level]->operator [](k),get_player_profile()->m_name)==0) {
							TGLinterface::print_left(tmp,m_font16,float(90),float(start_y+60+j*80+k*20),0,1,0,1);
						} else {
							TGLinterface::print_left(tmp,m_font16,float(90),float(start_y+60+j*80+k*20));
						} // if 
					} else {
						sprintf(tmp,"(%i) - --- - ---",k+1);
						TGLinterface::print_left(tmp,m_font16,float(90),float(start_y+60+j*80+k*20));
					} // if
				} // for
			} // for

			for(j=0;j<NLEVELS && j+m_highscores_first_level<m_highscores_times_names[0].Length();j++) {
				level=j+m_highscores_first_level;
				
				for(k=0;k<NRESULTS;k++) {
					if (k<m_highscores_times_names[ship][level]->Length()) {
						int days,hours,mins,secs,milis;
						milis=*(m_highscores_times_time[ship][level]->operator [](k));
						if (milis<0) {
							sprintf(tmp,"(%i) - %s - ---",k+1,m_highscores_times_names[ship][level]->operator [](k));
						} else {
							secs=milis/1000;
							milis=milis%1000;
							mins=secs/60;
							secs=secs%60;
							hours=mins/60;
							mins=mins%60;
							days=hours/24;
							hours=hours%24;
							if (days>0) {
								sprintf(tmp,"(%i) - %id %ih %im:%.2is:%.2i - %s",k+1,days,hours,mins,secs,milis/10,m_highscores_times_names[ship][level]->operator [](k));
							} else {
								if (hours>0) {
									sprintf(tmp,"(%i) - %ih %im:%.2is:%.2i - %s",k+1,hours,mins,secs,milis/10,m_highscores_times_names[ship][level]->operator [](k));
								} else {
									sprintf(tmp,"(%i) - %im:%.2is:%.2i - %s",k+1,mins,secs,milis/10,m_highscores_times_names[ship][level]->operator [](k));
								} // if 
							} // if 
						} // if

						if (strcmp(m_highscores_times_names[ship][level]->operator [](k),get_player_profile()->m_name)==0) {
							TGLinterface::print_left(tmp,m_font16,float(100+260),float(start_y+60+j*80+k*20),0,1,0,1);
						} else {
							TGLinterface::print_left(tmp,m_font16,float(100+260),float(start_y+60+j*80+k*20));
						} // if 
					} else {
						sprintf(tmp,"(%i) - --- - ---",k+1);
						TGLinterface::print_left(tmp,m_font16,float(100+260),float(start_y+60+j*80+k*20));
					} // if

				} // for

			} // for
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
} /* TGLapp::highscores_times_draw */ 

