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

#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "SDL_rotozoom.h"

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
#include "TGLmap.h"
#include "TGLobject.h"
#include "TGLobject_ballstand.h"
#include "TGLobject_redlight.h"
#include "TGLobject_radar.h"
#include "TGLobject_pipevscreen.h"
#include "TGLobject_pipehscreen.h"
#include "TGLobject_enemy.h"
#include "TGLobject_laser_left.h"
#include "TGLobject_laser_right.h"
#include "TGLobject_laser_up.h"
#include "TGLobject_laser_down.h"
#include "TGLobject_laser_horizontal.h"
#include "TGLobject_laser_vertical.h"
#include "TGLobject_cannon_left.h"
#include "TGLobject_cannon_right.h"
#include "TGLobject_cannon_down.h"
#include "TGLobject_cannon_up.h"
#include "TGLobject_fastcannon_left.h"
#include "TGLobject_fastcannon_right.h"
#include "TGLobject_fastcannon_down.h"
#include "TGLobject_fastcannon_up.h"
#include "TGLobject_ball.h"
#include "TGLobject_spike_left.h"
#include "TGLobject_spike_right.h"
#include "TGLobject_fuelrecharge.h"
#include "TGLobject_techno_computer.h"
#include "TGLobject_directionalcannon_left.h"
#include "TGLobject_directionalcannon_right.h"
#include "TGLobject_directionalcannon_up.h"
#include "TGLobject_directionalcannon_down.h"
#include "TGLobject_ha_directionalcannon_left.h"
#include "TGLobject_ha_directionalcannon_right.h"
#include "TGLobject_ha_directionalcannon_up.h"
#include "TGLobject_ha_directionalcannon_down.h"
#include "TGLobject_tank.h"
#include "TGLobject_tank_turret.h"
#include "TGLobject_tank_cannon.h"
#include "TGLobject_big_tank.h"
#include "TGLobject_big_tank_cannon.h"
#include "TGLobject_leftdoor.h"
#include "TGLobject_rightdoor.h"
#include "TGLobject_button.h"
#include "TGLobject_ship.h"

#include "collision.h"

#include "debug.h"


#define MAPPAUSE_TIME	25

// extern GLTile *last_collision;


TGLmap::TGLmap(GLTManager *GLTM)
{
	int i;

	m_bg=0;
	m_star_x=0;
	m_star_y=0;
	m_star_color=0;
	m_cycle=0;

	m_fg_dx = 40;
	m_fg_dy = 32;
	m_fg_cell_size=32;

	m_fg=new GLTile *[m_fg_dx*m_fg_dy];
	for(i=0;i<m_fg_dx*m_fg_dy;i++) m_fg[i]=0;

	set_background(0,GLTM);

} /* TGLmap::TGLmap */ 


