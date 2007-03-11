#ifdef KITSCHY_DEBUG_MEMORY 
#include "debug_memorymanager.h"
#endif

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "string.h"
#include <time.h>

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
#include "TGLobject_ball.h"
#include "TGLmap.h"
#include "TGL.h"
#include "TGLapp.h"

#include "TGLreplay.h"

#include "debug.h"

extern char *application_version;


TGLreplay_object_position::~TGLreplay_object_position()
{
	if (m_name!=0) {
		delete []m_name;
		m_name=0;
	} // if 
} /* TGLreplay_object_position::~TGLreplay_object_position */ 



TGLreplay::TGLreplay(FILE *fp)
{
	// Load a replay:
	/* ... */ 
} /* TGLreplay::TGLreplay */ 


TGLreplay::TGLreplay(char *map)
{
	m_version=new char[strlen(application_version)+1];
	strcpy(m_version,application_version);

#ifdef _WIN32
    struct tm today;

    _tzset();
    _getsystime(&today);

	m_year=today.tm_year;
	m_month=today.tm_mon;
	m_day=today.tm_mday;
	m_hour=today.tm_hour;
	m_minute=today.tm_min;
	m_second=today.tm_sec;
#else
    struct timeval ttime;
    struct tm *today;

    gettimeofday(&ttime, NULL);
    today = localtime(&(ttime.tv_sec));

	m_year=today->tm_year;
	m_month=today->tm_mon;
	m_day=today->tm_mday;
	m_hour=today->tm_hour;
	m_minute=today->tm_min;
	m_second=today->tm_sec;
#endif

	m_map=new char[strlen(map)+1];
	strcpy(m_map,map);
} /* TGLreplay::TGLreplay */ 


TGLreplay::~TGLreplay()
{
	if (m_version!=0) delete []m_version;
	m_version=0;

	if (m_map!=0) delete []m_map;
	m_map=0;
} /* TGLreplay::~TGLreplay */ 


int TGLreplay::get_length(void)
{
	return m_replay.Length();
} /* TGLreplay::get_length */ 


void TGLreplay::add_player(char *player_name)
{
	char *tmp=new char[strlen(player_name)+1];
	strcpy(tmp,player_name);
	m_players.Add(tmp);
} /* TGLreplay::add_player */ 


bool TGLreplay::save(FILE *fp)
{
	/* ... */ 

	return true;
} /* TGLreplay::save */ 


void TGLreplay::store_cycle(List<VirtualController> *m_input,List<TGLobject> *m_objects)
{
	int cycle=m_replay.Length();
	TGLreplay_node *node=0;
	VirtualController *vc,*vc2;
	TGLobject *o;
	TGLreplay_object_position *ro;
	const char *name;

	if ((cycle%KEYFRAME_PERIOD)==0) {
		node = new TGLreplay_node();
		node->m_keyframe=true;
		
		m_input->Rewind();
		while(m_input->Iterate(vc)) {
			vc2=new VirtualController(vc);
			node->m_input.Add(vc2);
		} // while

		m_objects->Rewind();
		while(m_objects->Iterate(o)) {
			ro=new TGLreplay_object_position();

			name=o->get_class();
			ro->m_name=new char[strlen(name)+1];
			strcpy(ro->m_name,name);
			ro->m_x=o->get_x();
			ro->m_y=o->get_y();
			ro->m_a=o->get_angle();

			if (o->is_a("TGLobject_ship")) {
				ro->m_speed_x=((TGLobject_ship *)o)->get_speedx();
				ro->m_speed_y=((TGLobject_ship *)o)->get_speedy();
			} else if (o->is_a("TGLobject_ball")) {
				ro->m_speed_x=((TGLobject_ball *)o)->get_speed_x();
				ro->m_speed_y=((TGLobject_ball *)o)->get_speed_y();
			} else {
				ro->m_speed_x=0;
				ro->m_speed_y=0;
			} // if

			node->m_objects.Add(ro);
		} // while

		m_replay.Add(node);

	} else {
		node = new TGLreplay_node();
		node->m_keyframe=false;
		
		m_input->Rewind();
		while(m_input->Iterate(vc)) {
			vc2=new VirtualController(vc);
			node->m_input.Add(vc2);
		} // while

		m_replay.Add(node);
	} // if 

} /* TGLreplay::cycle */ 
