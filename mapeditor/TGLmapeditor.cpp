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
#include "time.h"

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
#include "debug.h"

#include "GLTManager.h"
#include "SFXManager.h"

#include "TGLmap.h"
#include "TGLmapeditor.h"


TGLmapeditor::TGLmapeditor(void)
{
	m_font=TTF_OpenFont("lucon.ttf",12);

	m_map=new TGLmap(32,32);
	
	m_mode=0;
	m_cycle=0;
	m_state=0;

	m_pointer_x=0;
	m_pointer_y=0;
	sprintf(m_message,"Wellcome");

	zoom=1;

	m_GLTM=new GLTManager();

	// load all the tiles:	
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
				m_tiles.Add(m_GLTM->get(tmp));
				m_tiles_name.Add(new Symbol(tmp));
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
			m_tiles.Add(new GLTile(tmp));
		}
		free(namelist);
	}
#endif  


	// Add objects:
	m_objects_tile.Add(m_GLTM->get("objects/pipe-ball-stand1"));
	m_objects_name.Add(new Symbol("ball-stand"));

	m_objects_tile.Add(m_GLTM->get("objects/red-light1"));
	m_objects_name.Add(new Symbol("red-light"));

	m_objects_tile.Add(m_GLTM->get("objects/pipe-vertical-screen1"));
	m_objects_name.Add(new Symbol("pipe-vertical-screen"));

	m_objects_tile.Add(m_GLTM->get("objects/pipe-horizontal-screen1"));
	m_objects_name.Add(new Symbol("pipe-horizontal-screen"));

	m_objects_tile.Add(m_GLTM->get("objects/radar1"));
	m_objects_name.Add(new Symbol("radar"));

	m_objects_tile.Add(m_GLTM->get("objects/laser-left1"));
	m_objects_name.Add(new Symbol("laser-left"));

	m_objects_tile.Add(m_GLTM->get("objects/laser-right1"));
	m_objects_name.Add(new Symbol("laser-right"));

	m_objects_tile.Add(m_GLTM->get("objects/laser-up1"));
	m_objects_name.Add(new Symbol("laser-up"));

	m_objects_tile.Add(m_GLTM->get("objects/laser-down1"));
	m_objects_name.Add(new Symbol("laser-down"));

	m_objects_tile.Add(m_GLTM->get("objects/cannon-rock-left1"));
	m_objects_name.Add(new Symbol("cannon-rock-left"));

	m_objects_tile.Add(m_GLTM->get("objects/cannon-rock-right1"));
	m_objects_name.Add(new Symbol("cannon-rock-right"));

	m_objects_tile.Add(m_GLTM->get("objects/cannon-rock-down1"));
	m_objects_name.Add(new Symbol("cannon-rock-down"));

	m_objects_tile.Add(m_GLTM->get("objects/cannon-rock-up1"));
	m_objects_name.Add(new Symbol("cannon-rock-up"));

	m_objects_tile.Add(m_GLTM->get("objects/spike-left"));
	m_objects_name.Add(new Symbol("spike-left"));

	m_objects_tile.Add(m_GLTM->get("objects/spike-right"));
	m_objects_name.Add(new Symbol("spike-right"));

	m_objects_tile.Add(m_GLTM->get("objects/fuel-recharge"));
	m_objects_name.Add(new Symbol("fuel-recharge"));

	m_objects_tile.Add(m_GLTM->get("objects/cannon-techno-left1"));
	m_objects_name.Add(new Symbol("cannon-techno-left"));

	m_objects_tile.Add(m_GLTM->get("objects/cannon-techno-right1"));
	m_objects_name.Add(new Symbol("cannon-techno-right"));

	m_objects_tile.Add(m_GLTM->get("objects/cannon-techno-down1"));
	m_objects_name.Add(new Symbol("cannon-techno-down"));

	m_objects_tile.Add(m_GLTM->get("objects/cannon-techno-up1"));
	m_objects_name.Add(new Symbol("cannon-techno-up"));

	m_objects_tile.Add(m_GLTM->get("objects/fastcannon-techno-left1"));
	m_objects_name.Add(new Symbol("fastcannon-techno-left"));

	m_objects_tile.Add(m_GLTM->get("objects/fastcannon-techno-right1"));
	m_objects_name.Add(new Symbol("fastcannon-techno-right"));

	m_objects_tile.Add(m_GLTM->get("objects/fastcannon-techno-down1"));
	m_objects_name.Add(new Symbol("fastcannon-techno-down"));

	m_objects_tile.Add(m_GLTM->get("objects/fastcannon-techno-up1"));
	m_objects_name.Add(new Symbol("fastcannon-techno-up"));

	m_objects_tile.Add(m_GLTM->get("objects/wall-techno-computer1"));
	m_objects_name.Add(new Symbol("wall-techno-computer"));

	m_objects_tile.Add(m_GLTM->get("directionalcannon-left"));
	m_objects_name.Add(new Symbol("directional-cannon-left"));

	m_objects_tile.Add(m_GLTM->get("directionalcannon-right"));
	m_objects_name.Add(new Symbol("directional-cannon-right"));

	m_objects_tile.Add(m_GLTM->get("directionalcannon-down"));
	m_objects_name.Add(new Symbol("directional-cannon-down"));

	m_objects_tile.Add(m_GLTM->get("directionalcannon-up"));
	m_objects_name.Add(new Symbol("directional-cannon-up"));

	m_objects_tile.Add(m_GLTM->get("ha-directionalcannon-left"));
	m_objects_name.Add(new Symbol("ha-directional-cannon-left"));

	m_objects_tile.Add(m_GLTM->get("ha-directionalcannon-right"));
	m_objects_name.Add(new Symbol("ha-directional-cannon-right"));

	m_objects_tile.Add(m_GLTM->get("ha-directionalcannon-down"));
	m_objects_name.Add(new Symbol("ha-directional-cannon-down"));

	m_objects_tile.Add(m_GLTM->get("ha-directionalcannon-up"));
	m_objects_name.Add(new Symbol("ha-directional-cannon-up"));

	m_objects_tile.Add(m_GLTM->get("grey-tank"));
	m_objects_name.Add(new Symbol("grey-tank"));

	m_objects_tile.Add(m_GLTM->get("red-tank"));
	m_objects_name.Add(new Symbol("red-tank"));

	m_objects_tile.Add(m_GLTM->get("green-tank"));
	m_objects_name.Add(new Symbol("green-tank"));

	m_objects_tile.Add(m_GLTM->get("big-tank"));
	m_objects_name.Add(new Symbol("big-tank"));

	m_objects_tile.Add(m_GLTM->get("objects/door-left"));
	m_objects_name.Add(new Symbol("door-left"));

	m_objects_tile.Add(m_GLTM->get("objects/door-right"));
	m_objects_name.Add(new Symbol("door-right"));

	m_objects_tile.Add(m_GLTM->get("objects/button-red-left1"));
	m_objects_name.Add(new Symbol("button-red-left"));

	m_objects_tile.Add(m_GLTM->get("objects/button-red-right1"));
	m_objects_name.Add(new Symbol("button-red-right"));

	m_objects_tile.Add(m_GLTM->get("objects/button-red-down1"));
	m_objects_name.Add(new Symbol("button-red-down"));

	m_objects_tile.Add(m_GLTM->get("objects/button-red-up1"));
	m_objects_name.Add(new Symbol("button-red-up"));

	m_objects_tile.Add(m_GLTM->get("objects/button-purple-left1"));
	m_objects_name.Add(new Symbol("button-purple-left"));

	m_objects_tile.Add(m_GLTM->get("objects/button-purple-right1"));
	m_objects_name.Add(new Symbol("button-purple-right"));

	m_objects_tile.Add(m_GLTM->get("objects/button-purple-down1"));
	m_objects_name.Add(new Symbol("button-purple-down"));

	m_objects_tile.Add(m_GLTM->get("objects/button-purple-up1"));
	m_objects_name.Add(new Symbol("button-purple-up"));

	m_tiles.Rewind();
	m_objects_tile.Rewind();
	m_objects_name.Rewind();

} /* TGLmapeditor::TGLmapeditor */ 


