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
	char *tmp;
	TGLreplay_node *node;
	VirtualController *vc;
	TGLreplay_object_position *op;
	int i;

	fprintf(fp,"<replay>\n");
	fprintf(fp,"  <version>%s</version>\n",m_version);
	fprintf(fp,"  <date>\n");
	fprintf(fp,"    <day>%i</day>\n",m_day);
	fprintf(fp,"    <month>%i</month>\n",m_month);
	fprintf(fp,"    <year>%i</year>\n",m_year);
	fprintf(fp,"    <hour>%i</hour>\n",m_hour);
	fprintf(fp,"    <minute>%i</minute>\n",m_minute);
	fprintf(fp,"    <second>%i</second>\n",m_second);
	fprintf(fp,"  </date>\n");
	fprintf(fp,"  <map>%s</map>\n",m_map);
	
	fprintf(fp,"  <players>\n");
	m_players.Rewind();
	while(m_players.Iterate(tmp)) {
		fprintf(fp,"    <player>%i</player>\n",tmp);
	} // while 
	fprintf(fp,"  </players>\n");

	fprintf(fp,"  <cycles>\n");
	i=0;
	m_replay.Rewind();
	while(m_replay.Iterate(node)) {
		fprintf(fp,"    <cycle num=\"%i\">\n",i++);
		fprintf(fp,"      <input>\n");
		node->m_input.Rewind();
		while(node->m_input.Iterate(vc)) {
			fprintf(fp,"        <vc>\n");
			fprintf(fp,"          <current>%s %s %s %s %s %s %s %s</current>\n",
				(vc->m_joystick[0] ? "true":"false"),
				(vc->m_joystick[1] ? "true":"false"),
				(vc->m_joystick[2] ? "true":"false"),
				(vc->m_joystick[3] ? "true":"false"),
				(vc->m_button[0] ? "true":"false"),
				(vc->m_button[1] ? "true":"false"),
				(vc->m_pause ? "true":"false"),
				(vc->m_quit ? "true":"false"));
			fprintf(fp,"          <old>%s %s %s %s %s %s %s %s</old>\n",
				(vc->m_old_joystick[0] ? "true":"false"),
				(vc->m_old_joystick[1] ? "true":"false"),
				(vc->m_old_joystick[2] ? "true":"false"),
				(vc->m_old_joystick[3] ? "true":"false"),
				(vc->m_old_button[0] ? "true":"false"),
				(vc->m_old_button[1] ? "true":"false"),
				(vc->m_old_pause ? "true":"false"),
				(vc->m_old_quit ? "true":"false"));
			fprintf(fp,"        </vc>\n");
		} // while 
		fprintf(fp,"      </input>\n");

		if (node->m_keyframe) {
			fprintf(fp,"      <objects>\n");
			node->m_objects.Rewind();
			while(node->m_objects.Iterate(op)) {
				fprintf(fp,"        <object>\n");
				fprintf(fp,"          <name>%s</name>\n",op->m_name);
				fprintf(fp,"          <x>%f</x>\n",op->m_x);
				fprintf(fp,"          <y>%f</y>\n",op->m_y);
				fprintf(fp,"          <speedx>%f</speedx>\n",op->m_speed_x);
				fprintf(fp,"          <speedy>%f</speedx>\n",op->m_speed_y);
				fprintf(fp,"          <angle>%i</angle>\n",op->m_a);
				fprintf(fp,"        </object>\n");
			} // while 
			fprintf(fp,"      </objects>\n");
		} // if 
		fprintf(fp,"    </cycle>\n");
	} // while 
	fprintf(fp,"  </cycles>\n");

	fprintf(fp,"</replay>\n");

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
