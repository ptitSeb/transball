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
#include "sincos.h"
#include "Vector.h"

#include "GLTManager.h"
#include "SFXManager.h"
#include "TGLobject.h"
#include "TGLobject_bullet.h"
#include "TGLobject_bullet_hmissile.h"
#include "TGLobject_enemy.h"
#include "TGLobject_FX_particle.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_bullet_hmissile::TGLobject_bullet_hmissile(float x,float y,int ao,int angle,float speed,int power,GLTile *tile,GLTile *tile2,TGLobject *ship) : 
						  TGLobject_bullet(x,y,ao,angle,speed,power,tile,ship)
{
	m_tile2=tile2;
	m_target=0;
} /* TGLobject_bullet_hmissile::TGLobject_bullet_hmissile */ 


bool TGLobject_bullet_hmissile::cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{
	if ((m_cycle%4)==0) map->add_auxiliary_back_object(new TGLobject_FX_particle(get_x(),get_y(),rand()%60,0,0,1,false,0.125f,0,0.25f,0.75f,50,GLTM->get("objects/smoke")));

	{
		// Look for a new target:
		float d,min_d=0;
		m_target=0;
		List<TGLobject> *l=map->get_objects("TGLobject_enemy");
		TGLobject *o;

		l->Rewind();
		while(l->Iterate(o)) {
			d=float(sqrt((o->get_x()-get_x())*(o->get_x()-get_x())+(o->get_y()-get_y())*(o->get_y()-get_y())));
			if (m_target==0 || d<min_d) {
				m_target=o;
				min_d=d;
			} // if 
		} // while 
		
		l->ExtractAll();
		delete l;
	} // if 

	if (m_target!=0) {
		int i;
		bool first=true;
		int increment=0;
		float min_distance,d;
		GLTile *t;

		m_dest_x=m_target->get_x();
		m_dest_y=m_target->get_y();
		t=m_target->get_last_mask();
		if (t!=0) {
			float min_x,min_y,max_x,max_y;
			
			min_x=m_dest_x-t->get_hot_x();
			min_y=m_dest_y-t->get_hot_y();
			max_x=(m_dest_x-t->get_hot_x())+t->get_dx();
			max_y=(m_dest_y-t->get_hot_y())+t->get_dy();

			m_dest_x=(min_x+max_x)/2;
			m_dest_y=(min_y+max_y)/2;
		} // if 

		for(i=-2;i<3;i++) {
			int a=(m_angle+i)-90;
			while(a<0) a+=360;
			while(a>=360) a-=360;
			float next_x=float(m_x+(cos_table[a]*m_speed));
			float next_y=float(m_y+(sin_table[a]*m_speed));

			d=float(sqrt((next_x-m_dest_x)*(next_x-m_dest_x)+(next_y-m_dest_y)*(next_y-m_dest_y)));

			if (first || d<min_distance) {
				increment=i;
				min_distance=d;
			} // if 
			first=false;
		} // for

		m_angle+=increment;

	} // if 


	return TGLobject_bullet::cycle(k,map,GLTM,SFXM,sfx_volume);
} /* TGLobject_bullet_hmissile::cycle */ 


void TGLobject_bullet_hmissile::draw(GLTManager *GLTM)
{
	if (m_target!=0) {
		GLTile *dot=GLTM->get("objects/dot");
		float x=get_x(),y=get_y();
		float dx,dy;
		bool finished=false;
		float distance;
		do{
			dot->draw(x,y,0,0,0.5f);
			dx=m_dest_x-x;
			dy=m_dest_y-y;
			distance=float(sqrt(dx*dx+dy*dy));
			x+=((dx*16)/distance);
			y+=((dy*16)/distance);
		}while(distance>24);
	} // if 

	if (((m_cycle/4)%2)==0) m_last_tile=m_tile;
					   else m_last_tile=m_tile2;
	if (m_last_tile!=0) m_last_tile->draw(m_x,m_y,0,float(m_angle),1);

} /* TGLobject_bullet_hmissile::draw */ 


bool TGLobject_bullet_hmissile::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_bullet_hmissile")) return true;
	return TGLobject_bullet::is_a(c);
} /* TGLobject_bullet_hmissile::is_a */ 


bool TGLobject_bullet_hmissile::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_bullet_hmissile::is_a */ 


const char *TGLobject_bullet_hmissile::get_class(void)
{
	return "TGLobject_bullet_hmissile";
} /* TGLobject_bullet_hmissile::get_class */ 