TGLmapeditor::~TGLmapeditor()
{
	TTF_CloseFont(m_font);
	delete m_map;
	delete m_GLTM;
} /* TGLmapeditor::~TGLmapeditor */ 


bool TGLmapeditor::cycle(KEYBOARDSTATE *k)
{
	m_cycle++;

	m_map->cycle();

	switch(m_state) {
	case 0:
			// EDIT MODE:
			{
				switch(m_mode) {
				case 0: if (k->key_press(SDLK_TAB)) {
							m_mode=1;
						} // if
						if (k->key_press(SDLK_PAGEUP)) {
							if (!m_tiles.BeginP()) m_tiles.Prev();
						} // if 
						if (k->key_press(SDLK_PAGEDOWN)) {
							if (!m_tiles.LastP()) m_tiles.Next();
						} // if 

						if (k->key_press(SDLK_SPACE)) {
							int pos=m_pointer_x+m_pointer_y*m_map->get_dx();
							m_map->m_fg_tiles[pos]=m_tiles.GetObj();
						} // if 
						if (k->key_press(SDLK_BACKSPACE) || k->key_press(SDLK_DELETE)) {
							int pos=m_pointer_x+m_pointer_y*m_map->get_dx();
							m_map->m_fg_tiles[pos]=0;
						} // if 
						break;
				case 1: if (k->key_press(SDLK_TAB)) {
							m_mode=0;
						} // if
						if (k->key_press(SDLK_PAGEUP)) {
							if (!m_objects_tile.BeginP()) {
								m_objects_tile.Prev();
								m_objects_name.Prev();
							} // if 
						} // if 
						if (k->key_press(SDLK_PAGEDOWN)) {
							if (!m_objects_tile.LastP()) {
								m_objects_tile.Next();
								m_objects_name.Next();
							} // if
						} // if 

						if (k->key_press(SDLK_SPACE)) {
							List<ObjectPlaceHolder> todelete;
							ObjectPlaceHolder *o;
							m_map->m_fg_objects.Rewind();
							while(m_map->m_fg_objects.Iterate(o)) {
								if (o->m_x==m_pointer_x && o->m_y==m_pointer_y) todelete.Add(o);
							} // while 
							while(!todelete.EmptyP()) {
								o=todelete.ExtractIni();
								m_map->m_fg_objects.DeleteElement(o);
								delete o;
							} // while 	

							o=new ObjectPlaceHolder(m_objects_tile.GetObj(),m_objects_name.GetObj(),m_pointer_x,m_pointer_y);
							if (o->m_name->cmp("door-left")) {
								o->m_nparameters=2;
								o->m_parameters[0]=0;
								o->m_parameters[1]=0;
							} // if 
							if (o->m_name->cmp("door-right")) {
								o->m_nparameters=2;
								o->m_parameters[0]=0;
								o->m_parameters[1]=0;
							} // if 
							if (o->m_name->cmp("button-red-left")) {
								o->m_nparameters=1;
								o->m_parameters[0]=0;
							} // if 
							if (o->m_name->cmp("button-red-right")) {
								o->m_nparameters=1;
								o->m_parameters[0]=0;
							} // if 
							if (o->m_name->cmp("button-red-up")) {
								o->m_nparameters=1;
								o->m_parameters[0]=0;
							} // if 
							if (o->m_name->cmp("button-red-down")) {
								o->m_nparameters=1;
								o->m_parameters[0]=0;
							} // if 
							m_map->m_fg_objects.Add(o);
						} // if

						if (k->key_press(SDLK_BACKSPACE) || k->key_press(SDLK_DELETE)) {
							List<ObjectPlaceHolder> todelete;
							ObjectPlaceHolder *o;
							m_map->m_fg_objects.Rewind();
							while(m_map->m_fg_objects.Iterate(o)) {
								if (o->m_x==m_pointer_x && o->m_y==m_pointer_y) todelete.Add(o);
							} // while 
							while(!todelete.EmptyP()) {
								o=todelete.ExtractIni();
								m_map->m_fg_objects.DeleteElement(o);
								delete o;
							} // while 	
						} // if

						if (k->key_press(SDLK_1)) {
							ObjectPlaceHolder *o;
							m_map->m_fg_objects.Rewind();
							while(m_map->m_fg_objects.Iterate(o)) {
								if (o->m_x==m_pointer_x && o->m_y==m_pointer_y) {
									if (o->m_name->cmp("door-left") ||
										o->m_name->cmp("door-right")) {
										o->m_parameters[0]++;
										if (o->m_parameters[0]>=2) o->m_parameters[0]=0;
									} // if 
									if (o->m_name->cmp("button-red-left") ||
										o->m_name->cmp("button-red-right") ||
										o->m_name->cmp("button-red-down") ||
										o->m_name->cmp("button-red-up")) {
										o->m_parameters[0]++;
										if (o->m_parameters[0]>=8) o->m_parameters[0]=0;
									} // if 
								} // if 
							} // while 
						} // if
						if (k->key_press(SDLK_2)) {
							ObjectPlaceHolder *o;
							m_map->m_fg_objects.Rewind();
							while(m_map->m_fg_objects.Iterate(o)) {
								if (o->m_x==m_pointer_x && o->m_y==m_pointer_y) {
									if (o->m_name->cmp("door-left") ||
										o->m_name->cmp("door-right")) {
										o->m_parameters[1]++;
										if (o->m_parameters[1]>=9) o->m_parameters[1]=0;
									} // if 
								} // if 
							} // while 
						} // if

						break;
				} // switch 

				// Move pointer around:
				if (k->key_press(SDLK_UP)) {
					if (m_pointer_y>0) m_pointer_y--;
				} // if 
				if (k->key_press(SDLK_DOWN)) {
					if (m_pointer_y<m_map->get_dy()-1) m_pointer_y++;
				} // if 
				if (k->key_press(SDLK_LEFT)) {
					if (m_pointer_x>0) m_pointer_x--;
				} // if 
				if (k->key_press(SDLK_RIGHT)) {
					if (m_pointer_x<m_map->get_dx()-1) m_pointer_x++;
				} // if 

				// Change zoom:	
				if (k->key_press(SDLK_COMMA)) {
					if (zoom==1) {
						zoom=2;
					} else if(zoom==0.5f) {
						zoom=1;
					} else if(zoom==0.25f) {
						zoom=0.5f;
					} else if(zoom==0.125f) {
						zoom=0.25f;
					} else if(zoom==0.0625f) {
						zoom=0.125f;
					} // if 
				} // if 
				if (k->key_press(SDLK_PERIOD)) {
					if (zoom==2) {
						zoom=1;
					} else if(zoom==1) {
						zoom=0.5f;
					} else if(zoom==0.5f) {
						zoom=0.25f;
					} else if(zoom==0.25f) {
						zoom=0.125f;
					} else if(zoom==0.125f) {
						zoom=0.0625f;
					} // if 
				} // if 

				// Change background:
				if (k->key_press(SDLK_b)) {
					m_map->m_background++;
					if (m_map->m_background>4) m_map->m_background=0;
				} // if 

				// Change map size:
				if (k->keyboard[SDLK_LCTRL]) {
					if (k->key_press(SDLK_UP) && m_map->get_dy()>15) {
						m_map->set_dy(m_map->get_dy()-1);
					} // if
					if (k->key_press(SDLK_DOWN)) {
						m_map->set_dy(m_map->get_dy()+1);
					} // if
					if (k->key_press(SDLK_LEFT) && m_map->get_dx()>20) {
						m_map->set_dx(m_map->get_dx()-1);
					} // if
					if (k->key_press(SDLK_RIGHT)) {
						m_map->set_dx(m_map->get_dx()+1);
					} // if
				} // if

				// Load/save:
				if (k->keyboard[SDLK_LCTRL]) {
					if (k->key_press(SDLK_s)) {
						sprintf(m_editing_text,"map.tgl");
						m_editing_position=strlen(m_editing_text);
						m_state=2;
					} // if

					if (k->key_press(SDLK_l)) {
						sprintf(m_editing_text,"map.tgl");
						m_editing_position=strlen(m_editing_text);
						m_state=1;
					} // if

					if (k->key_press(SDLK_i)) {
						sprintf(m_editing_text,"map.map");
						m_editing_position=strlen(m_editing_text);
						m_state=3;
					} // if
				} // if

				if (k->key_press(SDLK_ESCAPE)) return false;
			}
			break;
	case 1:
	case 2:
	case 3:
			// EDIT TEXT MODE:
			{
				SDL_keysym *ke;

				while(!k->keyevents.EmptyP()) {
					ke=k->keyevents.ExtractIni();
					if (ke->unicode>=0x20 && ke->unicode<=0x7E && strlen(m_editing_text)<32) {
						if (m_editing_position<strlen(m_editing_text)) {
							for(int i=strlen(m_editing_text)+1;i>m_editing_position;i--) m_editing_text[i]=m_editing_text[i-1];
							m_editing_text[m_editing_position]=ke->unicode;
							m_editing_position++;
						} else {
							m_editing_text[m_editing_position]=ke->unicode;
							m_editing_position++;
							m_editing_text[m_editing_position]=0;
						} /* if */ 
					} /* if */ 

					if (ke->sym==SDLK_BACKSPACE) {
						if (m_editing_position>0) {
							strcpy(m_editing_text+m_editing_position-1,m_editing_text+m_editing_position);
							m_editing_position--;
						} /* if */ 
					} /* if */ 

					if (ke->sym==SDLK_DELETE) {
						if (m_editing_position<strlen(m_editing_text)) {
							strcpy(m_editing_text+m_editing_position,m_editing_text+m_editing_position+1);
						} /* if */ 
					} /* if */ 		

					if (ke->sym==SDLK_LEFT) {
						if (m_editing_position>0) {
							m_editing_position--;
						} /* if */ 
					} /* if */ 
					if (ke->sym==SDLK_RIGHT) {
						if (m_editing_position<strlen(m_editing_text)) {
							m_editing_position++;
						} /* if */ 
					} /* if */ 

					if (ke->sym==SDLK_HOME) m_editing_position=0;
					if (ke->sym==SDLK_END) m_editing_position=strlen(m_editing_text);
				
					if (ke->sym==SDLK_RETURN) {
						if (m_state==1) {
							// load:
							int i,j,no;
							int tmpdx,tmpdy,p1,p2;
							Symbol *symtmp;
							char tmp[256];
							FILE *fp;
							ObjectPlaceHolder *o;

							fp=fopen(m_editing_text,"r+");
							if (fp!=0) {
								delete m_map;

								fscanf(fp,"%i %i",&tmpdx,&tmpdy);
								
								m_map=new TGLmap(tmpdx,tmpdy);

								fscanf(fp,"%s",tmp);
								if (strcmp(tmp,"rock")==0) m_map->m_background=0;
								if (strcmp(tmp,"techno-red")==0) m_map->m_background=1;
								if (strcmp(tmp,"techno-blue")==0) m_map->m_background=2;
								if (strcmp(tmp,"techno-green")==0) m_map->m_background=3;
								if (strcmp(tmp,"snow")==0) m_map->m_background==4;

								for(i=0;i<m_map->get_dx()*m_map->get_dy();i++) {
									fscanf(fp,"%s",tmp);
									if (strcmp(tmp,"-")!=0) {
										symtmp=new Symbol(tmp);
										j=m_tiles_name.Position(symtmp);
										delete symtmp;
										if (j>=0) m_map->m_fg_tiles[i]=m_tiles[j];
									} // if 
								} // for

								fscanf(fp,"%i\n",&no);
								for(i=0;i<no;i++) {
									fscanf(fp,"%s",tmp);
									fscanf(fp,"%i %i",&tmpdx,&tmpdy);
									symtmp=new Symbol(tmp);
									j=m_objects_name.Position(symtmp);
									delete symtmp;
									if (j>=0) {
										o=new ObjectPlaceHolder(m_objects_tile[j],m_objects_name[j],tmpdx,tmpdy);
										m_map->m_fg_objects.Add(o);
										if (strcmp(tmp,"door-left")==0 ||
											strcmp(tmp,"door-right")==0) {
											fscanf(fp,"%i %i",&p1,&p2);
											o->m_nparameters=2;
											o->m_parameters[0]=p1;
											o->m_parameters[1]=p2;
										} // if 
										if (strcmp(tmp,"button-red-left")==0 ||
											strcmp(tmp,"button-red-right")==0 ||
											strcmp(tmp,"button-red-up")==0 ||
											strcmp(tmp,"button-red-down")==0) {
											fscanf(fp,"%i",&p1);
											o->m_nparameters=1;
											o->m_parameters[0]=p1;
										} // if 
									} // if 
								} // for
								fclose(fp);
								sprintf(m_message,"Success");
								m_pointer_x=m_pointer_y=0;
							} else {
								sprintf(m_message,"Error");
							} // if
						} // if
						
						if (m_state==2) {
							// save:
							int i;
							FILE *fp;
							fp=fopen(m_editing_text,"w+");
							if (fp!=0) {
								fprintf(fp,"%i %i\n",m_map->get_dx(),m_map->get_dy());
								switch(m_map->m_background) {
								case 0:	fprintf(fp,"rock\n");
										break;
								case 1:	fprintf(fp,"techno-red\n");
										break;
								case 2:	fprintf(fp,"techno-blue\n");
										break;
								case 3:	fprintf(fp,"techno-green\n");
										break;
								case 4:	fprintf(fp,"snow\n");
										break;
								} // switch

								for(i=0;i<m_map->get_dx()*m_map->get_dy();i++) {
									if (m_map->m_fg_tiles[i]==0) {
										fprintf(fp,"-\n");
									} else {
										fprintf(fp,"%s\n",m_tiles_name[m_tiles.PositionRef(m_map->m_fg_tiles[i])]->get());
									} // if 
								} // for

								fprintf(fp,"%i\n",m_map->m_fg_objects.Length());
								{
									int i;
									ObjectPlaceHolder *o;
									m_map->m_fg_objects.Rewind();
									while(m_map->m_fg_objects.Iterate(o)) {
										fprintf(fp,"%s %i %i",o->m_name->get(),o->m_x,o->m_y);
										for(i=0;i<o->m_nparameters;i++) fprintf(fp," %i",o->m_parameters[i]);
										fprintf(fp,"\n");
									} // while 					
								}
								fclose(fp);	
								sprintf(m_message,"Success");
							} else {
								sprintf(m_message,"Error");
							} // if
						} // if 

						if (m_state==3) {
							// import:
							int i,j,no;
							int tmpdx,tmpdy;
							Symbol *symtmp;
							char tmp[256];
							FILE *fp;
							ObjectPlaceHolder *o;

							fp=fopen(m_editing_text,"r+");
							if (fp!=0) {
								import_ST2_map(fp);
								fclose(fp);
								sprintf(m_message,"Success");
								m_pointer_x=m_pointer_y=0;
							} else {
								sprintf(m_message,"Error");
							} // if
						} // if 
						
						m_state=0;
					} /* if */ 

					if (ke->sym==SDLK_ESCAPE) m_state=0;

					delete ke;

				} /* while */ 
			}
			break;	
	} // switch

	return true;
} /* TGLmapeditor::cycle */ 