TGLmap::TGLmap(FILE *fp, GLTManager *GLTM)
{
	int i;
	char tmp[256];

	m_bg=0;
	m_star_x=0;
	m_star_y=0;
	m_star_color=0;
	m_cycle=0;

	if (2!=fscanf(fp,"%i %i",&m_fg_dx,&m_fg_dy)) return;
	m_fg_cell_size=32;

	if (1!=fscanf(fp,"%s",tmp)) return;

	if (strcmp(tmp,"rock")==0) set_background(0,GLTM);
	if (strcmp(tmp,"techno-red")==0) set_background(1,GLTM);
	if (strcmp(tmp,"techno-blue")==0) set_background(2,GLTM);
	if (strcmp(tmp,"techno-green")==0) set_background(3,GLTM);
	if (strcmp(tmp,"snow")==0) set_background(4,GLTM);

	m_fg=new GLTile *[m_fg_dx*m_fg_dy];
	for(i=0;i<m_fg_dx*m_fg_dy;i++) {
		if (1!=fscanf(fp,"%s",tmp)) return;
		if (strcmp(tmp,"-")==0) {
			m_fg[i]=0;
		} else {
			m_fg[i]=GLTM->get(tmp);
		} // if 
	} // for 

	// objects:
	{
		int no;
		int ix,iy;
		float x,y;
		if (1!=fscanf(fp,"%i",&no)) return;

		for(i=0;i<no;i++) {
			if (3!=fscanf(fp,"%s %i %i",tmp,&ix,&iy)) return;
			x=float(ix*m_fg_cell_size);
			y=float(iy*m_fg_cell_size);
			y+=STARFIELD;
			if (strcmp(tmp,"ball-stand")==0) {
				add_object(new TGLobject_ballstand(x,y));				
			} // if 
			if (strcmp(tmp,"red-light")==0) add_object(new TGLobject_redlight(x,y,0));
			if (strcmp(tmp,"red-light-snow")==0) add_object(new TGLobject_redlight(x,y,1));
			if (strcmp(tmp,"radar")==0) add_object(new TGLobject_radar(x,y));
			if (strcmp(tmp,"pipe-vertical-screen")==0) add_object(new TGLobject_pipevscreen(x,y));
			if (strcmp(tmp,"pipe-horizontal-screen")==0) add_object(new TGLobject_pipehscreen(x,y));
			if (strcmp(tmp,"laser-left")==0) add_object(new TGLobject_laser_left(x,y));
			if (strcmp(tmp,"laser-right")==0) add_object(new TGLobject_laser_right(x,y));
			if (strcmp(tmp,"laser-up")==0) add_object(new TGLobject_laser_up(x,y));
			if (strcmp(tmp,"laser-down")==0) add_object(new TGLobject_laser_down(x,y));
			if (strcmp(tmp,"cannon-rock-left")==0) add_object(new TGLobject_cannon_left(x,y,0));
			if (strcmp(tmp,"cannon-rock-right")==0) add_object(new TGLobject_cannon_right(x,y,0));
			if (strcmp(tmp,"cannon-rock-down")==0) add_object(new TGLobject_cannon_down(x,y,0));
			if (strcmp(tmp,"cannon-rock-up")==0) add_object(new TGLobject_cannon_up(x,y,0));
			if (strcmp(tmp,"cannon-techno-left")==0) add_object(new TGLobject_cannon_left(x,y,1));
			if (strcmp(tmp,"cannon-techno-right")==0) add_object(new TGLobject_cannon_right(x,y,1));
			if (strcmp(tmp,"cannon-techno-down")==0) add_object(new TGLobject_cannon_down(x,y,1));
			if (strcmp(tmp,"cannon-techno-up")==0) add_object(new TGLobject_cannon_up(x,y,1));
			if (strcmp(tmp,"fastcannon-techno-left")==0) add_object(new TGLobject_fastcannon_left(x,y));
			if (strcmp(tmp,"fastcannon-techno-right")==0) add_object(new TGLobject_fastcannon_right(x,y));
			if (strcmp(tmp,"fastcannon-techno-down")==0) add_object(new TGLobject_fastcannon_down(x,y));
			if (strcmp(tmp,"fastcannon-techno-up")==0) add_object(new TGLobject_fastcannon_up(x,y));
			if (strcmp(tmp,"spike-left")==0) add_object(new TGLobject_spike_left(x,y));
			if (strcmp(tmp,"spike-right")==0) add_object(new TGLobject_spike_right(x,y));
			if (strcmp(tmp,"fuel-recharge")==0) add_object(new TGLobject_fuelrecharge(x,y));
			if (strcmp(tmp,"wall-techno-computer")==0) add_object(new TGLobject_techno_computer(x,y));	
			if (strcmp(tmp,"directional-cannon-left")==0) add_object(new TGLobject_directionalcannon_left(x,y));	
			if (strcmp(tmp,"directional-cannon-right")==0) add_object(new TGLobject_directionalcannon_right(x,y));
			if (strcmp(tmp,"directional-cannon-up")==0) add_object(new TGLobject_directionalcannon_up(x,y));
			if (strcmp(tmp,"directional-cannon-down")==0) add_object(new TGLobject_directionalcannon_down(x,y));		
			if (strcmp(tmp,"ha-directional-cannon-left")==0) add_object(new TGLobject_ha_directionalcannon_left(x,y));	
			if (strcmp(tmp,"ha-directional-cannon-right")==0) add_object(new TGLobject_ha_directionalcannon_right(x,y));
			if (strcmp(tmp,"ha-directional-cannon-up")==0) add_object(new TGLobject_ha_directionalcannon_up(x,y));
			if (strcmp(tmp,"ha-directional-cannon-down")==0) add_object(new TGLobject_ha_directionalcannon_down(x,y));	
			if (strcmp(tmp,"grey-tank")==0) {
				TGLobject_enemy *tank,*turret,*cannon;
				tank=new TGLobject_tank(x,y,0);
				turret=new TGLobject_tank_turret(x,y,tank,0);
				cannon=new TGLobject_tank_cannon(x,y,turret);
				add_object(cannon);
				add_object(turret);
				add_object(tank);
				tank->exclude_for_collision(turret);
				tank->exclude_for_collision(cannon);
				turret->exclude_for_collision(tank);
				turret->exclude_for_collision(cannon);
				cannon->exclude_for_collision(tank);
				cannon->exclude_for_collision(turret);
			} // if 
			if (strcmp(tmp,"red-tank")==0) {
				TGLobject_enemy *tank,*turret,*cannon;
				tank=new TGLobject_tank(x,y,1);
				turret=new TGLobject_tank_turret(x,y,tank,1);
				cannon=new TGLobject_tank_cannon(x,y,turret);
				add_object(cannon);
				add_object(turret);
				add_object(tank);
				tank->exclude_for_collision(turret);
				tank->exclude_for_collision(cannon);
				turret->exclude_for_collision(tank);
				turret->exclude_for_collision(cannon);
				cannon->exclude_for_collision(tank);
				cannon->exclude_for_collision(turret);
			} // if 
			if (strcmp(tmp,"green-tank")==0) {
				TGLobject_enemy *tank,*turret,*cannon;
				tank=new TGLobject_tank(x,y,2);
				turret=new TGLobject_tank_turret(x,y,tank,2);
				cannon=new TGLobject_tank_cannon(x,y,turret);
				add_object(cannon);
				add_object(turret);
				add_object(tank);
				tank->exclude_for_collision(turret);
				tank->exclude_for_collision(cannon);
				turret->exclude_for_collision(tank);
				turret->exclude_for_collision(cannon);
				cannon->exclude_for_collision(tank);
				cannon->exclude_for_collision(turret);
			} // if 
			if (strcmp(tmp,"big-tank")==0) {
				TGLobject_enemy *tank,*turret,*cannon;
				tank=new TGLobject_big_tank(x,y);
				turret=new TGLobject_tank_turret(x,y,tank,3);
				cannon=new TGLobject_big_tank_cannon(x,y,turret);
				add_object(cannon);
				add_object(turret);
				add_object(tank);
				tank->exclude_for_collision(turret);
				tank->exclude_for_collision(cannon);
				turret->exclude_for_collision(tank);
				turret->exclude_for_collision(cannon);
				cannon->exclude_for_collision(tank);
				cannon->exclude_for_collision(turret);
			} // if 
			if (strcmp(tmp,"door-left")==0) {
				int p1,p2;
				if (2!=fscanf(fp,"%i %i",&p1,&p2)) return;
				add_object(new TGLobject_leftdoor(x,y,p1,p2));
			} // if 
			if (strcmp(tmp,"door-right")==0) {
				int p1,p2;
				if (2!=fscanf(fp,"%i %i",&p1,&p2)) return;
				add_object(new TGLobject_rightdoor(x,y,p1,p2));
			} // if 
			if (strcmp(tmp,"button-red-left")==0) {
				int p1;
				if (1!=fscanf(fp,"%i",&p1)) return;
				add_object(new TGLobject_button(x,y,p1,0));
			} // if 
			if (strcmp(tmp,"button-red-right")==0) {
				int p1;
				if (1!=fscanf(fp,"%i",&p1)) return;
				add_object(new TGLobject_button(x,y,p1,1));
			} // if 
			if (strcmp(tmp,"button-red-up")==0) {
				int p1;
				if (1!=fscanf(fp,"%i",&p1)) return;
				add_object(new TGLobject_button(x,y,p1,2));
			} // if 
			if (strcmp(tmp,"button-red-down")==0) {
				int p1;
				if (1!=fscanf(fp,"%i",&p1)) return;
				add_object(new TGLobject_button(x,y,p1,3));
			} // if 
			if (strcmp(tmp,"button-purple-left")==0) {
				int p1;
				if (1!=fscanf(fp,"%i",&p1)) return;
				add_object(new TGLobject_button(x,y,p1,4));
			} // if 
			if (strcmp(tmp,"button-purple-right")==0) {
				int p1;
				if (1!=fscanf(fp,"%i",&p1)) return;
				add_object(new TGLobject_button(x,y,p1,5));
			} // if 
			if (strcmp(tmp,"button-purple-up")==0) {
				int p1;
				if (1!=fscanf(fp,"%i",&p1)) return;
				add_object(new TGLobject_button(x,y,p1,6));
			} // if 
			if (strcmp(tmp,"button-purple-down")==0) {
				int p1;
				if (1!=fscanf(fp,"%i",&p1)) return;
				add_object(new TGLobject_button(x,y,p1,7));
			} // if 
			if (strcmp(tmp,"button-blue-left")==0) {
				int p1;
				if (1!=fscanf(fp,"%i",&p1)) return;
				add_object(new TGLobject_button(x,y,p1,8));
			} // if 
			if (strcmp(tmp,"button-blue-right")==0) {
				int p1;
				if (1!=fscanf(fp,"%i",&p1)) return;
				add_object(new TGLobject_button(x,y,p1,9));
			} // if 
			if (strcmp(tmp,"button-blue-up")==0) {
				int p1;
				if (1!=fscanf(fp,"%i",&p1)) return;
				add_object(new TGLobject_button(x,y,p1,10));
			} // if 
			if (strcmp(tmp,"button-blue-down")==0) {
				int p1;
				if (1!=fscanf(fp,"%i",&p1)) return;
				add_object(new TGLobject_button(x,y,p1,11));
			} // if 
		} // for 
	}

} /* TGLmap::TGLmap */ 


void TGLmap::reset(void)
{
	TGLobject_ballstand *bs = (TGLobject_ballstand *)object_exists("TGLobject_ballstand");
	m_cycle=0;

	if (bs!=0) {
		TGLobject_ball *ball = new TGLobject_ball(bs->get_x()+16,bs->get_y()+5);
		TGLobject_ship *ship = (TGLobject_ship *)object_exists("TGLobject_ship");
		m_fg_objects.Insert(ball);
		if (ball!=0 && ship!=0) {
			ball->exclude_for_collision(ship);
			ship->exclude_for_collision(ball);
		} // if 

	} // if 

	create_laser_objects();
} /* TGLmap::TGLmap */ 


