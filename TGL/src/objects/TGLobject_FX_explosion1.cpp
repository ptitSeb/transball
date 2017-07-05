#ifdef KITSCHY_DEBUG_MEMORY 
#include "debug_memorymanager.h"
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
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

#include "GLTManager.h"
#include "SFXManager.h"
#include "TGLobject.h"
#include "TGLobject_FX_explosion1.h"
#include "TGLobject_FX_particle.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_FX_explosion1::TGLobject_FX_explosion1(float x,float y,float maxsize,int duration) : TGLobject(x,y,0)
{
	m_maxsize=maxsize;
	m_duration=duration;
	m_state=0;
	m_cycle=0;
} /* TGLobject_FX_explosion1::TGLobject_FX_explosion1 */ 


TGLobject_FX_explosion1::~TGLobject_FX_explosion1()
{
} /* TGLobject_FX_explosion1::~TGLobject_FX_explosion1 */ 


bool TGLobject_FX_explosion1::cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{

	if (m_cycle==0) {
		int i;
		int range=100;
		range=int(m_maxsize*20);
		if (range<1) range=1;
		for(i=0;i<m_duration/4;i++) map->add_auxiliary_back_object(new TGLobject_FX_particle(get_x(),get_y(),rand()%60,float((rand()%(range*2+1))-range)/256.0f,float((rand()%(range*2+1))-range)/256.0f,4,true,0.5f,0.5f,0.5f,0.5f,1,1,1.0f,0,0.25f,0.25f,50,GLTM->get("objects/particle1")));
	} // if

	m_cycle++;

	if (m_cycle>m_duration) return false;
	return true;
} /* TGLobject_FX_explosion1::cycle */ 


void TGLobject_FX_explosion1::draw(GLTManager *GLTM)
{
	m_last_tile=0;

	{
		float t=m_cycle/float(m_duration);
		float radius;
		float r,g,b,a;
		int angle;

		radius=m_maxsize*(1-(1-t)*(1-t)*(1-t)*(1-t));
		if (t<0.25f) {
			r=(t/0.25f);
			g=(t/0.25f);
			b=(t/0.25f);
			a=(t/0.25f);
		} else {
			if (t<0.25f+0.375f) {
				float t1=(t-0.25f)/0.375f;
				r=1*(1-t1)+0.25f*(t1);
				g=1*(1-t1)+0.25f*(t1);
				b=1*(1-t1)+0.75f*(t1);
				a=1*(1-t1)+0.75f*(t1);
			} else {
				float t1=(t-(0.25f+0.375f))/0.375f;
				r=0.25f*(1-t1);
				g=0.25f*(1-t1);
				b=0.75f*(1-t1);
				a=0.75f*(1-t1);
			} // if 
		} // if 

		glColor4f(r,g,b,a);
		glBegin(GL_TRIANGLES);
	    for (angle=0;angle<360;angle+=30) {
		  glVertex2f(float(m_x),
					 float(m_y));
		  glVertex2f(float(m_x+(cos_degree(angle)*radius)),
					 float(m_y+(sin_degree(angle)*radius)));
		  glVertex2f(float(m_x+(cos_degree((angle+30)%360)*radius)),
					 float(m_y+(sin_degree((angle+30)%360)*radius)));
		} // for
		glEnd();

	}


} /* TGLobject_FX_explosion1::draw */ 


bool TGLobject_FX_explosion1::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_FX_explosion1")) return true;
	return TGLobject::is_a(c);
} /* TGLobject_FX_explosion1::is_a */ 


bool TGLobject_FX_explosion1::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_FX_explosion1::is_a */ 


const char *TGLobject_FX_explosion1::get_class(void)
{
	return "TGLobject_FX_explosion1";
} /* TGLobject_FX_explosion1::get_class */
 