void TGLmapeditor::draw(int SCREEN_X,int SCREEN_Y)
{
	float lightpos[4]={0,0,-1000,0};
	float tmpls[4]={1.0F,1.0F,1.0F,1.0};
	float tmpld[4]={1.0F,1.0F,1.0F,1.0};
	float tmpla[4]={1.0F,1.0F,1.0F,1.0};
    float ratio;

	m_screen_dx=SCREEN_X;
	m_screen_dy=SCREEN_Y;

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

	// draw start:
	glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

	// Draw palete:
	switch(m_mode) {
	case 0:	// TILE LIST
			{
				List<GLTile> l;
				GLTile *t;
				int y=0;
				float f=float(0.5f+(sin(m_cycle*0.5f)*0.5f));

				// determine start position:
				{
					int pos=m_tiles.PositionRef(m_tiles.GetObj());
					y=224-pos*32;

					if (y<-(m_tiles.Length()-13)*32) y=-(m_tiles.Length()-13)*32;
					if (y>0) y=0;
				}

				y+=64;

				l.Instance(m_tiles);
				l.Rewind();
				while(l.Iterate(t) && y<480) {
					if (y>=64) {
						if (t==m_tiles.GetObj()) {
							t->draw(1,1,1,f,0,y,0,0,32.0f/t->get_dy());
						} else {
							t->draw(0,y,0,0,32.0f/t->get_dy());
						} // if 
					} // if
					y+=32;
				} // while 
			}
			break;
	case 1: // OBJECT LIST:
			{
				List<GLTile> l;
				GLTile *t;
				int y=0;
				float f=float(0.5f+(sin(m_cycle*0.5f)*0.5f));

				// determine start position:
				{
					int pos=m_objects_tile.PositionRef(m_objects_tile.GetObj());
					y=224-pos*32;

					if (y<-(m_objects_tile.Length()-13)*32) y=-(m_objects_tile.Length()-13)*32;
					if (y>0) y=0;
				}

				y+=64;

				l.Instance(m_objects_tile);
				l.Rewind();
				while(l.Iterate(t) && y<480) {
					if (y>=64) {
						if (t==m_objects_tile.GetObj()) {
							t->draw(1,1,1,f,0,y,0,0,32.0f/t->get_dy());
						} else {
							t->draw(0,y,0,0,32.0f/t->get_dy());
						} // if 
					} // if 
					y+=32;
				} // while 
			}
			
			break;
	} // switch 

	// Draw map:
	{
		int i;
		int startx=0;
		int starty=0;
		int width=int(18/zoom);
		int height=int(15/zoom);

		startx=m_pointer_x-width/2;
		starty=m_pointer_y-height/2;

		if (startx>m_map->get_dx()-(width-1)) startx=m_map->get_dx()-(width-1);
		if (starty>m_map->get_dy()-(height-1)) starty=m_map->get_dy()-(height-1);
		if (startx<-1) startx=-1;
		if (starty<-1) starty=-1;

		glPushMatrix();
		glTranslatef(64,0,0);
		glScalef(zoom,zoom,zoom);

		m_map->draw(startx,starty,width+1,height+1,m_GLTM);

		// Draw map borders:
		for(i=startx+1;i<m_map->get_dx()+2;i++) {
			glColor4f(1,0,0,1);
			glPushMatrix();
			glTranslatef((i-startx-1)*32,(-starty-1)*32,0);
			glBegin(GL_QUADS);
			glVertex3f(0,0,0);
			glVertex3f(0,32,0);
			glVertex3f(32,32,0);
			glVertex3f(32,0,0);
			glEnd();
			glPopMatrix();

			glPushMatrix();
			glTranslatef((i-startx-1)*32,(-starty+m_map->get_dy())*32,0);
			glBegin(GL_QUADS);
			glVertex3f(0,0,0);
			glVertex3f(0,32,0);
			glVertex3f(32,32,0);
			glVertex3f(32,0,0);
			glEnd();
			glPopMatrix();
		} // for 
		for(i=0;i<m_map->get_dy()+2;i++) {
			if ((-startx-1)>=0) {
				glColor4f(1,0,0,1);
				glPushMatrix();
				glTranslatef((-startx-1)*32,(i-starty-1)*32,0);
				glBegin(GL_QUADS);
				glVertex3f(0,0,0);
				glVertex3f(0,32,0);
				glVertex3f(32,32,0);
				glVertex3f(32,0,0);
				glEnd();
				glPopMatrix();
			} // if

			glPushMatrix();
			glTranslatef((-startx+m_map->get_dx())*32,(i-starty-1)*32,0);
			glBegin(GL_QUADS);
			glVertex3f(0,0,0);
			glVertex3f(0,32,0);
			glVertex3f(32,32,0);
			glVertex3f(32,0,0);
			glEnd();
			glPopMatrix();
		} // for 

		glTranslatef((m_pointer_x-startx)*32,(m_pointer_y-starty)*32,0);
		{
			int sizex=32,sizey=32;
			float f=float(0.5f+(sin(m_cycle*0.5f)*0.5f));
			glColor4f(1,1,1,f);

			if (m_mode==0 && m_tiles.GetObj()!=0) {
				sizex=m_tiles.GetObj()->get_dx();
				sizey=m_tiles.GetObj()->get_dy();
			} // if 
			if (m_mode==1 && m_objects_tile.GetObj()!=0) {
				sizex=m_objects_tile.GetObj()->get_dx();
				sizey=m_objects_tile.GetObj()->get_dy();
			} // if 

			glBegin(GL_QUADS);
			glVertex3f(0,0,0);
			glVertex3f(0,sizey,0);
			glVertex3f(sizex,sizey,0);
			glVertex3f(sizex,0,0);
			glEnd();
		}
		glPopMatrix();
	}

	// Show map size:
	{
		char tmp[80];
		sprintf(tmp,"X %i",m_map->get_dx());
		print(32,64,tmp);
		sprintf(tmp,"Y %i",m_map->get_dy());
		print(32,80,tmp);
	}

	// status:
	switch(m_state) {
	case 0: print(0,0,m_message);
			break;
	case 1: print(0,0,"Load:");	
			print(0,16,m_editing_text);
			break;
	case 2: print(0,0,"Save:");					
			print(0,16,m_editing_text);
			break;
	case 3:	print(0,0,"Import:");				
			print(0,16,m_editing_text);
			break;
	} // switch

	// draw end

	glDisable(GL_BLEND);
	SDL_GL_SwapBuffers();
} /* TGLmapeditor::draw */ 


