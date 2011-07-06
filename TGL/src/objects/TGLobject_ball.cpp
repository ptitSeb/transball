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

#include "GLTManager.h"
#include "SFXManager.h"
#include "TGLobject.h"
#include "TGLobject_ball.h"
#include "TGLobject_button.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_ball::TGLobject_ball(float x,float y,int animation_offset) : TGLobject(x,y,animation_offset)
{
	m_attractor_timmer=0;
	m_speed_x=0;
	m_speed_y=0;
} /* TGLobject_ball::TGLobject_ball */ 


TGLobject_ball::~TGLobject_ball()
{
} /* TGLobject_ball::~TGLobject_ball */ 


bool TGLobject_ball::cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{
	switch(m_state) {
	case 0:	if (m_attractor_timmer>0) m_attractor_timmer--;		
			break;
	default:
			if (m_speed_x>0) m_speed_x-=1.0/256.0f;
			if (m_speed_x<0) m_speed_x+=1.0/256.0f;
			m_speed_y+=2.0f/256.0f;
			m_x+=m_speed_x;
			m_y+=m_speed_y;

			if (m_x<0) {
				m_x=0;
				m_speed_x=0;
			} /* if */ 
			if (m_y<0) {
				m_y=0;
				m_speed_y=0;
			} /* if */ 
			if (m_x>(map->get_dx())) {
				m_x=float(map->get_dx());
				m_speed_x=0;
			} /* if */ 
			if (m_y>(map->get_dy())) {
				m_y=float(map->get_dy());
				m_speed_y=0;
			} /* if */ 


			{
				float n;
				float coll_v_x=0,coll_v_y=0;

				if (map->collision_vector(this,&coll_v_x,&coll_v_y)) {
					
					// Check if a button is pressed:
					{
						TGLobject *o;
						o=map->collision_with_object(this);
						if (o!=0 && o->is_a("TGLobject_button")) {
							((TGLobject_button *)o)->ball_hit();
						} // if 
					}


					n=float(sqrt(coll_v_x*coll_v_x+coll_v_y*coll_v_y));

					if (n>0) {
						coll_v_x/=n;
						coll_v_y/=n;

						// 1) Move the ball one pixel in the direction opposite to the collision:
						m_x-=coll_v_x;
						m_y-=coll_v_y;

						// 2) Modify the speed of the ball with the collision:
						{
							float v_x1,v_y1;
							float v_x2,v_y2;
							float n1;

							n1=m_speed_x*coll_v_x+m_speed_y*coll_v_y;
							v_x1=coll_v_x*n1;
							v_y1=coll_v_y*n1;
							v_x2=m_speed_x-v_x1;
							v_y2=m_speed_y-v_y1;

							m_speed_x=(v_x2-(0.75f*v_x1));
							m_speed_y=(v_y2-(0.75f*v_y1));
						}
					} // if 

				} // if 

			}

			break;
	} // switch
	return true;
} /* TGLobject_ball::cycle */ 


void TGLobject_ball::draw(GLTManager *GLTM)
{
	switch(m_state) {
	case -1: m_last_tile=GLTM->get("objects/ball-grey");
			break;
	case 0: m_last_tile=GLTM->get("objects/ball-grey");
			break;
	case 1: m_last_tile=GLTM->get("objects/ball-blue");
			break;
	case 2: m_last_tile=GLTM->get("objects/ball-red");
			break;
	case 3: m_last_tile=GLTM->get("objects/ball-purple");
			break;
	case 4: m_last_tile=GLTM->get("objects/ball-green");
			break;
	case 5: m_last_tile=GLTM->get("objects/ball-yellow");
			break;
	case 6: m_last_tile=GLTM->get("objects/ball-darkblue");
			break;
	case 7: m_last_tile=GLTM->get("objects/ball-turquoise");
			break;
	} // switch

	if (m_last_tile!=0) m_last_tile->draw(m_x,m_y,0,0,1);
} /* TGLobject_ball::draw */ 


bool TGLobject_ball::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_ball")) return true;
	return TGLobject::is_a(c);
} /* TGLobject_ball::is_a */ 


bool TGLobject_ball::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_ball::is_a */ 


const char *TGLobject_ball::get_class(void)
{
	return "TGLobject_ball";
} /* TGLobject_ball::get_class */ 



bool TGLobject_ball::attractor(void)
{
	m_attractor_timmer+=2;
	if (m_attractor_timmer>=50) return true;
	return false;
} /* TGLobject_ball::attractor */ 


void TGLobject_ball::capture(int m_type)
{
	m_state=m_type;
} /* TGLobject_ball::capture */ 


float TGLobject_ball::get_speed_x(void)
{
	return m_speed_x;
} /* TGLobject_ball::get_speed_x */ 


float TGLobject_ball::get_speed_y(void)
{
	return m_speed_y;
} /* TGLobject_ball::get_speed_y */ 


void TGLobject_ball::set_speed_x(float speed_x)
{
	m_speed_x=speed_x;
} /* TGLobject_ball::set_speed_x */ 


void TGLobject_ball::set_speed_y(float speed_y)
{
	m_speed_y=speed_y;
} /* TGLobject_ball::set_speed_y */ 



