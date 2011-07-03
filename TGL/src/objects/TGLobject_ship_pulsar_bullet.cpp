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

#include "gl.h"
#include "glu.h"
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
#include "sincos.h"

#include "GLTManager.h"
#include "SFXManager.h"
#include "TGLobject.h"
#include "TGLobject_enemy.h"
#include "TGLobject_ship_pulsar_bullet.h"
#include "TGLobject_FX_particle.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_ship_pulsar_bullet::TGLobject_ship_pulsar_bullet(float x,float y, float angle, TGLobject *ship) : TGLobject(x,y,0)
{
	m_angle=angle;
	m_radius=8;
	m_speed = 2;
	timmer = 200;
	exclude_for_collision(ship);
	
	n_points = 64;
	m_point_x = new double[n_points];
	m_point_y = new double[n_points];
	m_point_alpha = new double[n_points];
	m_point_width = new double[n_points];
	for(int i = 0;i<n_points;i++) m_point_alpha[i] = 1.0;
	
	SDL_Surface *pixel_sfc = SDL_CreateRGBSurface(SDL_SWSURFACE,1,1,32,RMASK,GMASK,BMASK,AMASK);
	putpixel(pixel_sfc,0,0,SDL_MapRGBA(pixel_sfc->format, 255,255,255,255));
	pixel_tile = new GLTile(pixel_sfc);
	pixel_object = new TGLobject(pixel_tile,0,0,0);
	pixel_object->exclude_for_collision(ship);

} /* TGLobject_ship_shield::TGLobject_ship_shield */ 


TGLobject_ship_pulsar_bullet::~TGLobject_ship_pulsar_bullet()
{
	delete pixel_tile;
	delete pixel_object;
	delete []m_point_x;
	delete []m_point_y;
	delete []m_point_alpha;
	delete []m_point_width;
} /* TGLobject_ship_pulsar_bullet::~TGLobject_ship_pulsar_bullet */


