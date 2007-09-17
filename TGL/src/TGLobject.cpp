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
#include "TGLmap.h"
#include "collision.h"

#include "debug.h"




TGLobject::TGLobject(float x,float y)
{
	m_x=x;
	m_y=y;
	m_angle=0;
	m_scale=1;
	m_last_tile=0;
	m_last_mask=0;
	m_state=0;
	m_cycle=0;

	m_controller=0;

} /* TGLobject::TGLobject */ 


TGLobject::~TGLobject()
{
	m_excluded_from_collision.ExtractAll();
} /* TGLobject::~TGLobject */ 


void TGLobject::draw(GLTManager *GLTM)
{
} /* TGLobject::draw */ 


bool TGLobject::cycle(VirtualController *k,TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{
	m_cycle++;
	return true;
} /* TGLobject::cycle */ 


bool TGLobject::editor_cycle(TGLmap *map,GLTManager *GLTM)
{
	m_cycle++;
	return true;
} /* TGLobject::editor_cycle */ 


GLTile *TGLobject::get_last_tile(void)
{
	return m_last_tile;
} /* TGLobject::get_last_tile */ 


GLTile *TGLobject::get_last_mask(void)
{
	if (m_last_mask!=0) return m_last_mask;
	return m_last_tile;
} /* TGLobject::get_last_mask */ 


float TGLobject::get_x(void)
{
	return m_x;
} /* TGLobject::get_x */ 


float TGLobject::get_y(void)
{
	return m_y;
} /* TGLobject::get_y */ 


void TGLobject::set_x(float x)
{
	m_x=x;
} /* TGLobject::set_x */ 


void TGLobject::set_y(float y)
{
	m_y=y;
} /* TGLobject::set_y */ 


int TGLobject::get_angle(void)
{
	return m_angle;
} /* TGLobject::get_angle */ 


float TGLobject::get_scale(void)
{
	return m_scale;
} /* TGLobject::get_scale */ 


int TGLobject::get_state(void)
{
	return m_state;
} /* TGLobject::get_state */ 


bool TGLobject::is_a(Symbol *c)
{
	return c->cmp("TGLobject");
} /* TGLobject::is_a */ 


bool TGLobject::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject::is_a */ 


const char *TGLobject::get_class(void)
{
	return "TGLobject";
} /* TGLobject::get_class */ 


void TGLobject::exclude_for_collision(TGLobject *o)
{
	m_excluded_from_collision.Add(o);
} /* TGLobject::exclude_for_collision */ 


void TGLobject::reconsider_for_collision(TGLobject *o)
{
	m_excluded_from_collision.DeleteElement(o);
} /* TGLobject::reconsider_for_collision */ 


bool TGLobject::check_collision(TGLobject *o)
{
	return !(m_excluded_from_collision.MemberRefP(o));
} /* TGLobject::check_collision */ 


void TGLobject::set_controller(int c)
{
	m_controller=c;
} /* TGLobject::set_controller */ 


int TGLobject::get_controller(void)
{
	return m_controller;
} /* TGLobject::get_controller */ 



bool TGLobject::collision(TGLobject *o)
{
	GLTile *mask;
	SDL_Surface *sfc=0;
	float x,y;
	int hx,hy,angle;
	float s1;

	mask=get_last_mask();
	if (mask==0) return false;
	sfc=mask->get_tile(0);
	x=get_x();
	y=get_y();
	hx=mask->get_hot_x();
	hy=mask->get_hot_y();
	angle=get_angle();
	s1=get_scale();

	if (o!=this && o->check_collision(this) && this->check_collision(o)) {
		GLTile *mask2;
		SDL_Surface *sfc2=0;
		float x2,y2;
		int hx2,hy2,angle2;
		float s2;

		mask2=o->get_last_mask();
		if (mask2!=0) {
			sfc2=mask2->get_tile(0);
			x2=o->get_x();
			y2=o->get_y();
			hx2=mask2->get_hot_x();
			hy2=mask2->get_hot_y();
			angle2=o->get_angle();
			s2=o->get_scale();

			if (::collision(sfc,x,y,hx,hy,angle,s1,sfc2,x2,y2,hx2,hy2,angle2,s2)) return true;
		} // if 
	} // if 

	return false;
} /* TGLobject::collision */ 