void TGLmap::create_laser_objects(void) 
{
	List<TGLobject> l,l2,todelete;
	TGLobject *o,*o2,*pair;


	// First remove all the laser objects existing in the map:
	l.Instance(m_fg_objects);
	l.Rewind();
	while(l.Iterate(o)) {
		if (o->is_a("TGLobject_laser_horizontal") || o->is_a("TGLobject_laser_vertical")) todelete.Add(o);
	} // while 

	while(!todelete.EmptyP()) {
		o=todelete.ExtractIni();
		m_fg_objects.DeleteElement(o);
		delete o;
	} // while 

	l.Instance(m_fg_objects);
	l2.Instance(m_fg_objects);
	l.Rewind();
	while(l.Iterate(o)) {
		if (o->is_a("TGLobject_laser_left")) {
			// Fint pair:
			pair=0;
			l2.Rewind();
			while(l2.Iterate(o2)) {
				if (o2->is_a("TGLobject_laser_right") && 
					o2->get_y()==o->get_y() &&
					o2->get_x()>o->get_x()) {
					if (pair==0) pair=o2;
					if (pair->get_x()>o2->get_x()) pair=o2;
				} // if 
			} // while 

			if (pair!=0) {
				float i;
				for(i=o->get_x();i<=pair->get_x();i+=m_fg_cell_size) {
					m_fg_objects.Insert(new TGLobject_laser_horizontal(i,o->get_y(),o,pair));
				} // for
			} // if 
		} // if 

		if (o->is_a("TGLobject_laser_up")) {
			// Fint pair:
			pair=0;
			l2.Rewind();
			while(l2.Iterate(o2)) {
				if (o2->is_a("TGLobject_laser_down") && 
					o2->get_x()==o->get_x() &&
					o2->get_y()>o->get_y()) {
					if (pair==0) pair=o2;
					if (pair->get_y()>o2->get_y()) pair=o2;
				} // if 
			} // while 

			if (pair!=0) {
				float i;
				for(i=o->get_y();i<=pair->get_y();i+=m_fg_cell_size) {
					m_fg_objects.Insert(new TGLobject_laser_vertical(o->get_x(),i,o,pair));
				} // for
			} // if 
		} // if 
	} // while 
} /* TGLmap::create_laser_objects */ 


int TGLmap::get_cycle(void)
{
	return m_cycle;
} /* TGLmap::get_cycle */ 


void TGLmap::set_background(int type,GLTManager *GLTM)
{
	int i,j;

	m_bg_code = type;

	if (type==0) {
		// Create a rock backgorund:
		if (m_bg!=0) {
			for(i=0;i<m_bg_dx*m_bg_dy;i++) m_bg[i]=0;
			delete []m_bg;
		} // if
		
		m_bg_dx=(m_fg_dx+1)/2;
		m_bg_dy=(m_fg_dy+1)/2;
		m_bg_cell_size=64;

		m_bg=new GLTile *[m_bg_dx*m_bg_dy];
		for(i=0;i<m_bg_dx;i++) m_bg[i]=GLTM->get("background/top-rock");
		for(i=0;i<m_bg_dx;i++) m_bg[i+m_bg_dx]=GLTM->get("background/middle-rock");
		for(j=2;j<m_bg_dy;j++) {
			for(i=0;i<m_bg_dx;i++) m_bg[i+m_bg_dx*j]=GLTM->get("background/bottom-rock");
		} // for
	} // if 

	if (type==1) {
		// Create a techno-red backgorund:
		if (m_bg!=0) {
			for(i=0;i<m_bg_dx*m_bg_dy;i++) m_bg[i]=0;
			delete []m_bg;
		} // if

		m_bg_dx=(m_fg_dx+1)/2;
		m_bg_dy=(m_fg_dy+1)/2;
		m_bg_cell_size=64;

		m_bg=new GLTile *[m_bg_dx*m_bg_dy];
		for(i=0;i<m_bg_dx;i++) m_bg[i]=GLTM->get("background/top-techno-red");
		for(j=1;j<m_bg_dy;j++) {
			for(i=0;i<m_bg_dx;i++) m_bg[i+m_bg_dx*j]=GLTM->get("background/bottom-techno-red");
		} // for
	} // if 

	if (type==2) {
		// Create a techno-blue backgorund:
		if (m_bg!=0) {
			for(i=0;i<m_bg_dx*m_bg_dy;i++) m_bg[i]=0;
			delete []m_bg;
		} // if

		m_bg_dx=(m_fg_dx+1)/2;
		m_bg_dy=(m_fg_dy+1)/2;
		m_bg_cell_size=64;

		m_bg=new GLTile *[m_bg_dx*m_bg_dy];
		for(i=0;i<m_bg_dx;i++) m_bg[i]=GLTM->get("background/top-techno-blue");
		for(j=1;j<m_bg_dy;j++) {
			for(i=0;i<m_bg_dx;i++) m_bg[i+m_bg_dx*j]=GLTM->get("background/bottom-techno-blue");
		} // for
	} // if 

	if (type==3) {
		// Create a techno-green backgorund:
		if (m_bg!=0) {
			for(i=0;i<m_bg_dx*m_bg_dy;i++) m_bg[i]=0;
			delete []m_bg;
		} // if

		m_bg_dx=(m_fg_dx+1)/2;
		m_bg_dy=(m_fg_dy+1)/2;
		m_bg_cell_size=64;

		m_bg=new GLTile *[m_bg_dx*m_bg_dy];
		for(i=0;i<m_bg_dx;i++) m_bg[i]=GLTM->get("background/top-techno-green");
		for(j=1;j<m_bg_dy;j++) {
			for(i=0;i<m_bg_dx;i++) m_bg[i+m_bg_dx*j]=GLTM->get("background/bottom-techno-green");
		} // for
	} // if 

	if (type==4) {
		// Create a snow backgorund:
		if (m_bg!=0) {
			for(i=0;i<m_bg_dx*m_bg_dy;i++) m_bg[i]=0;
			delete []m_bg;
		} // if

		m_bg_dx=(m_fg_dx+1)/2;
		m_bg_dy=(m_fg_dy+1)/2;
		m_bg_cell_size=64;

		m_bg=new GLTile *[m_bg_dx*m_bg_dy];
		for(i=0;i<m_bg_dx;i++) m_bg[i]=0;
		for(i=0;i<m_bg_dx;i++) m_bg[i+m_bg_dx]=0;
		for(i=0;i<m_bg_dx;i+=2) m_bg[i]=GLTM->get("background/top-snow");
		for(j=2;j<m_bg_dy;j++) {
			for(i=0;i<m_bg_dx;i++) m_bg[i+m_bg_dx*j]=GLTM->get("background/bottom-snow");
		} // for
	} // if 

	if (m_star_x!=0) {
		delete []m_star_x;
		delete []m_star_y;
		delete []m_star_color;
	} // if

	{
		m_nstars=STARFIELD_STARS*m_fg_dx;
		m_star_x=new int[m_nstars];
		m_star_y=new int[m_nstars];
		m_star_color=new float[m_nstars];
		
		for(i=0;i<m_nstars;i++) {
			m_star_x[i]=rand()%(m_fg_dx*m_fg_cell_size);
			m_star_y[i]=rand()%(STARFIELD+32);
			m_star_color[i]=((rand()%1000)+1)*0.001f;
			m_star_color[i]*=m_star_color[i];
		} // for
	}


} /* TGLmap::set_background */ 


TGLmap::~TGLmap()
{
	int i;

	m_fg_ships.ExtractAll();

	for(i=0;i<m_bg_dx*m_bg_dy;i++) m_bg[i]=0;
	delete []m_bg;

	for(i=0;i<m_fg_dx*m_fg_dy;i++) m_fg[i]=0;
	delete []m_fg;

	delete []m_star_x;
	delete []m_star_y;
	delete []m_star_color;
} /* TGLmap::~TGLmap */ 


