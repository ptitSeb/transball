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
#include <time.h>
#include <math.h>
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
#include "compression.h"

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


char *objects_to_ignore[]={"TGLobject_ballstand",
						   "TGLobject_button",
						   "TGLobject_fuelrecharge",
						   "TGLobject_pipescreeen",
						   "TGLobject_pipevscreeen",
						   "TGLobject_radar",
						   "TGLobject_redlight",
						   "TGLobject_techno_computer",
						   "TGLobject_laser_horizontal",
						   "TGLobject_laser_vertical",
						   0};

TGLreplay_object_position::~TGLreplay_object_position()
{
	if (m_name!=0) {
		delete []m_name;
		m_name=0;
	} // if 
} /* TGLreplay_object_position::~TGLreplay_object_position */ 


TGLreplay_node::TGLreplay_node()
{
	m_keyframe=false;
} /* TGLreplay_node::TGLreplay_node */  


TGLreplay_node::~TGLreplay_node()
{
} /* TGLreplay_node::~TGLreplay_node */  


TGLreplay::TGLreplay(FILE *fp)
{
	// Load a replay:
	XMLNode *node;
	XMLNode *version=0,*date=0,*map=0,*players=0,*cycles=0,*fuel=0,*length=0;

	m_buffer=0;
	m_buffer_position=0;

	// Warning: This code assumes that 'fp' was open in binary mode!!!
/*
	{
		long l;

		fseek(fp,0, SEEK_END);
		l = ftell(fp);
		fseek(fp,0, SEEK_SET);

		m_buffer=new char[l];
		fread(m_buffer,l,1,fp);
	}
*/

	// Decompress the file:
	{
		unsigned l;
		m_buffer=decompress(fp,&l);
	}


	node=XMLNode::from_string(m_buffer,&m_buffer_position);

	m_version=0;
	m_map=0;
	m_year=0;
	m_month=0;
	m_day=0;
	m_hour=0;
	m_minute=0;
	m_second=0;
	m_initial_fuel=100;
	m_length=0;

	if (node!=0) {
		version=node->get_children("version");
		date=node->get_children("date");
		map=node->get_children("map");
		fuel=node->get_children("initial-fuel");
		players=node->get_children("players");
		cycles=node->get_children("cycles");
		length=node->get_children("length");
	} // if 

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

	if (fuel!=0) {
		m_initial_fuel=atoi(fuel->get_value()->get());
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

	if (length!=0) {
		m_length=atoi(length->get_value()->get());
	} // if 

	delete node;

	m_statistics_computed = false;
	m_max_speed = 0;
	m_average_speed = 0;
	m_average_speed_tmp = 0;

} /* TGLreplay::TGLreplay */ 


TGLreplay::TGLreplay(char *map)
{
	m_version=new char[strlen(application_version)+1];
	strcpy(m_version,application_version);

	m_buffer=0;
	m_buffer_position=-1;
	m_length=0;

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

	m_statistics_computed = false;
	m_max_speed = 0;
	m_average_speed = 0;
	m_average_speed_tmp = 0;
} /* TGLreplay::TGLreplay */ 


TGLreplay::~TGLreplay()
{
	if (m_buffer!=0) delete []m_buffer;
	m_buffer=0;

	if (m_version!=0) delete []m_version;
	m_version=0;

	if (m_map!=0) delete []m_map;
	m_map=0;
} /* TGLreplay::~TGLreplay */ 


bool TGLreplay::read_one_cycle(void)
{
	XMLNode *cycle;

	cycle=XMLNode::from_string(m_buffer,&m_buffer_position);

	if (cycle!=0) {
		XMLNode *input,*state,*text;
		TGLreplay_node *node;
		VirtualController *vc;
		char stmp1[16],stmp2[16],stmp3[16],stmp4[16],stmp5[16],stmp6[16],stmp7[16],stmp8[16];
	
		node=new TGLreplay_node();

		input=cycle->get_children("input");
		text=cycle->get_children("text");
		state=cycle->get_children("state");

		if (input!=0) {
			List<XMLNode> *l2;
			XMLNode *vc_node;

			l2=input->get_children();
			l2->Rewind();
			while(l2->Iterate(vc_node)) {

				vc=new VirtualController();

				sscanf(vc_node->get_value()->get(),"%s %s %s %s %s %s %s %s",stmp1,stmp2,stmp3,stmp4,stmp5,stmp6,stmp7,stmp8);
				vc->m_joystick[0]=(stmp1[0]=='t' ? true:false);
				vc->m_joystick[1]=(stmp2[0]=='t' ? true:false);
				vc->m_joystick[2]=(stmp3[0]=='t' ? true:false);
				vc->m_joystick[3]=(stmp4[0]=='t' ? true:false);
				vc->m_button[0]=(stmp5[0]=='t' ? true:false);
				vc->m_button[1]=(stmp6[0]=='t' ? true:false);
				vc->m_pause=(stmp7[0]=='t' ? true:false);
				vc->m_quit=(stmp8[0]=='t' ? true:false);								    

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
					op->m_x=load_float(tmp->get_value()->get());
				} // if 

				tmp=object->get_children("y");
				if (tmp!=0) {
					op->m_y=load_float(tmp->get_value()->get());
				} // if 

				tmp=object->get_children("speedx");
				if (tmp!=0) {
					op->m_speed_x=load_float(tmp->get_value()->get());
				} // if 

				tmp=object->get_children("speedy");
				if (tmp!=0) {
					op->m_speed_y=load_float(tmp->get_value()->get());
				} // if 

				tmp=object->get_children("angle");
				if (tmp!=0) {
					op->m_a=atoi(tmp->get_value()->get());
				} // if 

				if (!replay_ignored_object(op)) {
					node->m_objects.Add(op);
				} else {
					delete op;
				} // if 
			} // while 

		} else {
			node->m_keyframe=false;
		} // if 
		
		m_replay.Add(node);
		return true;
	} // if 

	return false;
} /* TGLreplay::read_one_cycle */ 


