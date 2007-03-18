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
#include "XMLparser.h"

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
	XMLNode *node=XMLNode::from_file(fp);
	XMLNode *version,*date,*map,*players,*cycles;

	m_version=0;
	m_map=0;
	m_year=0;
	m_month=0;
	m_day=0;
	m_hour=0;
	m_minute=0;
	m_second=0;

	version=node->get_children("version");
	date=node->get_children("date");
	map=node->get_children("map");
	players=node->get_children("players");
	cycles=node->get_children("cycles");

	if (version!=0) {
		m_version=new char[strlen(version->get_value()->get())+1];
		strcpy(m_version,version->get_value()->get());
	} // if 

	if (date!=0) {
		XMLNode *tmp;

		tmp=date->get_children("year");
		if (tmp!=0) m_year=atoi(tmp->get_value()->get());
		tmp=date->get_children("month");
		if (tmp!=0) m_month=atoi(tmp->get_value()->get());
		tmp=date->get_children("day");
		if (tmp!=0) m_day=atoi(tmp->get_value()->get());
		tmp=date->get_children("hour");
		if (tmp!=0) m_hour=atoi(tmp->get_value()->get());
		tmp=date->get_children("minute");
		if (tmp!=0) m_minute=atoi(tmp->get_value()->get());
		tmp=date->get_children("second");
		if (tmp!=0) m_second=atoi(tmp->get_value()->get());
	} // if 

	if (map!=0) {
		m_map=new char[strlen(map->get_value()->get())+1];
		strcpy(m_map,map->get_value()->get());
	} // if 

	// players
	if (players!=0) {
		List<XMLNode> *l;
		XMLNode *player,*tmp;

		l=players->get_children();
		l->Rewind();
		while(l->Iterate(player)) {
			tmp=player->get_children("name");
			if (tmp!=0) {
				char *name=new char[strlen(tmp->get_value()->get())+1];
				strcpy(name,tmp->get_value()->get());
				m_players.Add(name);
			} // if 
			tmp=player->get_children("ship");
			if (tmp!=0) {
				m_ships.Add(new int(atoi(tmp->get_value()->get())));
			} // if 
		} // while 
		delete l;
	} // if 

	// cycles
	if (cycles!=0) {
		List<XMLNode> *l;
		XMLNode *cycle,*input,*state;
		TGLreplay_node *node;
		VirtualController *vc;
		char stmp1[16],stmp2[16],stmp3[16],stmp4[16],stmp5[16],stmp6[16],stmp7[16],stmp8[16];

		l=cycles->get_children();
		l->Rewind();
		while(l->Iterate(cycle)) {
		
			node=new TGLreplay_node();

			input=cycle->get_children("input");
			state=cycle->get_children("state");

			if (input!=0) {
				List<XMLNode> *l2;
				XMLNode *vc_node,*tmp;

				l2=input->get_children();
				l2->Rewind();
				while(l2->Iterate(vc_node)) {

					vc=new VirtualController();

					tmp=vc_node->get_children("current");
					if (tmp!=0) {
						sscanf(tmp->get_value()->get(),"%s %s %s %s %s %s %s %s",stmp1,stmp2,stmp3,stmp4,stmp5,stmp6,stmp7,stmp8);
						vc->m_joystick[0]=(strcmp(stmp1,"true")==0 ? true:false);
						vc->m_joystick[1]=(strcmp(stmp2,"true")==0 ? true:false);
						vc->m_joystick[2]=(strcmp(stmp3,"true")==0 ? true:false);
						vc->m_joystick[3]=(strcmp(stmp4,"true")==0 ? true:false);
						vc->m_button[0]=(strcmp(stmp5,"true")==0 ? true:false);
						vc->m_button[1]=(strcmp(stmp6,"true")==0 ? true:false);
						vc->m_pause=(strcmp(stmp7,"true")==0 ? true:false);
						vc->m_quit=(strcmp(stmp8,"true")==0 ? true:false);								    
					} // if 

					tmp=vc_node->get_children("old");
					if (tmp!=0) {
						sscanf(tmp->get_value()->get(),"%s %s %s %s %s %s %s %s",stmp1,stmp2,stmp3,stmp4,stmp5,stmp6,stmp7,stmp8);
						vc->m_old_joystick[0]=(strcmp(stmp1,"true")==0 ? true:false);
						vc->m_old_joystick[1]=(strcmp(stmp2,"true")==0 ? true:false);
						vc->m_old_joystick[2]=(strcmp(stmp3,"true")==0 ? true:false);
						vc->m_old_joystick[3]=(strcmp(stmp4,"true")==0 ? true:false);
						vc->m_old_button[0]=(strcmp(stmp5,"true")==0 ? true:false);
						vc->m_old_button[1]=(strcmp(stmp6,"true")==0 ? true:false);
						vc->m_old_pause=(strcmp(stmp7,"true")==0 ? true:false);
						vc->m_old_quit=(strcmp(stmp8,"true")==0 ? true:false);								    
					} // if 

					node->m_input.Add(vc);

				} // while 
			} // if 

			if (state!=0) {
				node->m_keyframe=true;
				List<XMLNode> *l2;
				XMLNode *object,*tmp;
				TGLreplay_object_position *op;

				l2=state->get_children();
				l2->Rewind();
				while(l2->Iterate(object)) {
					op=new TGLreplay_object_position();

					tmp=object->get_children("name");
					if (tmp!=0) {
						op->m_name=new char[strlen(tmp->get_value()->get())+1];
						strcpy(op->m_name,tmp->get_value()->get());
					} else {
						op->m_name=0;
					} // if 

					tmp=object->get_children("x");
					if (tmp!=0) {
						op->m_x=float(atof(tmp->get_value()->get()));
					} // if 

					tmp=object->get_children("y");
					if (tmp!=0) {
						op->m_y=float(atof(tmp->get_value()->get()));
					} // if 

					tmp=object->get_children("speedx");
					if (tmp!=0) {
						op->m_speed_x=float(atof(tmp->get_value()->get()));
					} // if 

					tmp=object->get_children("speedy");
					if (tmp!=0) {
						op->m_speed_y=float(atof(tmp->get_value()->get()));
					} // if 

					tmp=object->get_children("angle");
					if (tmp!=0) {
						op->m_a=atoi(tmp->get_value()->get());
					} // if 

					node->m_objects.Add(op);
				} // while 

			} else {
				node->m_keyframe=false;
			} // if 
			
			m_replay.Add(node);
		} // while 
		delete l;
	} // if 

	delete node;
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