bool TGLmap::editor_cycle(GLTManager *GLTM)
{
	List<TGLobject> todelete;

	TGLobject *o;

	m_fg_objects.Rewind();
	while(m_fg_objects.Iterate(o)) {
		if (!o->editor_cycle(this,GLTM)) {
			todelete.Add(o);
		} // if 
	} // while 

	while(!todelete.EmptyP()) {
		o=todelete.ExtractIni();
		m_fg_ships.DeleteElement(o);
		m_fg_objects.DeleteElement(o);
		delete o;
	} // while 

	m_auxiliary_front_objects.Rewind();
	while(m_auxiliary_front_objects.Iterate(o)) {
		if (!o->editor_cycle(this,GLTM)) {
			todelete.Add(o);
		} // if 
	} // while 

	while(!todelete.EmptyP()) {
		o=todelete.ExtractIni();
		m_auxiliary_front_objects.DeleteElement(o);
		delete o;
	} // while 

	m_auxiliary_back_objects.Rewind();
	while(m_auxiliary_back_objects.Iterate(o)) {
		if (!o->editor_cycle(this,GLTM)) {
			todelete.Add(o);
		} // if 
	} // while 

	while(!todelete.EmptyP()) {
		o=todelete.ExtractIni();
		m_auxiliary_back_objects.DeleteElement(o);
		delete o;
	} // while 

	m_cycle++;

	return true;
} /* TGLmap::editor_cycle */ 


