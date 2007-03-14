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


void TGLmapeditor::import_ST2_map(FILE *fp)
{
	int i,tile;
	int dx,dy;
	int x,y;
	int n_tanks=0;
	int n_buttons=0;
	List<ObjectPlaceHolder> m_doors;

	delete m_map;

	// map info:
	fscanf(fp,"%i %i",&dx,&dy);

	m_map=new TGLmap(dx,dy);

	for(i=0;i<dx*dy;i++) {
		fscanf(fp,"%i",&tile);
		x=i%dx;
		y=i/dx;

		switch(tile) {
		case 0:	// empty
				break;

		// LINE 1:
		case 1:	if ((i%2)==0) m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-grass1");
						 else m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-grass2");
				break;
		case 2: m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-rock");
				break;
		case 3: m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-left");
				break;
		case 4: m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-right");
				break;
		case 5: m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-leftcorner");
				break;
		case 6: m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-leftcorner-grass1");
				break;
		case 7: m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-rightcorner-grass1");
				break;
		case 8: m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-rightcorner");
				break;
		case 9: m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-wall-leftcorner-grass1");
				break;
		case 10: m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-wall-leftcorner");
				break;
		case 11: m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-wall-rightcorner");
				break;
		case 12: m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-wall-rightcorner-grass1");
				break;
		case 13: m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ramp-left1-grass");
				break;
		case 14: m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ramp-left2-grass");
				break;
		case 15: m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ramp-right2-grass");
				break;
		case 16: m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ramp-right1-grass");
				break;
		case 17:
		case 18:
		case 20:
				{
					int j;
					j=x+y;
					
					if ((j%2)==0) {
						j=rand()%20;
						if (j==0) m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall5");
						if (j>0) m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall1");
					} else {
						j=rand()%20;
						if (j==0) m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall4");
						if (j>0 && j<=4) m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall3");
						if (j>4 && j<=8) m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall1");
						if (j>8) m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall2");
					} // if 
				}
				break;
		case 19: m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-grass-bush");
				break;

		// LINE 2:
		case 21:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-wall-leftcorner-grass2");
				break;
		case 22:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-wall-rightcorner-grass2");
				break;
		case 23:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-leftcorner-grass2");
				break;
		case 24:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-rightcorner-grass2");
				break;
		case 25:
		case 26:
		case 27:	
				m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-grass1");
				m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("radar"),
															  get_object_symbol_by_name("radar"),x,y));
				break;
		case 28:
		case 29:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-grass1");
				m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("red-light"),
															  get_object_symbol_by_name("red-light"),x,y));
				break;
		
		case 30:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-grass-top");
				break;

		case 31:
		case 33:
				m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-left");
				m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("laser-left"),
															  get_object_symbol_by_name("laser-left"),x,y));
				break;
		case 32:
		case 34:
				m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-right");
				m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("laser-right"),
															  get_object_symbol_by_name("laser-right"),x,y));
				break;
		case 35:
		case 37:
				m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-grass1");
				m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("laser-down"),
															  get_object_symbol_by_name("laser-down"),x,y));
				break;
		case 36:
		case 38:
				m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-rock");
				m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("laser-up"),
															  get_object_symbol_by_name("laser-up"),x,y));
				break;

		case 39:
				m_map->m_fg_tiles[i]=get_tile_by_name("foreground/spike-wall-left");
				m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("spike-left"),
															  get_object_symbol_by_name("spike-left"),x,y));
				break;
		case 40:
				m_map->m_fg_tiles[i]=get_tile_by_name("foreground/spike-wall-right");
				m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("spike-right"),
															  get_object_symbol_by_name("spike-right"),x-1,y));
				break;

		// LINE 3:
		// (metalic tiles) ...
		case 41:if ((i%3)==0) m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-techno1");
				if ((i%3)==1) m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-techno2");
				if ((i%3)==2) m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-techno3");
				break;
		case 42:if ((i%2)==0) m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-techno1");
				if ((i%2)==1) m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-techno2");
				break;
		case 43:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-left-techno");
				break;
		case 44:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-right-techno");
				break;
		case 45:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-leftcorner-techno");
				break;
		case 46:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-leftcorner-techno");
				break;
		case 47:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-rightcorner-techno");
				break;
		case 48:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-rightcorner-techno");
				break;
		case 49:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-wall-leftcorner-techno1");
				break;
		case 50:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-wall-leftcorner-techno");
				break;
		case 51:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-wall-rightcorner-techno");
				break;
		case 52:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-wall-rightcorner-techno1");
				break;
		case 53:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ramp-left-techno2");
				break;
		case 54:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ramp-left-techno1");
				break;
		case 55:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ramp-right-techno1");
				break;
		case 56:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ramp-right-techno2");
				break;	
		case 57:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-techno1");
				break;
		case 58:m_map->m_fg_tiles[i]=0;
				break;
		case 59:m_map->m_fg_tiles[i]=0;
				break;
		case 60:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-horizontal-broken-right");
				break;

		// LINE 4:
		case 61:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-wall-leftcorner-techno2");
				break;
		case 62:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-wall-rightcorner-techno2");
				break;
		case 63:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ramp-left-techno3");
				break;
		case 64:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ramp-right-techno3");
				break;
		case 65:
		case 66:
		case 67:
				m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("wall-techno-computer"),
															  get_object_symbol_by_name("wall-techno-computer"),x,y));				
				break;
		case 68:
		case 69:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-techno-camera");
				break;
		case 70:
		case 71:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-techno-camera2");
				break;

		case 72:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-rock-techno1");
				break;
		case 73:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-rock-techno2");
				break;
		case 74:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-rock-techno3");
				break;
		case 75:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-rock-techno5");
				break;
		case 76:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-rock-techno4");
				break;
		case 77:
		case 78:
		case 79:
		case 80:// left spike
				break;


		// LINE 5:
		case 81:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-vertical1");
				break;
		case 82:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-horizontal1");
				break;
		case 83:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-node");
				break;
		case 84:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-left-up-corner1");
				break;
		case 85:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-left-down-corner1");
				break;
		case 86:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-right-down-corner1");
				break;
		case 87:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-right-up-corner1");
				break;
		case 88:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-t-left");
				break;
		case 89:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-t-down");
				break;
		case 90:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-t-right");
				break;
		case 91:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-t-up");
				break;
		case 92:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-vertical-grass");
				break;
		case 93:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-horizontal-grass");
				break;	
		case 94:m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("pipe-vertical-screen"),
															  get_object_symbol_by_name("pipe-vertical-screen"),x,y));				
				break;
		case 95:m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("pipe-horizontal-screen"),
															  get_object_symbol_by_name("pipe-horizontal-screen"),x,y));				
				break;
		case 96:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-horizontal-broken-left");
				break;
		case 97:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-left-up-corner2");
				break;
		case 98:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-left-down-corner2");
				break;
		case 99:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-right-down-corner2");
				break;
		case 100:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-right-up-corner2");
				break;

		// LINE 6:
		case 101:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-snow1");
				break;
		case 102:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-snow");
				break;
		case 103:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-left-snow");
				break;
		case 104:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-right-snow");
				break;
		case 105:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-snow-leftcorner");
				break;
		case 106:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-leftcorner-snow1");
				break;
		case 107:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-rightcorner-snow1");
				break;
		case 108:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-snow-rightcorner");
				break;
		case 109:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-wall-leftcorner-snow1");
				break;
		case 110:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-wall-leftcorner-snow");
				break;
		case 111:
		case 112:
		case 113:
				m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("ball-stand"),
															  get_object_symbol_by_name("ball-stand"),x,y));
				break;
		case 114:{
					ObjectPlaceHolder *o;
					o=new ObjectPlaceHolder(get_object_by_name("door-left"),
										    get_object_symbol_by_name("door-left"),x,y);
					o->m_nparameters=2;
					o->m_parameters[0]=0;
					o->m_parameters[1]=0;
					m_map->m_fg_objects.Add(o);
					m_doors.Add(o);
				 }
				 break;
		case 115:{
					ObjectPlaceHolder *o;
					o=new ObjectPlaceHolder(get_object_by_name("door-right"),
											get_object_symbol_by_name("door-right"),x,y);
					o->m_nparameters=2;
					o->m_parameters[0]=0;
					o->m_parameters[1]=0;
					m_map->m_fg_objects.Add(o);
					m_doors.Add(o);
				 }
				 break;
		case 116:// laser
				break;
		case 117:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-snow1");	
				 {
					ObjectPlaceHolder *o;
					o=new ObjectPlaceHolder(get_object_by_name("button-blue-down"),
										    get_object_symbol_by_name("button-blue-down"),x,y);
					o->m_nparameters=1;
					o->m_parameters[0]=n_buttons;
					n_buttons++;
					m_map->m_fg_objects.Add(o);			
				 }
				 break;
		case 118:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-snow");				 
				 {
					ObjectPlaceHolder *o;
					o=new ObjectPlaceHolder(get_object_by_name("button-blue-up"),
										    get_object_symbol_by_name("button-blue-up"),x,y);
					o->m_nparameters=1;
					o->m_parameters[0]=n_buttons;
					n_buttons++;
					m_map->m_fg_objects.Add(o);			
				 }
				 break;
		case 119:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-left-snow");				 
				 {
					ObjectPlaceHolder *o;
					o=new ObjectPlaceHolder(get_object_by_name("button-blue-left"),
										    get_object_symbol_by_name("button-blue-left"),x,y);
					o->m_nparameters=1;
					o->m_parameters[0]=n_buttons;
					n_buttons++;
					m_map->m_fg_objects.Add(o);			
				 }
				 break;
		case 120:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-right-snow");				 
				 {
					ObjectPlaceHolder *o;
					o=new ObjectPlaceHolder(get_object_by_name("button-blue-right"),
										    get_object_symbol_by_name("button-blue-right"),x,y);
					o->m_nparameters=1;
					o->m_parameters[0]=n_buttons;
					n_buttons++;
					m_map->m_fg_objects.Add(o);			
				 }
				 break;

		// LINE 7:
		// ...
		case 121:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-wall-rightcorner-snow");
				break;
		case 122:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-wall-rightcorner-snow1");
				break;
		case 123:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ramp-left1-snow");
				break;
		case 124:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ramp-left2-snow");
				break;
		case 125:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ramp-right2-snow");
				break;
		case 126:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ramp-right1-snow");
				break;

		case 127:
		case 128:
		case 130:
				{
					int j;
					j=x+y;
					
					if ((j%2)==0) {
						j=rand()%20;
						if (j==0) m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-snow5");
						if (j>0) m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-snow1");
					} else {
						j=rand()%20;
						if (j==0) m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-snow4");
						if (j>0 && j<=4) m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-snow3");
						if (j>4 && j<=8) m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-snow1");
						if (j>8) m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-snow2");
					} // if 
				}
				break;
		case 129:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-snow-bush");
				break;

		case 131:// vertical laser
				 break;
		case 132:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-horizontal-snow");
				 break;
		case 133:
				m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("fuel-recharge"),
															  get_object_symbol_by_name("fuel-recharge"),x,y));
				break;
		case 134:
		case 135:
		case 136:// fuel recharge 
				break;
		case 137:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-techno1");	
				 {
					ObjectPlaceHolder *o;
					o=new ObjectPlaceHolder(get_object_by_name("button-purple-down"),
										    get_object_symbol_by_name("button-purple-down"),x,y);
					o->m_nparameters=1;
					o->m_parameters[0]=n_buttons;
					n_buttons++;
					m_map->m_fg_objects.Add(o);			
				 }
				 break;
		case 138:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-techno");				 
				 {
					ObjectPlaceHolder *o;
					o=new ObjectPlaceHolder(get_object_by_name("button-purple-up"),
										    get_object_symbol_by_name("button-purple-up"),x,y);
					o->m_nparameters=1;
					o->m_parameters[0]=n_buttons;
					n_buttons++;
					m_map->m_fg_objects.Add(o);			
				 }
				 break;
		case 139:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-left-techno");				 
				 {
					ObjectPlaceHolder *o;
					o=new ObjectPlaceHolder(get_object_by_name("button-purple-left"),
										    get_object_symbol_by_name("button-purple-left"),x,y);
					o->m_nparameters=1;
					o->m_parameters[0]=n_buttons;
					n_buttons++;
					m_map->m_fg_objects.Add(o);			
				 }
				 break;
		case 140:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-right-techno");				 
				 {
					ObjectPlaceHolder *o;
					o=new ObjectPlaceHolder(get_object_by_name("button-purple-right"),
										    get_object_symbol_by_name("button-purple-right"),x,y);
					o->m_nparameters=1;
					o->m_parameters[0]=n_buttons;
					n_buttons++;
					m_map->m_fg_objects.Add(o);			
				 }
				 break;


		// LINE 8:
		case 141:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-wall-leftcorner-snow2");	
				 break;
		case 142:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-wall-rightcorner-snow2");	
				 break;
		case 143:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-leftcorner-snow2");	
				 break;
		case 144:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-rightcorner-snow2");	
				 break;

		case 145:
		case 146:
		case 147:
				m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-snow1");
				m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("radar"),
															  get_object_symbol_by_name("radar"),x,y));
				break;
		case 148:
		case 149:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-snow1");
				m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("red-light-snow"),
															  get_object_symbol_by_name("red-light-snow"),x,y));
				break;
				
		case 151:
		case 152:
		case 153:
		case 154:// right spike 
				break;
		case 155:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-directionalcannon-techno");				 
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("directional-cannon-down"),
															   get_object_symbol_by_name("directional-cannon-down"),x,y));
				 break;
		case 156:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-directionalcannon-techno");				 
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("directional-cannon-up"),
															   get_object_symbol_by_name("directional-cannon-up"),x,y));
				 break;
		case 157:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-grass1");	
				 {
					ObjectPlaceHolder *o;
					o=new ObjectPlaceHolder(get_object_by_name("button-red-down"),
										    get_object_symbol_by_name("button-red-down"),x,y);
					o->m_nparameters=1;
					o->m_parameters[0]=n_buttons;
					n_buttons++;
					m_map->m_fg_objects.Add(o);			
				 }
				 break;
		case 158:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-rock");				 
				 {
					ObjectPlaceHolder *o;
					o=new ObjectPlaceHolder(get_object_by_name("button-red-up"),
										    get_object_symbol_by_name("button-red-up"),x,y);
					o->m_nparameters=1;
					o->m_parameters[0]=n_buttons;
					n_buttons++;
					m_map->m_fg_objects.Add(o);			
				 }
				 break;
		case 159:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-left");				 
				 {
					ObjectPlaceHolder *o;
					o=new ObjectPlaceHolder(get_object_by_name("button-red-left"),
										    get_object_symbol_by_name("button-red-left"),x,y);
					o->m_nparameters=1;
					o->m_parameters[0]=n_buttons;
					n_buttons++;
					m_map->m_fg_objects.Add(o);			
				 }
				 break;
		case 160:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-right");				 
				 {
					ObjectPlaceHolder *o;
					o=new ObjectPlaceHolder(get_object_by_name("button-red-right"),
										    get_object_symbol_by_name("button-red-right"),x,y);
					o->m_nparameters=1;
					o->m_parameters[0]=n_buttons;
					n_buttons++;
					m_map->m_fg_objects.Add(o);			
				 }
				 break;

		// LINE 9:

		case 161:
		case 163:
				m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-left-snow");
				m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("laser-left"),
															  get_object_symbol_by_name("laser-left"),x,y));
				break;
		case 162:
		case 164:
				m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-right-snow");
				m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("laser-right"),
															  get_object_symbol_by_name("laser-right"),x,y));
				break;
		case 165:
		case 167:
				m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-snow1");
				m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("laser-down"),
															  get_object_symbol_by_name("laser-down"),x,y));
				break;
		case 166:
		case 168:
				m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-snow");
				m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("laser-up"),
															  get_object_symbol_by_name("laser-up"),x,y));
				break;


		case 171:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-cannon-snow1");
				 break;
		case 172:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-cannon-snow");
				 break;
		case 173:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-left-cannon-snow");
				 break;
		case 174:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-right-cannon-snow");
				 break;
		case 175:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-left-directionalcannon-techno");				 
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("directional-cannon-left"),
															   get_object_symbol_by_name("directional-cannon-left"),x,y));
				 break;
		case 176:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-right-directionalcannon-techno");				 
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("directional-cannon-right"),
															   get_object_symbol_by_name("directional-cannon-right"),x,y));
				 break;
		case 177:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-cannon-snow1");
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("cannon-rock-down"),
															   get_object_symbol_by_name("cannon-rock-down"),x,y));
				 break;
		case 178:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-cannon-snow");
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("cannon-rock-up"),
															   get_object_symbol_by_name("cannon-rock-up"),x,y));
				 break;
		case 179:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-left-cannon-snow");
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("cannon-rock-left"),
															   get_object_symbol_by_name("cannon-rock-left"),x,y));
				 break;
		case 180:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-right-cannon-snow");
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("cannon-rock-right"),
															   get_object_symbol_by_name("cannon-rock-right"),x,y));
				 break;
		// LINE 10:
		// ...
		case 185:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/big-rock-grass1");
				break;
		case 186:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/big-rock-grass2");
				break;
		case 187:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/big-rock-grass3");
				break;
		case 188:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/big-rock-grass4");
				break;
		
		case 193:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-ground-snow");
				break;
		case 194:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-ceiling-snow");
				break;
		case 195:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-wall-left-snow");
				break;
		case 196:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-wall-right-snow");
				break;
		case 197:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-cannon-techno");				 
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("fastcannon-techno-down"),
															   get_object_symbol_by_name("fastcannon-techno-down"),x,y));
				 break;
		case 198:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-cannon-techno");				 
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("fastcannon-techno-up"),
															   get_object_symbol_by_name("fastcannon-techno-up"),x,y));
				 break;
		case 199:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-left-cannon-techno");				 
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("fastcannon-techno-left"),
															   get_object_symbol_by_name("fastcannon-techno-left"),x,y));
				 break;
		case 200:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-right-cannon-techno");
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("fastcannon-techno-right"),
															   get_object_symbol_by_name("fastcannon-techno-right"),x,y));
				 break;

		// LINE 11:
		// ...
		case 211:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/rock-heads1");
				 break;			
		case 212:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/rock-heads2");
				 break;			
		case 213:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-ground-techno");
				 break;			
		case 214:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-ceiling-techno");
				 break;			
		case 215:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-wall-left-techno");
				 break;			
		case 216:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-wall-right-techno");
				 break;			
		case 217:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-cannon-techno");				 
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("cannon-techno-down"),
															   get_object_symbol_by_name("cannon-techno-down"),x,y));
				 break;
		case 218:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-cannon-techno");				 
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("cannon-techno-up"),
															   get_object_symbol_by_name("cannon-techno-up"),x,y));
				 break;
		case 219:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-left-cannon-techno");				 
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("cannon-techno-left"),
															   get_object_symbol_by_name("cannon-techno-left"),x,y));
				 break;
		case 220:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-right-cannon-techno");
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("cannon-techno-right"),
															   get_object_symbol_by_name("cannon-techno-right"),x,y));
				 break;
		case 221:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/crashed-tank-1");
				 break;			
		case 222:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/crashed-tank-2");
				 break;			

		// LINE 12:
		case 223:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/tower-techno1");
				 break;			
		case 224:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/tower-techno2");
				 break;			
		case 231:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/rock-heads3");
				 break;			
		case 232:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/rock-heads4");
				 break;			
		case 233:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-ground-grass");
				 break;	
		case 234:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-ceiling-rock");
				 break;	
		case 235:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-wall-left");
				 break;	
		case 236:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-wall-right");
				 break;	
		case 237:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-cannon");				 
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("cannon-rock-down"),
															   get_object_symbol_by_name("cannon-rock-down"),x,y));
				 break;
		case 238:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-cannon");				 
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("cannon-rock-up"),
															   get_object_symbol_by_name("cannon-rock-up"),x,y));
				 break;
		case 239:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-left-cannon");				 
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("cannon-rock-left"),
															   get_object_symbol_by_name("cannon-rock-left"),x,y));
				 break;
		case 240:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-right-cannon");
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("cannon-rock-right"),
															   get_object_symbol_by_name("cannon-rock-right"),x,y));
				 break;

		case 387:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/floor-directionalcannon-techno");				 
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("ha-directional-cannon-down"),
															   get_object_symbol_by_name("ha-directional-cannon-down"),x,y));
				 break;
		case 388:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/ceiling-directionalcannon-techno");				 
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("ha-directional-cannon-up"),
															   get_object_symbol_by_name("ha-directional-cannon-up"),x,y));
				 break;
		case 407:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-left-directionalcannon-techno");				 
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("ha-directional-cannon-left"),
															   get_object_symbol_by_name("ha-directional-cannon-left"),x,y));
				 break;
		case 408:m_map->m_fg_tiles[i]=get_tile_by_name("foreground/wall-right-directionalcannon-techno");				 
				 m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("ha-directional-cannon-right"),
															   get_object_symbol_by_name("ha-directional-cannon-right"),x,y));
				 break;

		case 423:
				m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-vertical1");
				m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("laser-left"),
															  get_object_symbol_by_name("laser-left"),x,y));
				break;
		case 424:
				m_map->m_fg_tiles[i]=get_tile_by_name("foreground/pipe-vertical1");
				m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("laser-right"),
															  get_object_symbol_by_name("laser-right"),x,y));
				break;


		default:output_debug_message("Missing Tile: %i\n",tile);
				break;
		} // switch
	} // for

	// doors:
	for(i=0;i<m_doors.Length()/2;i++) {
		int initial_state,action;
		fscanf(fp,"%i %i",&initial_state,&action);
		if (initial_state!=0) initial_state=1;
		m_doors[i*2]->m_parameters[0]=initial_state;
		m_doors[i*2]->m_parameters[1]=action;
		m_doors[i*2+1]->m_parameters[0]=initial_state;
		m_doors[i*2+1]->m_parameters[1]=action;
		// ...
	} // for 

	// tanks:
	fscanf(fp,"%i",&n_tanks);
	for(i=0;i<n_tanks;i++) {
		int x,y,type;
		fscanf(fp,"%i %i %i",&x,&y,&type);

		if (type==0) {
			m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("grey-tank"),
															   get_object_symbol_by_name("grey-tank"),x,y-8));
		} // if 
		if (type==1) {
			m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("green-tank"),
															   get_object_symbol_by_name("green-tank"),x,y-8));
		} // if 
		if (type==2) {
			m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("red-tank"),
															   get_object_symbol_by_name("red-tank"),x,y-8));
		} // if 
		if (type==3) {
			m_map->m_fg_objects.Add(new ObjectPlaceHolder(get_object_by_name("big-tank"),
															   get_object_symbol_by_name("big-tank"),x,y-8));
		} // if 
	} // for 

	fscanf(fp,"%i",&i);
	if (i==0) m_map->m_background=0;
	if (i==1) m_map->m_background=4;
	if (i==2) m_map->m_background=1;

	m_doors.ExtractAll();

} /* TGLmapeditor::import_ST2_map */  


