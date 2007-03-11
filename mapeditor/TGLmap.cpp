#ifdef KITSCHY_DEBUG_MEMORY 
#include "debug_memorymanager.h"
#endif


#ifdef _WIN32
#include "windows.h"
#endif

#include "assert.h"
#include "math.h"

#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "List.h"
#include "Symbol.h"
#include "auxiliar.h"
#include "2DCMC.h"
#include "GLTile.h"

#include "GLTManager.h"

#include "TGLmap.h"


TGLmap::TGLmap(int dx,int dy)
{
	int i;

	m_dx=dx;
	m_dy=dy;
	m_fg_tiles=new GLTile *[dx*dy];

	for(i=0;i<dx*dy;i++) m_fg_tiles[i]=0;

	m_cycle=0;

	m_background=0;

	m_font=TTF_OpenFont("lucon.ttf",12);
} /* TGLmap::TGLmap */ 


TGLmap::~TGLmap()
{
	int i;
	for(i=0;i<m_dx*m_dy;i++) m_fg_tiles[i]=0;

	delete []m_fg_tiles;

	TTF_CloseFont(m_font);
} /* TGLmap::~TGLmap */ 


void TGLmap::cycle(void) 
{
	m_cycle++;
} /* TGLmap::cycle */ 


void TGLmap::draw(int startx,int starty,int dx,int dy,GLTManager *GLTM)
{
	int x,y;
	int offs;
	ObjectPlaceHolder *o;
	int local_cycle;
	List<ObjectPlaceHolder> erroneous;


	// Draw background:
	switch(m_background) {
	case 0:
			{
				for(y=0;y<dy && starty+y<m_dy;y++) {
					for(x=0;x<dx && startx+x<m_dx;x++) {
						if (startx+x>=0 && starty+y>=0 && (startx+x)%2==0) {
							if (starty+y==0) GLTM->get("background/top-rock")->draw(x*32,y*32,0,0,1);
							if (starty+y==2) GLTM->get("background/middle-rock")->draw(x*32,y*32,0,0,1);
							if (starty+y>2 && ((starty+y)%2)==0) GLTM->get("background/bottom-rock")->draw(x*32,y*32,0,0,1);
						} // if
					} // for
				} // for					
			}
			break;
	case 1:
			{
				for(y=0;y<dy && starty+y<m_dy;y++) {
					for(x=0;x<dx && startx+x<m_dx;x++) {
						if (startx+x>=0 && starty+y>=0 && (startx+x)%2==0) {
							if (starty+y==0) GLTM->get("background/top-techno-red")->draw(x*32,y*32,0,0,1);
							if (starty+y>0 && ((starty+y)%2)==0) GLTM->get("background/bottom-techno-red")->draw(x*32,y*32,0,0,1);
						} // if
					} // for
				} // for					
			}
			break;
	case 2:
			{
				for(y=0;y<dy && starty+y<m_dy;y++) {
					for(x=0;x<dx && startx+x<m_dx;x++) {
						if (startx+x>=0 && starty+y>=0 && (startx+x)%2==0) {
							if (starty+y==0) GLTM->get("background/top-techno-blue")->draw(x*32,y*32,0,0,1);
							if (starty+y>0 && ((starty+y)%2)==0) GLTM->get("background/bottom-techno-blue")->draw(x*32,y*32,0,0,1);
						} // if
					} // for
				} // for					
			}
			break;
	case 3:
			{
				for(y=0;y<dy && starty+y<m_dy;y++) {
					for(x=0;x<dx && startx+x<m_dx;x++) {
						if (startx+x>=0 && starty+y>=0 && (startx+x)%2==0) {
							if (starty+y==0) GLTM->get("background/top-techno-green")->draw(x*32,y*32,0,0,1);
							if (starty+y>0 && ((starty+y)%2)==0) GLTM->get("background/bottom-techno-green")->draw(x*32,y*32,0,0,1);
						} // if
					} // for
				} // for					
			}
			break;
	} // switch

	m_fg_objects.Rewind();
	while(m_fg_objects.Iterate(o)) {
//		if (o->m_x>=startx && o->m_x<(startx+dx) &&
//			o->m_y>=starty && o->m_y<(starty+dy)) {
			// Draw objects:
			if (o->m_name->cmp("ball-stand")) {
				GLTM->get("objects/pipe-ball-stand1")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
			} else if (o->m_name->cmp("red-light")) {
				local_cycle=m_cycle%48;
				if (local_cycle<24) GLTM->get("objects/red-light1")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
							   else GLTM->get("objects/red-light2")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
			} else if (o->m_name->cmp("pipe-vertical-screen")) {
				local_cycle=(m_cycle%120)/8;
				if (local_cycle==0) GLTM->get("objects/pipe-vertical-screen1")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==1) GLTM->get("objects/pipe-vertical-screen2")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==2) GLTM->get("objects/pipe-vertical-screen1")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==3) GLTM->get("objects/pipe-vertical-screen2")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==4) GLTM->get("objects/pipe-vertical-screen1")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==5) GLTM->get("objects/pipe-vertical-screen2")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==6) GLTM->get("objects/pipe-vertical-screen1")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==7) GLTM->get("objects/pipe-vertical-screen2")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==8) GLTM->get("objects/pipe-vertical-screen3")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==9) GLTM->get("objects/pipe-vertical-screen4")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==10) GLTM->get("objects/pipe-vertical-screen5")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==11) GLTM->get("objects/pipe-vertical-screen6")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==12) GLTM->get("objects/pipe-vertical-screen7")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==13) GLTM->get("objects/pipe-vertical-screen8")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==14) GLTM->get("objects/pipe-vertical-screen9")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
			} else if (o->m_name->cmp("pipe-horizontal-screen")) {
				local_cycle=(m_cycle%96)/8;
				if (local_cycle==0) GLTM->get("objects/pipe-horizontal-screen1")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==1) GLTM->get("objects/pipe-horizontal-screen2")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==2) GLTM->get("objects/pipe-horizontal-screen1")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==3) GLTM->get("objects/pipe-horizontal-screen2")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==4) GLTM->get("objects/pipe-horizontal-screen3")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==5) GLTM->get("objects/pipe-horizontal-screen2")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==6) GLTM->get("objects/pipe-horizontal-screen1")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==7) GLTM->get("objects/pipe-horizontal-screen3")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==8) GLTM->get("objects/pipe-horizontal-screen4")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==9) GLTM->get("objects/pipe-horizontal-screen2")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==10) GLTM->get("objects/pipe-horizontal-screen3")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==11) GLTM->get("objects/pipe-horizontal-screen2")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
			} else if (o->m_name->cmp("radar")) {
				local_cycle=(m_cycle%64)/8;
				if (local_cycle==0) GLTM->get("objects/radar1")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==1) GLTM->get("objects/radar2")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==2) GLTM->get("objects/radar3")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==3) GLTM->get("objects/radar4")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==4) GLTM->get("objects/radar5")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==5) GLTM->get("objects/radar4")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==6) GLTM->get("objects/radar3")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==7) GLTM->get("objects/radar2")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
			} else if (o->m_name->cmp("laser-left")) {
				local_cycle=(m_cycle/96)%2;

				// Find the pair:
				{
					ObjectPlaceHolder *o2,*pair=0;
					List<ObjectPlaceHolder> l;
					l.Instance(m_fg_objects);
					l.Rewind();
					while(l.Iterate(o2)) {
						if (o2->m_name->cmp("laser-right") && o2->m_y==o->m_y &&
							o2->m_x>o->m_x) {
							if (pair==0) pair=o2;
							if (pair->m_x>o2->m_x) pair=o2;
						} // if 
					} // while 
					if (pair!=0) {
						if (local_cycle==1) {
							// Draw the laser:
							int i;
							bool flicker=((rand()%10)==0 ? true:false);
							for(i=o->m_x;i<=pair->m_x;i++) {
								if (flicker) GLTM->get("objects/laser-horizontal2")->draw((i-startx)*32,(o->m_y-starty)*32,0,0,1);
										else GLTM->get("objects/laser-horizontal1")->draw((i-startx)*32,(o->m_y-starty)*32,0,0,1);
							} // for
						} // if 
					} else {
						erroneous.Add(o);
					} // if 

					if (local_cycle==0) GLTM->get("objects/laser-left1")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
					if (local_cycle==1) GLTM->get("objects/laser-left2")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				}
			} else if (o->m_name->cmp("laser-right")) {
				local_cycle=(m_cycle/96)%2;
				if (local_cycle==0) GLTM->get("objects/laser-right1")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==1) GLTM->get("objects/laser-right2")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				// Find the pair:
				{
					ObjectPlaceHolder *o2,*pair=0;
					List<ObjectPlaceHolder> l;
					l.Instance(m_fg_objects);
					l.Rewind();
					while(l.Iterate(o2)) {
						if (o2->m_name->cmp("laser-left") && o2->m_y==o->m_y &&
							o2->m_x<o->m_x) {
							if (pair==0) pair=o2;
							if (pair->m_x<o2->m_x) pair=o2;
						} // if 
					} // while 
					if (pair==0) {
						erroneous.Add(o);
					} // if 
				}

			} else if (o->m_name->cmp("laser-up")) {
				local_cycle=(m_cycle/96)%2;

				// Find the pair:
				{
					ObjectPlaceHolder *o2,*pair=0;
					List<ObjectPlaceHolder> l;
					l.Instance(m_fg_objects);
					l.Rewind();
					while(l.Iterate(o2)) {
						if (o2->m_name->cmp("laser-down") && o2->m_x==o->m_x &&
							o2->m_y>o->m_y) {
							if (pair==0) pair=o2;
							if (pair->m_y>o2->m_y) pair=o2;
						} // if 
					} // while 
					if (pair!=0) {
						if (local_cycle==1) {
							// Draw the laser:
							int i;
							bool flicker=((rand()%10)==0 ? true:false);
							for(i=o->m_y;i<=pair->m_y;i++) {
								if (flicker) GLTM->get("objects/laser-vertical2")->draw((o->m_x-startx)*32,(i-starty)*32,0,0,1);
										else GLTM->get("objects/laser-vertical1")->draw((o->m_x-startx)*32,(i-starty)*32,0,0,1);
							} // for
						} // if 
					} else {
						erroneous.Add(o);
					} // if 

					if (local_cycle==0) GLTM->get("objects/laser-up1")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
					if (local_cycle==1) GLTM->get("objects/laser-up2")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				}
			
			} else if (o->m_name->cmp("laser-down")) {
				local_cycle=(m_cycle/96)%2;
				if (local_cycle==0) GLTM->get("objects/laser-down1")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==1) GLTM->get("objects/laser-down2")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				// Find the pair:
				{
					ObjectPlaceHolder *o2,*pair=0;
					List<ObjectPlaceHolder> l;
					l.Instance(m_fg_objects);
					l.Rewind();
					while(l.Iterate(o2)) {
						if (o2->m_name->cmp("laser-up") && o2->m_x==o->m_x &&
							o2->m_y<o->m_y) {
							if (pair==0) pair=o2;
							if (pair->m_y<o2->m_y) pair=o2;
						} // if 
					} // while 
					if (pair==0) {
						erroneous.Add(o);
					} // if 
				}			
			} else if (o->m_name->cmp("spike-left")) {
				local_cycle=(m_cycle/2)%64;
				if (local_cycle<32) GLTM->get("objects/spike-left")->draw((o->m_x-startx)*32-(32-local_cycle),(o->m_y-starty)*32,0,0,1);
				if (local_cycle>=32) GLTM->get("objects/spike-left")->draw((o->m_x-startx)*32-(local_cycle-32),(o->m_y-starty)*32,0,0,1);
			} else if (o->m_name->cmp("spike-right")) {
				local_cycle=(m_cycle/2)%64;
				if (local_cycle<32) GLTM->get("objects/spike-right")->draw((o->m_x-startx)*32+(32-local_cycle),(o->m_y-starty)*32,0,0,1);
				if (local_cycle>=32) GLTM->get("objects/spike-right")->draw((o->m_x-startx)*32+(local_cycle-32),(o->m_y-starty)*32,0,0,1);
			} else if (o->m_name->cmp("wall-techno-computer")) {
				local_cycle=(m_cycle%56)/8;
				if (local_cycle==0) GLTM->get("objects/wall-techno-computer1")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==1) GLTM->get("objects/wall-techno-computer2")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==2) GLTM->get("objects/wall-techno-computer3")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==3) GLTM->get("objects/wall-techno-computer4")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==4) GLTM->get("objects/wall-techno-computer5")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==5) GLTM->get("objects/wall-techno-computer6")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				if (local_cycle==6) GLTM->get("objects/wall-techno-computer7")->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
			} else if (o->m_name->cmp("directional-cannon-left")) {
				local_cycle=m_cycle%160;
				if (local_cycle<80) {
					local_cycle=-80+local_cycle*2;
				} else {
					local_cycle=80-((local_cycle-80)*2);
				} // if 
				GLTM->get("objects/directionalcannon1")->draw((o->m_x-startx)*32+19,(o->m_y-starty)*32+15,0,local_cycle,1);
			} else if (o->m_name->cmp("directional-cannon-right")) {
				local_cycle=m_cycle%160;
				if (local_cycle<80) {
					local_cycle=180-80+local_cycle*2;
				} else {
					local_cycle=180+80-((local_cycle-80)*2);
				} // if 
				GLTM->get("objects/directionalcannon1")->draw((o->m_x-startx)*32+12,(o->m_y-starty)*32+15,0,local_cycle,1);
			} else if (o->m_name->cmp("directional-cannon-up")) {
				local_cycle=m_cycle%160;
				if (local_cycle<80) {
					local_cycle=90-80+local_cycle*2;
				} else {
					local_cycle=90+80-((local_cycle-80)*2);
				} // if 
				GLTM->get("objects/directionalcannon1")->draw((o->m_x-startx)*32+16,(o->m_y-starty)*32+19,0,local_cycle,1);
			} else if (o->m_name->cmp("directional-cannon-down")) {
				local_cycle=m_cycle%160;
				if (local_cycle<80) {
					local_cycle=-90-80+local_cycle*2;
				} else {
					local_cycle=-90+80-((local_cycle-80)*2);
				} // if 
				GLTM->get("objects/directionalcannon1")->draw((o->m_x-startx)*32+15,(o->m_y-starty)*32+12,0,local_cycle,1);
			} else if (o->m_name->cmp("ha-directional-cannon-left")) {
				local_cycle=m_cycle%160;
				if (local_cycle<80) {
					local_cycle=-80+local_cycle*2;
				} else {
					local_cycle=80-((local_cycle-80)*2);
				} // if 
				GLTM->get("objects/ha-directionalcannon1")->draw((o->m_x-startx)*32+19,(o->m_y-starty)*32+15,0,local_cycle,1);
			} else if (o->m_name->cmp("ha-directional-cannon-right")) {
				local_cycle=m_cycle%160;
				if (local_cycle<80) {
					local_cycle=180-80+local_cycle*2;
				} else {
					local_cycle=180+80-((local_cycle-80)*2);
				} // if 
				GLTM->get("objects/ha-directionalcannon1")->draw((o->m_x-startx)*32+12,(o->m_y-starty)*32+15,0,local_cycle,1);
			} else if (o->m_name->cmp("ha-directional-cannon-up")) {
				local_cycle=m_cycle%160;
				if (local_cycle<80) {
					local_cycle=90-80+local_cycle*2;
				} else {
					local_cycle=90+80-((local_cycle-80)*2);
				} // if 
				GLTM->get("objects/ha-directionalcannon1")->draw((o->m_x-startx)*32+16,(o->m_y-starty)*32+19,0,local_cycle,1);
			} else if (o->m_name->cmp("ha-directional-cannon-down")) {
				local_cycle=m_cycle%160;
				if (local_cycle<80) {
					local_cycle=-90-80+local_cycle*2;
				} else {
					local_cycle=-90+80-((local_cycle-80)*2);
				} // if 
				GLTM->get("objects/ha-directionalcannon1")->draw((o->m_x-startx)*32+15,(o->m_y-starty)*32+12,0,local_cycle,1);
			} else if (o->m_name->cmp("door-left")) {
				char tmp[80];
				if (o->m_parameters[1]==0) {
					sprintf(tmp,"BALL");
				} else {
					sprintf(tmp,"B%i",o->m_parameters[1]);
				} // if 
				print((o->m_x-startx)*32,(o->m_y-starty)*32,tmp);
				if (o->m_parameters[0]==0) {
					o->m_tile->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				} else {
					o->m_tile->draw((o->m_x-startx)*32-22,(o->m_y-starty)*32,0,0,1);
				} // if 
			} else if (o->m_name->cmp("door-right")) {
				char tmp[80];
				if (o->m_parameters[1]==0) {
					sprintf(tmp,"BALL");
				} else {
					sprintf(tmp,"B%i",o->m_parameters[1]);
				} // if 
				print((o->m_x-startx)*32,(o->m_y-starty)*32,tmp);
				if (o->m_parameters[0]==0) {
					o->m_tile->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				} else {
					o->m_tile->draw((o->m_x-startx)*32+22,(o->m_y-starty)*32,0,0,1);
				} // if 
			} else if (o->m_name->cmp("button-red-left")) {
				char tmp[80];
				sprintf(tmp,"B%i",o->m_parameters[0]+1);
				o->m_tile->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				print((o->m_x-startx)*32,(o->m_y-starty)*32,tmp);
			} else if (o->m_name->cmp("button-red-right")) {
				char tmp[80];
				sprintf(tmp,"B%i",o->m_parameters[0]+1);
				o->m_tile->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				print((o->m_x-startx)*32,(o->m_y-starty)*32,tmp);
			} else if (o->m_name->cmp("button-red-up")) {
				char tmp[80];
				sprintf(tmp,"B%i",o->m_parameters[0]+1);
				o->m_tile->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				print((o->m_x-startx)*32,(o->m_y-starty)*32,tmp);
			} else if (o->m_name->cmp("button-red-down")) {
				char tmp[80];
				sprintf(tmp,"B%i",o->m_parameters[0]+1);
				o->m_tile->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				print((o->m_x-startx)*32,(o->m_y-starty)*32,tmp);

			} else if (o->m_name->cmp("button-purple-left")) {
				char tmp[80];
				sprintf(tmp,"B%i",o->m_parameters[0]+1);
				o->m_tile->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				print((o->m_x-startx)*32,(o->m_y-starty)*32,tmp);
			} else if (o->m_name->cmp("button-purple-right")) {
				char tmp[80];
				sprintf(tmp,"B%i",o->m_parameters[0]+1);
				o->m_tile->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				print((o->m_x-startx)*32,(o->m_y-starty)*32,tmp);
			} else if (o->m_name->cmp("button-purple-up")) {
				char tmp[80];
				sprintf(tmp,"B%i",o->m_parameters[0]+1);
				o->m_tile->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				print((o->m_x-startx)*32,(o->m_y-starty)*32,tmp);
			} else if (o->m_name->cmp("button-purple-down")) {
				char tmp[80];
				sprintf(tmp,"B%i",o->m_parameters[0]+1);
				o->m_tile->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
				print((o->m_x-startx)*32,(o->m_y-starty)*32,tmp);
			} else {
				o->m_tile->draw((o->m_x-startx)*32,(o->m_y-starty)*32,0,0,1);
			} // if 
