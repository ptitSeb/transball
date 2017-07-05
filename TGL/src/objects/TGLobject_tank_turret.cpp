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
#include "TGLobject_bullet.h"
#include "TGLobject_enemy.h"
#include "TGLobject_tank_turret.h"
#include "TGLobject_FX_explosion2.h"
#include "TGLmap.h"

#include "debug.h"




TGLobject_tank_turret::TGLobject_tank_turret(float x,float y,int ao,TGLobject_enemy *tank,int type) : TGLobject_enemy(x,y,ao)
{
	m_state=0;
	m_hitpoints=10;
	m_tank=tank;
	m_type=type;
} /* TGLobject_tank_turret::TGLobject_tank_turret */ 


TGLobject_tank_turret::~TGLobject_tank_turret()
{
} /* TGLobject_tank_turret::~TGLobject_tank_turret */ 


bool TGLobject_tank_turret::cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume)
{
	if (m_tank->get_hitpoints()<m_hitpoints) m_hitpoints=m_tank->get_hitpoints();
	if (m_tank->get_hitpoints()>m_hitpoints) m_tank->set_hitpoints(m_hitpoints);

	if (m_type==3) {
		m_x=(m_tank->get_x());
		m_y=(m_tank->get_y()-26);
	} else {
		m_x=(m_tank->get_x()+2);
		m_y=(m_tank->get_y()-17);
	} // if 

	m_angle=m_tank->get_angle()/2;

	if (m_hit) {
		Sound_play(SFXM->get("sfx/enemyhit"),sfx_volume);
		m_hit=false;
	} // if 

	if (m_hitpoints<=0) {
		Sound_play(SFXM->get("sfx/explosion"),sfx_volume);
		map->add_auxiliary_front_object(new TGLobject_FX_explosion2(get_x(),get_y(),128,200));
		return false;
	} // if 


	return true;
} /* TGLobject_tank_turret::cycle */ 


bool TGLobject_tank_turret::editor_cycle(TGLmap *map,GLTManager *GLTM)
{
	if (m_type==3) {
		m_x=(m_tank->get_x());
		m_y=(m_tank->get_y()-26);
	} else {
		m_x=(m_tank->get_x()+2);
		m_y=(m_tank->get_y()-17);
	} // if 

	m_angle=m_tank->get_angle()/2;

	return true;
} /* TGLobject_tank_turret::editor_cycle */


void TGLobject_tank_turret::draw(GLTManager *GLTM)
{
	if (m_last_tile==0) {
		if (m_type==0) m_last_tile=GLTM->get("objects/grey-tank-turret");
		if (m_type==1) m_last_tile=GLTM->get("objects/red-tank-turret");
		if (m_type==2) m_last_tile=GLTM->get("objects/green-tank-turret");
		if (m_type==3) m_last_tile=GLTM->get("objects/big-tank-turret");
	} // if 
	if (m_last_tile!=0) m_last_tile->draw(m_x,m_y,0,float(m_angle),1);
} /* TGLobject_tank_turret::draw */ 


bool TGLobject_tank_turret::is_a(Symbol *c)
{
	if (c->cmp("TGLobject_tank_turret")) return true;
	return TGLobject_enemy::is_a(c);
} /* TGLobject_tank_turret::is_a */ 


bool TGLobject_tank_turret::is_a(char *c)
{
	bool retval;
	Symbol *s=new Symbol(c);

	retval=is_a(s);

	delete s;

	return retval;
} /* TGLobject_tank_turret::is_a */ 


const char *TGLobject_tank_turret::get_class(void)
{
	return "TGLobject_tank_turret";
} /* TGLobject_tank_turret:get_class */ 