int TGLreplay::get_length(void)
{
	return m_length;
} /* TGLreplay::get_length */ 


void TGLreplay::add_player(char *player_name,int ship)
{
	char *tmp=new char[strlen(player_name)+1];
	strcpy(tmp,player_name);
	m_players.Add(tmp);
	m_ships.Add(new int(ship));
} /* TGLreplay::add_player */ 


bool TGLreplay::save(FILE *fp_out)
{
	char *tmp;
	TGLreplay_node *node;
	VirtualController *vc;
	TGLreplay_object_position *op;
	int i,*itmp;
	char buff[256],*buff2;
	List<char> elements;


	sprintf(buff,"<replay-info>\n");
	buff2=new char[strlen(buff)+1];
	strcpy(buff2,buff);
	elements.Add(buff2);
	sprintf(buff,"<version>%s</version>\n",m_version);
	buff2=new char[strlen(buff)+1];
	strcpy(buff2,buff);
	elements.Add(buff2);
	sprintf(buff,"<date>\n");
	buff2=new char[strlen(buff)+1];
	strcpy(buff2,buff);
	elements.Add(buff2);
	sprintf(buff,"<day>%i</day>\n",m_day);
	buff2=new char[strlen(buff)+1];
	strcpy(buff2,buff);
	elements.Add(buff2);
	sprintf(buff,"<month>%i</month>\n",m_month);
	buff2=new char[strlen(buff)+1];
	strcpy(buff2,buff);
	elements.Add(buff2);
	sprintf(buff,"<year>%i</year>\n",m_year);
	buff2=new char[strlen(buff)+1];
	strcpy(buff2,buff);
	elements.Add(buff2);
	sprintf(buff,"<hour>%i</hour>\n",m_hour);
	buff2=new char[strlen(buff)+1];
	strcpy(buff2,buff);
	elements.Add(buff2);
	sprintf(buff,"<minute>%i</minute>\n",m_minute);
	buff2=new char[strlen(buff)+1];
	strcpy(buff2,buff);
	elements.Add(buff2);
	sprintf(buff,"<second>%i</second>\n",m_second);
	buff2=new char[strlen(buff)+1];
	strcpy(buff2,buff);
	elements.Add(buff2);
	sprintf(buff,"</date>\n");
	buff2=new char[strlen(buff)+1];
	strcpy(buff2,buff);
	elements.Add(buff2);
	sprintf(buff,"<map>%s</map>\n",m_map);
	buff2=new char[strlen(buff)+1];
	strcpy(buff2,buff);
	elements.Add(buff2);
	sprintf(buff,"<players>\n");
	buff2=new char[strlen(buff)+1];
	strcpy(buff2,buff);
	elements.Add(buff2);

	m_players.Rewind();
	m_ships.Rewind();
	while(m_players.Iterate(tmp) && m_ships.Iterate(itmp)) {
		sprintf(buff,"<player><name>%s</name><ship>%i</ship></player>\n",tmp,*itmp);
		buff2=new char[strlen(buff)+1];
		strcpy(buff2,buff);
		elements.Add(buff2);
	} // while 
	
	sprintf(buff,"</players>\n");
	buff2=new char[strlen(buff)+1];
	strcpy(buff2,buff);
	elements.Add(buff2);

	sprintf(buff,"<length>%i</length>\n",m_replay.Length());
	buff2=new char[strlen(buff)+1];
	strcpy(buff2,buff);
	elements.Add(buff2);

	sprintf(buff,"</replay-info>\n");
	buff2=new char[strlen(buff)+1];
	strcpy(buff2,buff);
	elements.Add(buff2);	

	i=0;
	m_replay.Rewind();
	while(m_replay.Iterate(node)) {

		sprintf(buff,"<cycle num=\"%i\">\n<input>\n",i++);
		buff2=new char[strlen(buff)+1];
		strcpy(buff2,buff);
		elements.Add(buff2);

		node->m_input.Rewind();
		while(node->m_input.Iterate(vc)) {
			sprintf(buff,"<vc>%s %s %s %s %s %s %s %s</vc>",
				(vc->m_joystick[0] ? "t":"f"),
				(vc->m_joystick[1] ? "t":"f"),
				(vc->m_joystick[2] ? "t":"f"),
				(vc->m_joystick[3] ? "t":"f"),
				(vc->m_button[0] ? "t":"f"),
				(vc->m_button[1] ? "t":"f"),
				(vc->m_pause ? "t":"f"),
				(vc->m_quit ? "t":"f"));
			buff2=new char[strlen(buff)+1];
			strcpy(buff2,buff);
			elements.Add(buff2);
		} // while 
		sprintf(buff,"</input>\n");
		buff2=new char[strlen(buff)+1];
		strcpy(buff2,buff);
		elements.Add(buff2);

		if (node->m_keyframe) {
			sprintf(buff,"<state>\n");
			buff2=new char[strlen(buff)+1];
			strcpy(buff2,buff);
			elements.Add(buff2);

			node->m_objects.Rewind();
			while(node->m_objects.Iterate(op)) {

				sprintf(buff,"<object>\n<name>%s</name>\n<x>",op->m_name);
				buff2=new char[strlen(buff)+1];
				strcpy(buff2,buff);
				elements.Add(buff2);				

				save_float(op->m_x,buff);
				buff2=new char[strlen(buff)+1];
				strcpy(buff2,buff);
				elements.Add(buff2);				

				sprintf(buff,"</x>\n<y>");
				buff2=new char[strlen(buff)+1];
				strcpy(buff2,buff);
				elements.Add(buff2);				

				save_float(op->m_y,buff);
				buff2=new char[strlen(buff)+1];
				strcpy(buff2,buff);
				elements.Add(buff2);				
				
				sprintf(buff,"</y>\n<speedx>");
				buff2=new char[strlen(buff)+1];
				strcpy(buff2,buff);
				elements.Add(buff2);				

				save_float(op->m_speed_x,buff);
				buff2=new char[strlen(buff)+1];
				strcpy(buff2,buff);
				elements.Add(buff2);				
				
				sprintf(buff,"</speedx>\n<speedy>");
				buff2=new char[strlen(buff)+1];
				strcpy(buff2,buff);
				elements.Add(buff2);				

				save_float(op->m_speed_y,buff);
				buff2=new char[strlen(buff)+1];
				strcpy(buff2,buff);
				elements.Add(buff2);				

				sprintf(buff,"</speedy>\n<angle>%i</angle>\n</object>\n",op->m_a);
				buff2=new char[strlen(buff)+1];
				strcpy(buff2,buff);
				elements.Add(buff2);				
			} // while 
			sprintf(buff,"</state>\n");
			buff2=new char[strlen(buff)+1];
			strcpy(buff2,buff);
			elements.Add(buff2);				
		} // if 
		sprintf(buff,"</cycle>\n");
		buff2=new char[strlen(buff)+1];
		strcpy(buff2,buff);
		elements.Add(buff2);				
	} // while 
	
	{
		int len,pos;
		char *token;

		len=0;
		elements.Rewind();
		while(elements.Iterate(token)) {
			len+=strlen(token)+1;
		} /* while */ 
		len++;

		tmp=new char[len];

		pos=0;
		elements.Rewind();
		while(elements.Iterate(token)) {
			sprintf(tmp+pos,"%s",token);
			pos+=strlen(token);
		} /* while */ 
		tmp[pos]=0;


//		fprintf(fp_out,"%s",tmp);

		compress(tmp,len,fp_out);
		delete []tmp;
	}

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

	m_statistics_computed = false;

	{
		m_objects->Rewind();
		while(m_objects->Iterate(o)) {
			if (o->is_a("TGLobject_ship")) {
				float sx = ((TGLobject_ship *)o)->get_speedx();
				float sy = ((TGLobject_ship *)o)->get_speedy();
				float speed = float(sqrt(sx*sx+sy*sy));
				m_average_speed_tmp += speed;
				m_average_speed = m_average_speed_tmp/(m_length+1);
				if (speed>m_max_speed) m_max_speed = speed;
			} // if
		} // while 
	}


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
			
			if (!replay_ignored_object(o)) {
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
			} // if
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

	m_length++;

} /* TGLreplay::store_cycle */ 