//		} // if 
	} // while 


	for(y=0;y<dy && starty+y<m_dy;y++) {
		for(x=0;x<dx && startx+x<m_dx;x++) {
			if (startx+x>=0 && starty+y>=0) {
				offs=(startx+x)+(starty+y)*m_dx;
				if (m_fg_tiles[offs]!=0) {
					m_fg_tiles[offs]->draw(x*32,y*32,0,0,1);
				} // if 
			} // if
		} // for
	} // for


	// Show errors:
	while(!erroneous.EmptyP()) {
		o=erroneous.ExtractIni();
		glColor4f(1,0,0,0.5+0.5*sin(m_cycle*0.2f));
		glPushMatrix();
		glTranslatef((o->m_x-startx)*32,(o->m_y-starty)*32,0);
		glBegin(GL_QUADS);
		glVertex3f(0,0,0);
		glVertex3f(0,32,0);
		glVertex3f(32,32,0);
		glVertex3f(32,0,0);
		glEnd();
		glPopMatrix();
	} // while 

} /* TGLmap::draw */ 


void TGLmap::set_dx(int dx)
{
	GLTile **tmp;
	int i,j;

	tmp=new GLTile *[dx*m_dy];

	for(i=0;i<dx*m_dy;i++) tmp[i]=0;

	for(i=0;i<m_dy;i++) {
		for(j=0;j<m_dx && j<dx;j++) {
			tmp[j+i*dx]=m_fg_tiles[j+i*m_dx];
		} // for
	} // for
	for(i=0;i<m_dx*m_dy;i++) m_fg_tiles[i]=0;
	delete []m_fg_tiles;

	m_dx=dx;
	m_fg_tiles=tmp;

	{
		List<ObjectPlaceHolder> todelete;
		ObjectPlaceHolder *o;
		m_fg_objects.Rewind();
		while(m_fg_objects.Iterate(o)) {
			if (o->m_x>=dx) todelete.Add(o);
		} // while 
		while(!todelete.EmptyP()) {
			o=todelete.ExtractIni();
			m_fg_objects.DeleteElement(o);
			delete o;
		} // while 
	}
} /* TGLmap::set_dx */ 


void TGLmap::set_dy(int dy)
{
	GLTile **tmp;
	int i,j;

	tmp=new GLTile *[m_dx*dy];

	for(i=0;i<m_dx*dy;i++) tmp[i]=0;

	for(i=0;i<m_dy && i<dy;i++) {
		for(j=0;j<m_dx;j++) {
			tmp[j+i*m_dx]=m_fg_tiles[j+i*m_dx];
		} // for
	} // for
	for(i=0;i<m_dx*m_dy;i++) m_fg_tiles[i]=0;
	delete []m_fg_tiles;

	m_dy=dy;
	m_fg_tiles=tmp;

	{
		List<ObjectPlaceHolder> todelete;
		ObjectPlaceHolder *o;
		m_fg_objects.Rewind();
		while(m_fg_objects.Iterate(o)) {
			if (o->m_y>=dy) todelete.Add(o);
		} // while 
		while(!todelete.EmptyP()) {
			o=todelete.ExtractIni();
			m_fg_objects.DeleteElement(o);
			delete o;
		} // while 
	}
} /* TGLmap::set_dy */ 


void TGLmap::print(int x,int y,char *text)
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
} /* TGLmap::print */ 