void TGLmapeditor::save_configuration(void)
{
} /* TGLmapeditor::save_configuration */ 


void TGLmapeditor::load_configuration(void)
{
} /* TGLmapeditor::load_configuration */ 



void TGLmapeditor::print(int x,int y,char *text)
{
	char tmp[80];
	GLTile *tile;
	SDL_Surface *sfc;
	SDL_Color c;
	c.r=255;
	c.g=255;
	c.b=255;

	sfc=TTF_RenderText_Solid(m_font,text,c);
	tile=new GLTile(sfc);
	glNormal3f(0,0,1);
	tile->draw(x,y,0,0,1);
	delete tile;
} /* TGLmapeditor::print */ 


GLTile *TGLmapeditor::get_tile_by_name(char *name)
{
	Symbol *s=new Symbol(name);
	int pos=m_tiles_name.Position(s);

	if (pos!=-1) return m_tiles[pos];

	return 0;
} /* TGLmapeditor::get_tile_by_name */ 


GLTile *TGLmapeditor::get_object_by_name(char *name)
{
	Symbol *s=new Symbol(name);
	int pos=m_objects_name.Position(s);

	if (pos!=-1) return m_objects_tile[pos];

	return 0;
} /* TGLmapeditor::get_object_by_name */ 


Symbol *TGLmapeditor::get_object_symbol_by_name(char *name)
{
	Symbol *s=new Symbol(name);
	int pos=m_objects_name.Position(s);

	if (pos!=-1) return m_objects_name[pos];

	return 0;
} /* TGLmapeditor::get_object_symbol_by_name */ 