void TGLreplay::add_player(char *player_name,int ship)
{
	char *tmp=new char[strlen(player_name)+1];
	strcpy(tmp,player_name);
	m_players.Add(tmp);
	m_ships.Add(new int(ship));
} /* TGLreplay::add_player */ 


bool TGLreplay::save(FILE *fp)
{
	char *tmp;
	TGLreplay_node *node;
	VirtualController *vc;
	TGLreplay_object_position *op;
	int i,*itmp;

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
	m_ships.Rewind();
	while(m_players.Iterate(tmp) && m_ships.Iterate(itmp)) {
		fprintf(fp,"    <player>\n");
		fprintf(fp,"      <name>%s</name>\n",tmp);
		fprintf(fp,"      <ship>%i</ship>\n",*itmp);
		fprintf(fp,"    </player>\n",tmp);
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
			fprintf(fp,"      <state>\n");
			node->m_objects.Rewind();
			while(node->m_objects.Iterate(op)) {
				fprintf(fp,"        <object>\n");
				fprintf(fp,"          <name>%s</name>\n",op->m_name);
				fprintf(fp,"          <x>%.20f</x>\n",op->m_x);
				fprintf(fp,"          <y>%.20f</y>\n",op->m_y);
				fprintf(fp,"          <speedx>%.20f</speedx>\n",op->m_speed_x);
				fprintf(fp,"          <speedy>%.20f</speedx>\n",op->m_speed_y);
				fprintf(fp,"          <angle>%i</angle>\n",op->m_a);
				fprintf(fp,"        </object>\n");
			} // while 
			fprintf(fp,"      </state>\n");
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

} /* TGLreplay::store_cycle */ 


bool TGLreplay::execute_cycle(List<VirtualController> *m_input,List<TGLobject> *m_objects,List<TGLobject> *m_todelete,List<TGLobject> *m_toadd)
{
	TGLreplay_node *node;
	bool retval;
	VirtualController *vc1,*vc2;

	retval=m_replay.Iterate(node);

	m_input->Rewind();
	node->m_input.Rewind();
	while(m_input->Iterate(vc1) && node->m_input.Iterate(vc2)) vc1->copy(vc2);

	if (node->m_keyframe) {
		
		// ...

	} // if 

	return retval;
} /* TGLreplay::execute_cycle */ 


void TGLreplay::rewind(void)
{
	m_replay.Rewind();
} /* TGLreplay::rewind */ 


int TGLreplay::get_playership(char *player_name)
{
	int pos=m_players.Position(player_name);

	if (pos>=0) return *(m_ships[pos]);

	return -1;
} /* TGLreplay::get_playership */ 