bool TGLreplay::execute_cycle(List<VirtualController> *m_input,List<TGLobject> *m_objects,List<TGLobject> *m_todelete,List<TGLobject> *m_toadd)
{
	TGLreplay_node *node;
	bool retval;
	VirtualController *vc1,*vc2;
	TGLreplay_object_position *ro;
	TGLobject *go,*go_found;

	retval=read_one_cycle();

	m_replay.Forward();
	node=m_replay.GetObj();

	m_input->Rewind();
	node->m_input.Rewind();
	while(m_input->Iterate(vc1) && node->m_input.Iterate(vc2)) {
		vc1->new_cycle();
		vc1->copy_current(vc2);
	} // while 

	if (node->m_keyframe) {
		node->m_objects.Rewind();
		while(node->m_objects.Iterate(ro)) {
			float closest=0,d;
			go_found=0;
			m_objects->Rewind();
			while(m_objects->Iterate(go) && go_found==0) {
				if (strcmp(go->get_class(),ro->m_name)==0) { 
					d=(go->get_x()-ro->m_x)*(go->get_x()-ro->m_x)+(go->get_y()-ro->m_y)*(go->get_y()-ro->m_y);
					if (go_found==0 || d<closest) {
						go_found=go;
						closest=d;
					} // if
				} // if 
			} // while 

			if (go_found!=0) {
				m_objects->DeleteElement(go_found);
#ifdef __DEBUG_MESSAGES
				if (go_found->get_x()!=ro->m_x ||
					go_found->get_y()!=ro->m_y) {
					output_debug_message("TGLreplay: [%i] Adjustment in object '%s' (%g,%g) -> (%g,%g)\n",m_replay.PositionRef(node),ro->m_name,go_found->get_x(),go_found->get_y(),ro->m_x,ro->m_y);
				} // if 
#endif
				go_found->set_x(ro->m_x);
				go_found->set_y(ro->m_y);
				if (go_found->is_a("TGLobject_ship")) {
					((TGLobject_ship *)go_found)->set_speedx(ro->m_speed_x);
					((TGLobject_ship *)go_found)->set_speedy(ro->m_speed_y);
				} else if (go_found->is_a("TGLobject_ball")) {
					((TGLobject_ball *)go_found)->set_speed_x(ro->m_speed_x);
					((TGLobject_ball *)go_found)->set_speed_y(ro->m_speed_y);
				} // if
			} else {
				// Create a new object:
				// ...
//				m_toadd->Add(ro);
			} // if 
		} // while 

		while(!m_objects->EmptyP()) {
			go=m_objects->ExtractIni();
			if (!replay_ignored_object(go)) m_todelete->Add(go);
		} // while
		
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


char *TGLreplay::get_map(void)
{
	return m_map;
} /* TGLreplay::get_map */ 


int TGLreplay::get_initial_fuel(void)
{
	return m_initial_fuel;
} /* TGLreplay::get_initial_fuel */ 


float TGLreplay::get_max_speed(void)
{
	return m_max_speed;
} /* TGLreplay::get_max_speed */ 


float TGLreplay::get_average_speed(void)
{
	return m_average_speed;
} /* TGLreplay::get_average_speed */ 


int TGLreplay::get_fuel_used(void)
{
	if (m_statistics_computed) {
		return m_used_fuel;
	} else {
		compute_statistics();
		return m_used_fuel;
	} // if 
} /* TGLreplay::get_fuel_used */ 

int TGLreplay::get_n_shots(void)
{
	if (m_statistics_computed) {
		return m_n_shots;
	} else {
		compute_statistics();
		return m_n_shots;
	} // if 
} /* TGLreplay::get_n_shots */ 




void TGLreplay::compute_statistics(void)
{
	List<TGLreplay_node> l;
	TGLreplay_node *n;
	int ship = *(m_ships[0]);

	m_used_fuel = 0;
	m_n_shots = 0;

	l.Instance(m_replay);
	l.Rewind();
	while(l.Iterate(n)) {

		// Computing Fuel:
		if (n->m_input[0]->m_button[0]) {
			switch(ship) {
			case 0:	if (!n->m_input[0]->m_old_button[0]) {
						m_used_fuel+=64;
						m_n_shots++;
					} // if
					break;
			case 1: if (!n->m_input[0]->m_old_button[0]) {
						m_used_fuel+=96;
						m_n_shots++;
					} // if 
					break;
			case 2: if (!n->m_input[0]->m_old_button[0]) {
						m_used_fuel+=40;
						m_n_shots++;
					} // if
					break;
			case 3: if (!n->m_input[0]->m_old_button[0]) {
						m_used_fuel+=40;
						m_n_shots++;
					} // if
					break;
			case 4: if (!n->m_input[0]->m_old_button[0]) m_n_shots++;
					m_used_fuel+=2;
					break;
			case 5: if (!n->m_input[0]->m_old_button[0]) {
						m_used_fuel+=40;
						m_n_shots++;
					} // if
					break;
			case 6: if (!n->m_input[0]->m_old_button[0]) {
						m_used_fuel+=40;
						m_n_shots++;
					} // if
					break;
			case 7: if (!n->m_input[0]->m_old_button[0]) {
						m_used_fuel+=40;
						m_n_shots++;
					} // if
					break;
			case 8: if (!n->m_input[0]->m_old_button[0]) {
						m_used_fuel+=64;
						m_n_shots++;
					} // if
					break;
			case 9: m_used_fuel+=6;
					break;
			case 10:if (!n->m_input[0]->m_old_button[0]) {
						m_used_fuel+=32;
					} // if
					break;
			} // switch
		} // if 
		if (n->m_input[0]->m_joystick[VC_UP]) {
			if (ship==3) {
				if (n->m_input[0]->m_joystick[VC_DOWN]) {
					m_used_fuel+=2;
				} else {
					m_used_fuel++;
				} // if 
			} else {
				m_used_fuel++;
			} // if
		} // if 
		if (n->m_input[0]->m_joystick[VC_DOWN]) {
			if (!n->m_input[0]->m_joystick[VC_UP] && ship==2) m_used_fuel++;
			if (n->m_input[0]->m_joystick[VC_LEFT] && ship==5) m_used_fuel++;
			if (n->m_input[0]->m_joystick[VC_RIGHT] && ship==5) m_used_fuel++;
			if (ship==6) m_used_fuel++;
		} // if
	} // while 
} /* TGLreplay::compute_statistics */ 


char *TGLreplay::get_playername(int player)
{
	return m_players[player];
} /* TGLreplay::get_playername */ 


bool TGLreplay::replay_ignored_object(TGLobject *o)
{
	int i;

	for(i=0;objects_to_ignore[i]!=0;i++) {
		if (strcmp(o->get_class(),objects_to_ignore[i])==0) return true;
	} // if 

	return false;
} /* TGLreplay::replay_ignored_object */ 


bool TGLreplay::replay_ignored_object(TGLreplay_object_position *o)
{
	int i;

	for(i=0;objects_to_ignore[i]!=0;i++) {
		if (strcmp(o->m_name,objects_to_ignore[i])==0) return true;
	} // if 

	return false;
} /* TGLreplay::replay_ignored_object */ 



void TGLreplay::save_float(float v,FILE *fp)
{
	char tmp[9];
	char *ptr=(char *)&v;

#if SDL_BYTEORDER == SDL_LITTLE_ENDIAN
	tmp[0]='a'+(ptr[0]&0xf);
	tmp[1]='a'+((ptr[0]>>4)&0xf);
	tmp[2]='a'+(ptr[1]&0xf);
	tmp[3]='a'+((ptr[1]>>4)&0xf);
	tmp[4]='a'+(ptr[2]&0xf);
	tmp[5]='a'+((ptr[2]>>4)&0xf);
	tmp[6]='a'+(ptr[3]&0xf);
	tmp[7]='a'+((ptr[3]>>4)&0xf);
#else
	tmp[0]='a'+(ptr[3]&0xf);
	tmp[1]='a'+((ptr[3]>>4)&0xf);
	tmp[2]='a'+(ptr[2]&0xf);
	tmp[3]='a'+((ptr[2]>>4)&0xf);
	tmp[4]='a'+(ptr[1]&0xf);
	tmp[5]='a'+((ptr[1]>>4)&0xf);
	tmp[6]='a'+(ptr[0]&0xf);
	tmp[7]='a'+((ptr[0]>>4)&0xf);
#endif
	tmp[8]=0;

	fprintf(fp,tmp);
} /* TGLreplay::save_float */ 


void TGLreplay::save_float(float v,char *tmp)
{
	char *ptr=(char *)&v;

#if SDL_BYTEORDER == SDL_LITTLE_ENDIAN
	tmp[0]='a'+(ptr[0]&0xf);
	tmp[1]='a'+((ptr[0]>>4)&0xf);
	tmp[2]='a'+(ptr[1]&0xf);
	tmp[3]='a'+((ptr[1]>>4)&0xf);
	tmp[4]='a'+(ptr[2]&0xf);
	tmp[5]='a'+((ptr[2]>>4)&0xf);
	tmp[6]='a'+(ptr[3]&0xf);
	tmp[7]='a'+((ptr[3]>>4)&0xf);
#else
	tmp[0]='a'+(ptr[3]&0xf);
	tmp[1]='a'+((ptr[3]>>4)&0xf);
	tmp[2]='a'+(ptr[2]&0xf);
	tmp[3]='a'+((ptr[2]>>4)&0xf);
	tmp[4]='a'+(ptr[1]&0xf);
	tmp[5]='a'+((ptr[1]>>4)&0xf);
	tmp[6]='a'+(ptr[0]&0xf);
	tmp[7]='a'+((ptr[0]>>4)&0xf);
#endif
	tmp[8]=0;
} /* TGLreplay::save_float */ 


float TGLreplay::load_float(char *str)
{
	float tmp=0;
	char *ptr=(char *)&tmp;

	if ((str[0]>='0' && str[0]<='9') || str[0]=='-') {
		tmp=float(atof(str));
	} else {
#if SDL_BYTEORDER == SDL_LITTLE_ENDIAN
		ptr[0]|=(str[0]-'a');
		ptr[0]|=(str[1]-'a')<<4;
		ptr[1]|=(str[2]-'a');
		ptr[1]|=(str[3]-'a')<<4;
		ptr[2]|=(str[4]-'a');
		ptr[2]|=(str[5]-'a')<<4;
		ptr[3]|=(str[6]-'a');
		ptr[3]|=(str[7]-'a')<<4;
#else
		ptr[3]|=(str[0]-'a');
		ptr[3]|=(str[1]-'a')<<4;
		ptr[2]|=(str[2]-'a');
		ptr[2]|=(str[3]-'a')<<4;
		ptr[1]|=(str[4]-'a');
		ptr[1]|=(str[5]-'a')<<4;
		ptr[0]|=(str[6]-'a');
		ptr[0]|=(str[7]-'a')<<4;
#endif
	} // if 

	return tmp;
} /* TGLreplay::load_float */ 