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

#include "GLTManager.h"
#include "SFXManager.h"
#include "TGLobject.h"
#include "TGLobject_bullet.h"
#include "TGLobject_bullet_missile.h"
#include "TGLobject_enemy.h"
#include "TGLobject_FX_particle.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_bullet_missile::TGLobject_bullet_missile(float x,float y,int angle,float speed,int power,GLTile *tile,GLTile *tile2,TGLobject *ship) : 
						  TGLobject_bullet(x,y,angle,speed,power,tile,ship)
{
	m_tile2=tile2;
} /* TGLobject_bullet_missile::TGLobject_bullet_missile */ 


bool TGLobject_bullet_missile::cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{
	if ((m_cycle%4)==0) map->add_auxiliary_back_object(new TGLobject_FX_particle(get_x(),get_y(),rand()%60,0,0,1,false,0.125f,0,0.25f,0.75f,50,GLTM->get("objects/smoke")));

	return TGLobject_bullet::cycle(k,map,GLTM,SFXM,sfx_volume);
} /* TGLobject_bullet_missile::cycle */ 


void TGLobject_bullet_missile::draw(GLTManager *GLTM)
{
	if (((m_cycle/4)%2)==0) m_last_tile=m_tile;
					   else m_last_tile=m_tile2;
	if (m_last_tile!=0) m_last_tile->draw(m_x,m_y,0,float(m_angle),1);
} /* TGLobject_bullet_missile::draw */ 


bool TGLobject_bullet_missile::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_bullet_missile")) return true;
	return TGLobject_bullet::is_a(c);
} /* TGLobject_bullet_missile::is_a */ 


bool TGLobject_bullet_missile::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_bullet_missile::is_a */ 


const char *TGLobject_bullet_missile::get_class(void)
{
	return "TGLobject_bullet_missile";
} /* TGLobject_bullet_missile::get_class */ 