bool TGLmap::cycle(List<VirtualController> *lv,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{
	List<TGLobject> todelete;

	TGLobject *o;

	m_fg_objects.Rewind();
	while(m_fg_objects.Iterate(o)) {
		if (!o->cycle((*lv)[o->get_controller()],this,GLTM,SFXM,sfx_volume)) {
			todelete.Add(o);
		} // if 
	} // while 

	while(!todelete.EmptyP()) {
		o=todelete.ExtractIni();
		m_fg_ships.DeleteElement(o);
		m_fg_objects.DeleteElement(o);
		delete o;
	} // while 

	m_auxiliary_front_objects.Rewind();
	while(m_auxiliary_front_objects.Iterate(o)) {
		if (!o->cycle((*lv)[o->get_controller()],this,GLTM,SFXM,sfx_volume)) {
			todelete.Add(o);
		} // if 
	} // while 

	while(!todelete.EmptyP()) {
		o=todelete.ExtractIni();
		m_auxiliary_front_objects.DeleteElement(o);
		delete o;
	} // while 

	m_auxiliary_back_objects.Rewind();
	while(m_auxiliary_back_objects.Iterate(o)) {
		if (!o->cycle((*lv)[o->get_controller()],this,GLTM,SFXM,sfx_volume)) {
			todelete.Add(o);
		} // if 
	} // while 

	while(!todelete.EmptyP()) {
		o=todelete.ExtractIni();
		m_auxiliary_back_objects.DeleteElement(o);
		delete o;
	} // while 

	m_cycle++;

	return true;
} /* TGLmap::cycle */ 


void TGLmap::draw(int focus_x,int focus_y,int dx,int dy,GLTManager *GLTM)
{
	int offsx,offsy;

	if (focus_x>(m_fg_dx*m_fg_cell_size-dx/2)) focus_x=(m_fg_dx*m_fg_cell_size-dx/2);
	if (focus_y>(m_fg_dy*m_fg_cell_size+STARFIELD-dy/2)) focus_y=(m_fg_dy*m_fg_cell_size+STARFIELD-dy/2);
	if (focus_x<dx/2) focus_x=dx/2;
	if (focus_y<dy/2) focus_y=dy/2;

	offsx=focus_x-dx/2;
	offsy=focus_y-dy/2;

	// Draw starfield:
	{
		int i;
		glPushMatrix();
		glTranslatef(float(-(offsx/4)),float(-(offsy/4)),0);

		glBegin(GL_POINTS);
		for(i=0;i<m_nstars;i++) {
			glColor3f(m_star_color[i],m_star_color[i],m_star_color[i]);
			glVertex3f(float(m_star_x[i]),float(m_star_y[i]),0);
		} // for
		glEnd();


		for(i=0;i<m_nstars;i++) {
			glPushMatrix();
			glTranslatef(float(m_star_x[i]),float(m_star_y[i]),0);
			draw_glow(8,m_star_color[i]*6,m_star_color[i],m_star_color[i],m_star_color[i],0.125f);
			glPopMatrix();
		} // for
		
		glPopMatrix();
	}

	// Draw background:
	{
		int i,j,k;
		glPushMatrix();
		glTranslatef(float(-(offsx/2)),float(-(offsy/2)),0);
		for(i=0;i<m_bg_dy;i++) {
			k=i*m_bg_dx;
			for(j=0;j<m_bg_dx;j++,k++) {
				if (m_bg[k]!=0) {
					m_bg[k]->draw(float(j*m_bg_cell_size),float(i*m_bg_cell_size+STARFIELD),0,0,1);
				} // if 
			} // for
		} // for
		glPopMatrix();
	}

	// Draw auxiliary back objects:
	{
		glPushMatrix();
		glTranslatef(float(-offsx),float(-offsy),0);
		TGLobject *o;
		m_auxiliary_back_objects.Rewind();
		while(m_auxiliary_back_objects.Iterate(o)) {
			o->draw(GLTM);
		} // while 
		glPopMatrix();
	}


	// Draw objects:
	{
		glPushMatrix();
		glTranslatef(float(-offsx),float(-offsy),0);
		TGLobject *o;
		m_fg_objects.Rewind();
		while(m_fg_objects.Iterate(o)) {
			o->draw(GLTM);
		} // while 
		glPopMatrix();
	}

	// Draw foreground:
	{
		int i,j,k;
		glPushMatrix();
		glTranslatef(float(-offsx),float(-offsy),0);
		for(i=0;i<m_fg_dy;i++) {
			k=i*m_fg_dx;
			for(j=0;j<m_fg_dx;j++,k++) {
				if (m_fg[k]!=0) {
					m_fg[k]->draw(float(j*m_fg_cell_size),float(i*m_fg_cell_size+STARFIELD),0,0,1);
				} // if 
			} // for
		} // for
		glPopMatrix();
	}

	// Draw auxiliary front objects:
	{
		glPushMatrix();
		glTranslatef(float(-offsx),float(-offsy),0);
		TGLobject *o;
		m_auxiliary_front_objects.Rewind();
		while(m_auxiliary_front_objects.Iterate(o)) {
			o->draw(GLTM);
		} // while 
		glPopMatrix();
	}

	// Draw the collision surface:
//	if (last_collision!=0) {
//		last_collision->draw(0,0,0,0,1);
//	} // if 
	
} /* TGLmap::draw */ 



void TGLmap::draw(int focus_x,int focus_y,int dx,int dy,GLTManager *GLTM,float zoom)
{
	int offsx,offsy;

	offsx=int((focus_x)-(dx/(2*zoom)));
	offsy=int((focus_y)-(dy/(2*zoom)));

	glPushMatrix();
	glScalef(zoom,zoom,zoom);

	// Draw starfield:
	{
		int i;
		glPushMatrix();
		glTranslatef(float(-(offsx/4)),float(-(offsy/4)),0);

		glBegin(GL_POINTS);
		for(i=0;i<m_nstars;i++) {
			glColor3f(m_star_color[i],m_star_color[i],m_star_color[i]);
			glVertex3f(float(m_star_x[i]),float(m_star_y[i]),0);
		} // for
		glEnd();


		for(i=0;i<m_nstars;i++) {
			glPushMatrix();
			glTranslatef(float(m_star_x[i]),float(m_star_y[i]),0);
			draw_glow(8,m_star_color[i]*6,m_star_color[i],m_star_color[i],m_star_color[i],0.125f);
			glPopMatrix();
		} // for
		
		glPopMatrix();
	}

	// Draw background:
	{
		int i,j,k;
		glPushMatrix();
		glTranslatef(float(-(offsx/2)),float(-(offsy/2)),0);
		for(i=0;i<m_bg_dy;i++) {
			k=i*m_bg_dx;
			for(j=0;j<m_bg_dx;j++,k++) {
				if (m_bg[k]!=0) {
					m_bg[k]->draw(float(j*m_bg_cell_size),float(i*m_bg_cell_size+STARFIELD),0,0,1);
				} // if 
			} // for
		} // for
		glPopMatrix();
	}

	// Draw auxiliary back objects:
	{
		glPushMatrix();
		glTranslatef(float(-offsx),float(-offsy),0);
		TGLobject *o;
		m_auxiliary_back_objects.Rewind();
		while(m_auxiliary_back_objects.Iterate(o)) {
			o->draw(GLTM);
		} // while 
		glPopMatrix();
	}


	// Draw objects:
	{
		glPushMatrix();
		glTranslatef(float(-offsx),float(-offsy),0);
		TGLobject *o;
		m_fg_objects.Rewind();
		while(m_fg_objects.Iterate(o)) {
			o->draw(GLTM);
		} // while 
		glPopMatrix();
	}

	// Draw foreground:
	{
		int i,j,k;
		glPushMatrix();
		glTranslatef(float(-offsx),float(-offsy),0);
		for(i=0;i<m_fg_dy;i++) {
			k=i*m_fg_dx;
			for(j=0;j<m_fg_dx;j++,k++) {
				if (m_fg[k]!=0) {
					m_fg[k]->draw(float(j*m_fg_cell_size),float(i*m_fg_cell_size+STARFIELD),0,0,1);
				} // if 
			} // for
		} // for
		glPopMatrix();
	}

	// Draw auxiliary front objects:
	{
		glPushMatrix();
		glTranslatef(float(-offsx),float(-offsy),0);
		TGLobject *o;
		m_auxiliary_front_objects.Rewind();
		while(m_auxiliary_front_objects.Iterate(o)) {
			o->draw(GLTM);
		} // while 
		glPopMatrix();
	}


	// Draw the bounding box of the editable space: (ince this function is only used by the map editor)
	{
		glPushMatrix();
		glTranslatef(float(-offsx),float(-offsy)+STARFIELD,0);
		glBegin(GL_LINES);
		glColor4f(1,1,1,0.5f);
		glVertex3f(0,0,0);
		glVertex3f(float(get_dx()),0,0);
		glVertex3f(0,float(get_dy()-STARFIELD),0);
		glVertex3f(float(get_dx()),float(get_dy()-STARFIELD),0);
		glVertex3f(0,0,0);
		glVertex3f(0,float(get_dy()-STARFIELD),0);
		glVertex3f(float(get_dx()),0,0);
		glVertex3f(float(get_dx()),float(get_dy()-STARFIELD),0);
		glEnd();
		glPopMatrix();
	}


	// Draw the collision surface:
//	if (last_collision!=0) {
//		last_collision->draw(0,0,0,0,1);
//	} // if 

	glPopMatrix();
	
} /* TGLmap::draw */ 




void TGLmap::add_object(TGLobject *o)
{
	m_fg_objects.Add(o);
	if (o->is_a("TGLobject_ship")) m_fg_ships.Add(o);
	o->set_cycle(get_cycle());
} /* TGLmap::add_object */ 


void TGLmap::add_object_back(TGLobject *o)
{
	m_fg_objects.Insert(o);
	if (o->is_a("TGLobject_ship")) m_fg_ships.Add(o);
	o->set_cycle(get_cycle());
} /* TGLmap::add_object_back */ 


void TGLmap::remove_object(TGLobject *o)
{
	m_fg_objects.DeleteElement(o);
	if (o->is_a("TGLobject_ship")) m_fg_ships.DeleteElement(o);
} /* TGLmap::remove_object */ 


void TGLmap::remove_auxiliary_front_object(class TGLobject *o)
{
	m_auxiliary_front_objects.DeleteElement(o);
} /* TGLmap::remove_auxiliary_front_object */ 


void TGLmap::remove_auxiliary_back_object(class TGLobject *o)
{
	m_auxiliary_back_objects.DeleteElement(o);
} /* TGLmap::remove_auxiliary_back_object */ 


void TGLmap::add_auxiliary_front_object(TGLobject *o)
{
	m_auxiliary_front_objects.Add(o);
} /* TGLmap::add_auxiliary_front_object */ 


void TGLmap::insert_auxiliary_front_object(TGLobject *o)
{
	m_auxiliary_front_objects.Insert(o);
} /* TGLmap::add_auxiliary_front_object */ 


void TGLmap::add_auxiliary_back_object(TGLobject *o)
{
	m_auxiliary_back_objects.Add(o);
} /* TGLmap::add_auxiliary_back_object */ 


bool TGLmap::object_exists(TGLobject *o)
{
	return m_fg_objects.MemberRefP(o);
} /* TGLmap::object_exists */ 


List<TGLobject> *TGLmap::get_objects(char *type)
{
	List<TGLobject> *l=new List<TGLobject>,l2;
	TGLobject *o;

	l2.Instance(m_fg_objects);
	while(l2.Iterate(o)) {
		if (o->is_a(type)) l->Add(o);
	} // while 

	return l;
} /* TGLmap::get_objects */ 



TGLobject *TGLmap::object_exists(char *type)
{
	List<TGLobject> l;
	TGLobject *o;

	if (strcmp(type,"TGLobject_ship")==0) l.Instance(m_fg_ships);
									 else l.Instance(m_fg_objects);

	l.Rewind();
	while(l.Iterate(o)) {
		if (o->is_a(type)) return o;
	} // while
	return 0;
} /* TGLmap::object_exists */ 


TGLobject *TGLmap::object_exists(char *type,float x1,float y1,float x2,float y2)
{
	List<TGLobject> l;
	TGLobject *o;

	if (strcmp(type,"TGLobject_ship")==0) l.Instance(m_fg_ships);
									 else l.Instance(m_fg_objects);

	l.Rewind();
	while(l.Iterate(o)) {
		if (o->get_x()>=x1 &&
			o->get_y()>=y1 &&
			o->get_x()<x2 &&
			o->get_y()<y2 &&
			o->is_a(type)) return o;
	} // while
	return 0;
} /* TGLmap::object_exists */ 


bool TGLmap::collision(TGLobject *o,float offsx,float offsy,int offs_alpha)
{
	GLTile *mask;
	SDL_Surface *sfc=0;
	float x,y;
	int hx,hy,angle;
	float s1;

	mask=o->get_last_mask();
	if (mask==0) return false;
	sfc=mask->get_tile(0);
	x=o->get_x()+offsx;
	y=o->get_y()+offsy;
	hx=mask->get_hot_x();
	hy=mask->get_hot_y();
	angle=o->get_angle()+offs_alpha;
	s1=o->get_scale();

	// Collision against objects:
	{
		List<TGLobject> l;
		TGLobject *o2;

		l.Instance(m_fg_objects);
		l.Rewind();
		while(l.Iterate(o2)) {
			if (o2!=o && o->check_collision(o2) && o2->check_collision(o)) {
				GLTile *mask2;
				SDL_Surface *sfc2=0;
				float x2,y2;
				int hx2,hy2,angle2;
				float s2;

				mask2=o2->get_last_mask();
				if (mask2!=0) {
					sfc2=mask2->get_tile(0);
					x2=o2->get_x();
					y2=o2->get_y();
					hx2=mask2->get_hot_x();
					hy2=mask2->get_hot_y();
					angle2=o2->get_angle();
					s2=o2->get_scale();

					if (::collision(sfc,x,y,hx,hy,angle,s1,sfc2,x2,y2,hx2,hy2,angle2,s2)) return true;
				} // if 

			} // if 
		} // while 
	}

	// Collision with foreground:
	{
		int i,j,k;
		int start_x,start_y,end_x,end_y;
		
		// This assumes that the objects are small! (64x64 at most)
		start_x=int((x/m_fg_cell_size)-3);
		end_x=int((x/m_fg_cell_size)+3);
		start_y=int(((y-STARFIELD)/m_fg_cell_size)-3);
		end_y=int(((y-STARFIELD)/m_fg_cell_size)+3);

		if (start_x<0) start_x=0;
		if (start_y<0) start_y=0;
		if (end_x>m_fg_dx) end_x=m_fg_dx;
		if (end_y>m_fg_dy) end_y=m_fg_dy;

		for(i=start_y;i<end_y;i++) {
			k=i*m_fg_dx+start_x;
			for(j=start_x;j<end_x;j++,k++) {
				if (m_fg[k]!=0) {
					SDL_Surface *sfc2=m_fg[k]->get_tile(0);
					if (::collision(sfc,x,y,hx,hy,angle,s1,sfc2,float(j*m_fg_cell_size),float(i*m_fg_cell_size+STARFIELD),0,0,0,1)) return true;
				} // if 
			} // for
		} // for
	}

	return false;
} /* TGLmap::collision */ 


bool TGLmap::collision_vector(TGLobject *o,float *vx,float *vy)
{
	GLTile *mask;
	SDL_Surface *sfc=0;
	float x,y;
	int hx,hy,angle;
	float s1;

	mask=o->get_last_mask();
	if (mask==0) return false;
	sfc=mask->get_tile(0);
	x=o->get_x();
	y=o->get_y();
	hx=mask->get_hot_x();
	hy=mask->get_hot_y();
	angle=o->get_angle();
	s1=o->get_scale();

	// Collision against objects:
	{
		List<TGLobject> l;
		TGLobject *o2;

		l.Instance(m_fg_objects);
		l.Rewind();
		while(l.Iterate(o2)) {
			if (o2!=o && o->check_collision(o2) && o2->check_collision(o)) {
				GLTile *mask2;
				SDL_Surface *sfc2=0;
				float x2,y2;
				int hx2,hy2,angle2;
				float s2;

				mask2=o2->get_last_mask();
				if (mask2!=0) {
					sfc2=mask2->get_tile(0);
					x2=o2->get_x();
					y2=o2->get_y();
					hx2=mask2->get_hot_x();
					hy2=mask2->get_hot_y();
					angle2=o2->get_angle();
					s2=o2->get_scale();

					if (::collision_vector(sfc,x,y,hx,hy,angle,s1,sfc2,x2,y2,hx2,hy2,angle2,s2,vx,vy)) return true;
				} // if 

			} // if 
		} // while 
	}

	// Collision with foreground:
	{
		int i,j,k;
		int start_x,start_y,end_x,end_y;
		
		// This assumes that the objects are small! (64x64 at most)
		start_x=int((x/m_fg_cell_size)-3);
		end_x=int((x/m_fg_cell_size)+3);
		start_y=int(((y-STARFIELD)/m_fg_cell_size)-3);
		end_y=int(((y-STARFIELD)/m_fg_cell_size)+3);

		if (start_x<0) start_x=0;
		if (start_y<0) start_y=0;
		if (end_x>m_fg_dx) end_x=m_fg_dx;
		if (end_y>m_fg_dy) end_y=m_fg_dy;

		for(i=start_y;i<end_y;i++) {
			k=i*m_fg_dx+start_x;
			for(j=start_x;j<end_x;j++,k++) {
				if (m_fg[k]!=0) {
					SDL_Surface *sfc2=m_fg[k]->get_tile(0);
					if (::collision_vector(sfc,x,y,hx,hy,angle,s1,sfc2,float(j*m_fg_cell_size),float(i*m_fg_cell_size+STARFIELD),0,0,0,1,vx,vy)) return true;
				} // if 
			} // for
		} // for
	}

	return false;
} /* TGLmap::collision_vector */ 



TGLobject *TGLmap::collision_with_object(TGLobject *o)
{
	GLTile *mask;
	SDL_Surface *sfc=0;
	float x,y;
	int hx,hy,angle;
	float s1;

	mask=o->get_last_mask();
	if (mask==0) return false;
	sfc=mask->get_tile(0);
	x=o->get_x();
	y=o->get_y();
	hx=mask->get_hot_x();
	hy=mask->get_hot_y();
	angle=o->get_angle();
	s1=o->get_scale();

	// Collision against objects:
	{
		List<TGLobject> l;
		TGLobject *o2;

		l.Instance(m_fg_objects);
		l.Rewind();
		while(l.Iterate(o2)) {
			if (o2!=o && o->check_collision(o2) && o2->check_collision(o)) {
				GLTile *mask2;
				SDL_Surface *sfc2=0;
				float x2,y2;
				int hx2,hy2,angle2;
				float s2;

				mask2=o2->get_last_mask();
				if (mask2!=0) {
					sfc2=mask2->get_tile(0);
					x2=o2->get_x();
					y2=o2->get_y();
					hx2=mask2->get_hot_x();
					hy2=mask2->get_hot_y();
					angle2=o2->get_angle();
					s2=o2->get_scale();

					if (::collision(sfc,x,y,hx,hy,angle,s1,sfc2,x2,y2,hx2,hy2,angle2,s2)) return o2;
				} // if 

			} // if 
		} // while 
	}

	return 0;
} /* TGLmap::collision_with_object */ 


TGLobject *TGLmap::collision_with_object(float x,float y)
{
	List<TGLobject> l;
	TGLobject *o2;

	l.Instance(m_fg_objects);
	l.Rewind();
	while(l.Iterate(o2)) {
		GLTile *mask2;
		SDL_Surface *sfc2=0;
		float x2,y2;
		int hx2,hy2,angle2;
		float s2;

		mask2=o2->get_last_mask();
		if (mask2!=0) {
			sfc2=mask2->get_tile(0);
			x2=o2->get_x();
			y2=o2->get_y();
			hx2=mask2->get_hot_x();
			hy2=mask2->get_hot_y();
			angle2=o2->get_angle();
			s2=o2->get_scale();

			if (::collision_with_point(sfc2,x2,y2,hx2,hy2,angle2,s2,x,y)) return o2;
		} // if 

	} // while 

	return 0;
} /* TGLmap::collision_with_object */ 


bool TGLmap::collision_with_foreground(TGLobject *o,float offsx,float offsy,int offs_alpha)
{
	GLTile *mask;
	SDL_Surface *sfc=0;
	float x,y;
	int hx,hy,angle;
	float s1;
	bool transformation_done=false;

	mask=o->get_last_mask();
	if (mask==0) return false;
	sfc=mask->get_tile(0);
	x=o->get_x()+offsx;
	y=o->get_y()+offsy;
	hx=mask->get_hot_x();
	hy=mask->get_hot_y();
	angle=o->get_angle()+offs_alpha;
	s1=o->get_scale();

	// Collision with foreground:
	{
		int i,j,k;
		int start_x,start_y,end_x,end_y;
		
		// This assumes that the objects are small! (64x64 at most)
		start_x=int((x/m_fg_cell_size)-3);
		end_x=int((x/m_fg_cell_size)+3);
		start_y=int(((y-STARFIELD)/m_fg_cell_size)-3);
		end_y=int(((y-STARFIELD)/m_fg_cell_size)+3);

		if (start_x<0) start_x=0;
		if (start_y<0) start_y=0;
		if (end_x>m_fg_dx) end_x=m_fg_dx;
		if (end_y>m_fg_dy) end_y=m_fg_dy;

		for(i=start_y;i<end_y;i++) {
			k=i*m_fg_dx+start_x;
			for(j=start_x;j<end_x;j++,k++) {
				if (m_fg[k]!=0) {

					if ((angle!=0 || s1!=1) && !transformation_done) {
						// Transform the object only once for not repeating the transformation many times in the collision routine:
						SDL_Surface *obj1_sfc;
						int minx=0,maxx=0;
						int miny=0,maxy=0;
						int new_hot_x1,new_hot_y1;
						SDL_Rect r1;

						// Draw the first object:
						if (angle==0 && s1==1) obj1_sfc=sfc;
										  else obj1_sfc=rotozoomSurface(sfc,-angle,s1,0);
						{
							float rad_ang1=float((float(-angle)*M_PI))/180;
							float tmpx1=hx-float(sfc->w)/2;
							float tmpy1=hy-float(sfc->h)/2;
							float cx1=float(obj1_sfc->w)/2;
							float cy1=float(obj1_sfc->h)/2;
							new_hot_x1=int(cx1+((cos(rad_ang1)*tmpx1+sin(rad_ang1)*tmpy1))*s1);
							new_hot_y1=int(cy1+((-sin(rad_ang1)*tmpx1+cos(rad_ang1)*tmpy1))*s1);

							r1.x=int(x-new_hot_x1);
							r1.y=int(y-new_hot_y1);
							r1.w=obj1_sfc->w;
							r1.h=obj1_sfc->h;

							sfc=obj1_sfc;
							x=r1.x;
							y=r1.y;
							hx=0;
							hy=0;
							angle=0;
							s1=1;

							transformation_done=true;
						}
					} // if 

					SDL_Surface *sfc2=m_fg[k]->get_tile(0);
					if (::collision(sfc,x,y,hx,hy,angle,s1,sfc2,float(j*m_fg_cell_size),float(i*m_fg_cell_size+STARFIELD),0,0,0,1)) {
						if (transformation_done) SDL_FreeSurface(sfc);
						return true;
					} // if 
				} // if 
			} // for
		} // for
	}

	if (transformation_done) SDL_FreeSurface(sfc);
	return false;
} /* TGLmap::collision_with_foreground */ 


void TGLmap::action(int action)
{
	List<TGLobject> l;
	TGLobject *o;

	l.Instance(m_fg_objects);
	l.Rewind();
	while(l.Iterate(o)) {
		if (o->is_a("TGLobject_leftdoor")) {
			((TGLobject_leftdoor *)o)->action(action);
		} // if 
		if (o->is_a("TGLobject_rightdoor")) {
			((TGLobject_rightdoor *)o)->action(action);
		} // if 
	} // while 

} /* TGLmap::action */ 


void TGLmap::draw_glow(int triangles,float radius,float r,float g,float b,float a)
{
	int i;
	float x1,y1,x2,y2,angle,inc;

	if (triangles<3) triangles=3;
	if (triangles>1024) triangles=1024;
	angle=0;
	inc=float((M_PI*2)/triangles);

	angle=0;
	x2=float(cos(angle)*radius);
	y2=float(sin(angle)*radius);
	angle+=inc;
	for(i=0;i<triangles;i++,angle+=inc) {
		x1=x2;
		y1=y2;
		x2=float(cos(angle)*radius);
		y2=float(sin(angle)*radius);
		glBegin(GL_TRIANGLES);
		glColor4f(r,g,b,a);
		glVertex3f(0,0,0);
		glColor4f(r,g,b,0);
		glVertex3f(x1,y1,0);
		glVertex3f(x2,y2,0);
		glEnd();
	} // for

} /* TGLmap::draw_glow */ 


void TGLmap::resize(int dx,int dy,GLTManager *GLTM)
{
	int i;
	int x,y;
	GLTile **m_fg_tmp;

	m_fg_tmp=new GLTile *[dx*dy];
	for(i=0;i<dx*dy;i++) {
		x = i%dx;
		y = i/dx;
		if ( x<m_fg_dx && y<m_fg_dy ) {
			m_fg_tmp[i]=m_fg[x+y*m_fg_dx];
			m_fg[x+y*m_fg_dx]=0;
		} else {
			m_fg_tmp[i]=0;
		} // if 
	} // for 

	delete []m_fg;
	m_fg = m_fg_tmp;
	m_fg_dx = dx;
	m_fg_dy = dy;


	List<TGLobject> to_delete;
	TGLobject *o;
	m_fg_objects.Rewind();
	while(m_fg_objects.Iterate(o)) {
		if (o->get_x()>m_fg_dx*m_fg_cell_size ||
			o->get_y()>m_fg_dy*m_fg_cell_size) {
			to_delete.Add(o);
		} // if
	} // while 

	while(!to_delete.EmptyP()) {
		o=to_delete.ExtractIni();
		remove_object(o);
		delete o;
	} // while 

	set_background(m_bg_code,GLTM);
} /* TGLmap::resize */ 


void TGLmap::save(FILE *fp,GLTManager *GLTM)
{
	int i;

	fprintf(fp,"%i %i\n",m_fg_dx,m_fg_dy);
	switch(m_bg_code) {
	case 1: fprintf(fp,"techno-red\n");	break;
	case 2: fprintf(fp,"techno-blue\n");	break;
	case 3: fprintf(fp,"techno-green\n");	break;
	case 4: fprintf(fp,"snow\n");	break;
	default: fprintf(fp,"rock\n");	break;
	} // switch
	
	for(i=0;i<m_fg_dx*m_fg_dy;i++) {
		if (m_fg[i]==0) {
			fprintf(fp,"-\n");
		} else {
			fprintf(fp,"%s\n",GLTM->get_name(m_fg[i]));
		} // if 
	} // for 

	/*

		Infer the number of objects and save them...

	*/ 

	{
		int no=0;
		List<TGLobject> l;
		TGLobject *o;

		l.Instance(m_fg_objects);
		l.Rewind();
		while(l.Iterate(o)) {
			if (o->is_a("TGLobject_ballstand")) no++;
			if (o->is_a("TGLobject_redlight")) no++;
			if (o->is_a("TGLobject_radar")) no++;
			if (o->is_a("TGLobject_pipevscreen")) no++;
			if (o->is_a("TGLobject_pipehscreen")) no++;
			if (o->is_a("TGLobject_laser_left")) no++;
			if (o->is_a("TGLobject_laser_right")) no++;
			if (o->is_a("TGLobject_laser_up")) no++;
			if (o->is_a("TGLobject_laser_down")) no++;
			if (o->is_a("TGLobject_cannon_left")) no++;
			if (o->is_a("TGLobject_cannon_right")) no++;
			if (o->is_a("TGLobject_cannon_down")) no++;
			if (o->is_a("TGLobject_cannon_up")) no++;
			if (o->is_a("TGLobject_fastcannon_left")) no++;
			if (o->is_a("TGLobject_fastcannon_right")) no++;
			if (o->is_a("TGLobject_fastcannon_down")) no++;
			if (o->is_a("TGLobject_fastcannon_up")) no++;
			if (o->is_a("TGLobject_spike_left")) no++;
			if (o->is_a("TGLobject_spike_right")) no++;
			if (o->is_a("TGLobject_fuelrecharge")) no++;
			if (o->is_a("TGLobject_techno_computer")) no++;
			if (o->is_a("TGLobject_directionalcannon_left")) no++;
			if (o->is_a("TGLobject_directionalcannon_right")) no++;
			if (o->is_a("TGLobject_directionalcannon_up")) no++;
			if (o->is_a("TGLobject_directionalcannon_down")) no++;
			if (o->is_a("TGLobject_ha_directionalcannon_left")) no++;
			if (o->is_a("TGLobject_ha_directionalcannon_right")) no++;
			if (o->is_a("TGLobject_ha_directionalcannon_up")) no++;
			if (o->is_a("TGLobject_ha_directionalcannon_down")) no++;
			if (o->is_a("TGLobject_tank")) no++;
			if (o->is_a("TGLobject_big_tank")) no++;
			if (o->is_a("TGLobject_leftdoor")) no++;
			if (o->is_a("TGLobject_rightdoor")) no++;
			if (o->is_a("TGLobject_button")) no++;
		} // while 

		fprintf(fp,"%i\n",no);

		l.Instance(m_fg_objects);
		l.Rewind();
		while(l.Iterate(o)) {
			if (o->is_a("TGLobject_ballstand")) fprintf(fp,"ball-stand %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_redlight")) fprintf(fp,"red-light %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_radar")) fprintf(fp,"radar %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_pipevscreen")) fprintf(fp,"pipe-vertical-screen %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_pipehscreen")) fprintf(fp,"pipe-horizontal-screen %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_laser_left")) fprintf(fp,"laser-left %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_laser_right")) fprintf(fp,"laser-right %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_laser_up")) fprintf(fp,"laser-up %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_laser_down")) fprintf(fp,"laser-down %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_cannon_left") && ((TGLobject_cannon_left *)o)->get_type()==0) fprintf(fp,"cannon-rock-left %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_cannon_right") && ((TGLobject_cannon_left *)o)->get_type()==0) fprintf(fp,"cannon-rock-right %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_cannon_down") && ((TGLobject_cannon_left *)o)->get_type()==0) fprintf(fp,"cannon-rock-down %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_cannon_up") && ((TGLobject_cannon_left *)o)->get_type()==0) fprintf(fp,"cannon-rock-up %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_cannon_left") && ((TGLobject_cannon_left *)o)->get_type()==1) fprintf(fp,"cannon-techno-left %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_cannon_right") && ((TGLobject_cannon_left *)o)->get_type()==1) fprintf(fp,"cannon-techno-right %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_cannon_down") && ((TGLobject_cannon_left *)o)->get_type()==1) fprintf(fp,"cannon-techno-down %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_cannon_up") && ((TGLobject_cannon_left *)o)->get_type()==1) fprintf(fp,"cannon-techno-up %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_fastcannon_left")) fprintf(fp,"fastcannon-techno-left %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_fastcannon_right")) fprintf(fp,"fastcannon-techno-right %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_fastcannon_down")) fprintf(fp,"fastcannon-techno-down %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_fastcannon_up")) fprintf(fp,"fastcannon-techno-up %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_spike_left")) fprintf(fp,"spike-left %i %i\n",int(((TGLobject_spike_left *)o)->get_start_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_spike_right")) fprintf(fp,"spike-right %i %i\n",int(((TGLobject_spike_left *)o)->get_start_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_fuelrecharge")) fprintf(fp,"fuel-recharge %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_techno_computer")) fprintf(fp,"wall-techno-computer %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_directionalcannon_left")) fprintf(fp,"directional-cannon-left %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_directionalcannon_right")) fprintf(fp,"directional-cannon-right %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_directionalcannon_up")) fprintf(fp,"directional-cannon-up %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_directionalcannon_down")) fprintf(fp,"directional-cannon-down %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_ha_directionalcannon_left")) fprintf(fp,"ha-directional-cannon-left %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_ha_directionalcannon_right")) fprintf(fp,"ha-directional-cannon-right %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_ha_directionalcannon_up")) fprintf(fp,"ha-directional-cannon-up %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_ha_directionalcannon_down")) fprintf(fp,"ha-directional-cannon-down %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_tank") && ((TGLobject_tank *)o)->get_type()==0) fprintf(fp,"grey-tank %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_tank") && ((TGLobject_tank *)o)->get_type()==1) fprintf(fp,"red-tank %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_tank") && ((TGLobject_tank *)o)->get_type()==2) fprintf(fp,"green-tank %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_big_tank")) fprintf(fp,"big-tank %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size));
			if (o->is_a("TGLobject_leftdoor")) fprintf(fp,"door-left %i %i %i %i\n",int(((TGLobject_leftdoor *)o)->get_start_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size),int(((TGLobject_leftdoor *)o)->get_state()),int(((TGLobject_leftdoor *)o)->get_action()));
			if (o->is_a("TGLobject_rightdoor")) fprintf(fp,"door-right %i %i %i %i\n",int(((TGLobject_leftdoor *)o)->get_start_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size),int(((TGLobject_leftdoor *)o)->get_state()),int(((TGLobject_leftdoor *)o)->get_action()));
			if (o->is_a("TGLobject_button") && ((TGLobject_button *)o)->get_type()==0) fprintf(fp,"button-red-left %i %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size),int(((TGLobject_leftdoor *)o)->get_action()));
			if (o->is_a("TGLobject_button") && ((TGLobject_button *)o)->get_type()==1) fprintf(fp,"button-red-right %i %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size),int(((TGLobject_leftdoor *)o)->get_action()));
			if (o->is_a("TGLobject_button") && ((TGLobject_button *)o)->get_type()==2) fprintf(fp,"button-red-up %i %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size),int(((TGLobject_leftdoor *)o)->get_action()));
			if (o->is_a("TGLobject_button") && ((TGLobject_button *)o)->get_type()==3) fprintf(fp,"button-red-down %i %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size),int(((TGLobject_leftdoor *)o)->get_action()));
			if (o->is_a("TGLobject_button") && ((TGLobject_button *)o)->get_type()==4) fprintf(fp,"button-purple-left %i %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size),int(((TGLobject_leftdoor *)o)->get_action()));
			if (o->is_a("TGLobject_button") && ((TGLobject_button *)o)->get_type()==5) fprintf(fp,"button-purple-right %i %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size),int(((TGLobject_leftdoor *)o)->get_action()));
			if (o->is_a("TGLobject_button") && ((TGLobject_button *)o)->get_type()==6) fprintf(fp,"button-purple-up %i %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size),int(((TGLobject_leftdoor *)o)->get_action()));
			if (o->is_a("TGLobject_button") && ((TGLobject_button *)o)->get_type()==7) fprintf(fp,"button-purple-down %i %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size),int(((TGLobject_leftdoor *)o)->get_action()));
			if (o->is_a("TGLobject_button") && ((TGLobject_button *)o)->get_type()==8) fprintf(fp,"button-blue-left %i %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size),int(((TGLobject_leftdoor *)o)->get_action()));
			if (o->is_a("TGLobject_button") && ((TGLobject_button *)o)->get_type()==9) fprintf(fp,"button-blue-right %i %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size),int(((TGLobject_leftdoor *)o)->get_action()));
			if (o->is_a("TGLobject_button") && ((TGLobject_button *)o)->get_type()==10) fprintf(fp,"button-blue-up %i %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size),int(((TGLobject_leftdoor *)o)->get_action()));
			if (o->is_a("TGLobject_button") && ((TGLobject_button *)o)->get_type()==11) fprintf(fp,"button-blue-down %i %i %i\n",int(o->get_x()/m_fg_cell_size),int((o->get_y()-STARFIELD)/m_fg_cell_size),int(((TGLobject_leftdoor *)o)->get_action()));
		} // while 
	
	}

} /* TGLmap::save */ 