bool TGLobject_ship_pulsar_bullet::cycle(VirtualController *k,TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{
	m_cycle++;
	timmer--;
	m_radius+=0.5;
	
	// Move:
	if (m_state==0) {
		int a=m_angle-90;
		while(a<0) a+=360;
		while(a>=360) a-=360;
		m_x+=float((cos_table[a]*m_speed));
		m_y+=float((sin_table[a]*m_speed));
	} // if	
	
	// after a while, the ripple desintegrates:
	if (timmer==0) {
		// one point at random (use the "x" position as the random number generator to avoid calls to "rand()"):
		m_point_alpha[int(m_x)%n_points] *= 0.5;
	}
	
	// propagate disintegration:
	double max_alpha = 0;
	double previous_alpha = m_point_alpha[n_points-1], tmp;
	for(int i = 0;i<n_points;i++) {
		tmp = m_point_alpha[i];
		if (m_point_alpha[i]>max_alpha) max_alpha = m_point_alpha[i];
		if (m_point_alpha[i]<0.99 ||
			m_point_alpha[(i+1)%n_points]<0.99 ||
			previous_alpha<0.99) {
			m_point_alpha[i]*=0.5;
		}
		previous_alpha = tmp;
	}
	
	// collisions for the parts of the wave still alive:
	for(int i = 0;i<n_points;i++) {
		if (m_point_alpha[i]>0.99) {
			/*
			 bool TGLmap::pixel_collision(float x,float y)
			 {
			 SDL_Surface *pixel_sfc = SDL_CreateRGBSurface(SDL_SWSURFACE,1,1,32,RMASK,GMASK,BMASK,AMASK);
			 putpixel(pixel_sfc,0,0,SDL_MapRGBA(pixel_sfc->format, 255,255,255,255));
			 GLTile *tmp = new GLTile(pixel_sfc);
			 TGLobject *pixel = new TGLobject(tmp,x,y,0);
			 bool res = collision(pixel,0,0,0);
			 delete pixel;
			 delete tmp;
			 return res;
			 }
			 */
			
			if (map->collision(pixel_object,m_x+m_point_x[i],m_y+m_point_y[i],0)) {
				m_point_alpha[i]*=0.5;
				
				TGLobject *o=map->collision_with_object(m_x+m_point_x[i],m_y+m_point_y[i]);
				if (o!=0) {
					if (o->is_a("TGLobject_enemy") && this->check_collision(o)) {
						((TGLobject_enemy *)o)->hit(1);
					} // if
				} // if 	
			} // if 
		}
	}
	
	if (max_alpha<0.05) return false;
	
//	if (get_x()<0 || get_y()<0 || get_x()>map->get_dx() || get_y()>map->get_dy()) return false;
	
	return true;
} /* TGLobject_ship_pulsar_bullet::cycle */ 

void TGLobject_ship_pulsar_bullet::draw(GLTManager *GLTM)
{
	glPushMatrix();
	glTranslatef(m_x,m_y,0);
	if (m_angle!=0) glRotatef(m_angle,0,0,1);
	
	double d = 0, r = 0, perturb = 0;
	double a = 0;
	for(int i = 0;i<n_points;i++,a+=(M_PI*2/n_points)) {
		d = a * m_radius;
		perturb = m_radius*0.1;
		if (perturb>8) perturb = 10;
		r = m_radius + sin(d*0.075)*perturb;
		m_point_x[i] = r*cos(a);
		m_point_y[i] = r*sin(a)*0.1;
		m_point_width[i] = 8;
	}

	glBegin(GL_QUAD_STRIP);
	double wx,wy,vx,vy,vn;
	for(int i = 0;i<n_points;i++) {
		wx = m_point_x[(i+1)%n_points] - m_point_x[i];
		wy = m_point_y[(i+1)%n_points] - m_point_y[i];
		vx = wy;
		vy = -wx;
		vn = sqrt(vx*vx + vy*vy);
		vx = vx/vn;
		vy = vy/vn;
		glColor4f(0.0,0.0,1,0.5*m_point_alpha[i]);
		glVertex3f(m_point_x[i], m_point_y[i], 0);
		glColor4f(0.0,0.0,0.0,m_point_alpha[i]);
		glVertex3f(m_point_x[i]+vx*m_point_width[i], m_point_y[i]+vy*m_point_width[i], 0);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for(int i = 0;i<n_points;i++) {
		wx = m_point_x[(i+1)%n_points] - m_point_x[i];
		wy = m_point_y[(i+1)%n_points] - m_point_y[i];
		vx = wy;
		vy = -wx;
		vn = sqrt(vx*vx + vy*vy);
		vx = vx/vn;
		vy = vy/vn;
		glColor4f(0.0,0.0,1,0.5*m_point_alpha[i]);
		glVertex3f(m_point_x[i], m_point_y[i], 0);
		glColor4f(0.0,0.0,1,0.0);
		glVertex3f(m_point_x[i]-vx*m_point_width[i], m_point_y[i]-vy*m_point_width[i], 0);
	}
	glEnd();

	glBegin(GL_LINES);
	for(int i = 0;i<n_points;i++) {
		glColor4f(1,1,1,m_point_alpha[i]);
		glVertex3f(m_point_x[i], m_point_y[i], 0);
		glVertex3f(m_point_x[(i+1)%n_points], m_point_y[(i+1)%n_points], 0);
	}
	glEnd();
	 
	glPopMatrix();
} /* TGLobject_ship_pulsar_bullet::draw */ 


bool TGLobject_ship_pulsar_bullet::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_ship_pulsar_bullet")) return true;
	return TGLobject::is_a(c);
} /* TGLobject_ship_pulsar_bullet::is_a */ 


bool TGLobject_ship_pulsar_bullet::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_ship_pulsar_bullet::is_a */ 


const char *TGLobject_ship_pulsar_bullet::get_class(void)
{
	return "TGLobject_ship_pulsar_bullet";
} /* TGLobject_ship_pulsar_bullet:get_class */ 

