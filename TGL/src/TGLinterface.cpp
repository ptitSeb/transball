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
#include "auxiliar.h"
#include "GLTile.h"

#include "TGLinterface.h"


List<TGLInterfaceElement> TGLinterface::m_elements;


TGLInterfaceElement::~TGLInterfaceElement() 
{
} /* TGLInterfaceElement::~TGLInterfaceElement */ 


bool TGLInterfaceElement::check_status(int mousex,int mousey,int button)
{
	return false;
} /* TGLInterfaceElement::check_status */ 


void TGLInterfaceElement::draw(void)
{
} /* TGLInterfaceElement::draw */ 



TGLText::TGLText(char *text,TTF_Font *font,float x,float y,bool centered)
{
	m_centered=centered;
	m_text=new char[strlen(text)+1];
	strcpy(m_text,text);
	m_font=font;
	m_x=x;
	m_y=y;
	m_enabled=true;
	m_tile=0;
} /* TGLText::TGLText */ 


TGLText::~TGLText()
{
	delete []m_text;
	m_text=0;

	if (m_tile!=0) delete m_tile;
	m_tile=0;
} /* TGLText::~TGLText */ 


void TGLText::draw(void)
{
	if (m_tile==0) {
		if (m_centered) m_tile=TGLinterface::tile_print_center(m_text,m_font);
				   else m_tile=TGLinterface::tile_print_left(m_text,m_font);
	} // if 

	if (m_enabled) m_tile->draw(m_x,m_y+TTF_FontHeight(m_font)/2,0,0,1);
			  else m_tile->draw(0.33f,0.33f,0.33f,1,m_x,m_y+TTF_FontHeight(m_font)/2,0,0,1);
} /* TGLText::draw */ 




void TGLinterface::add_element(TGLInterfaceElement *e)
{
	m_elements.Add(e);
} /* TGLinterface::add_element */ 


void TGLinterface::remove_element(TGLInterfaceElement *e)
{
	m_elements.DeleteElement(e);
} /* TGLinterface::remove_element */ 


void TGLinterface::substitute_element(TGLInterfaceElement *old,TGLInterfaceElement *e)
{
	int pos=m_elements.PositionRef(old);
	if (pos>=0) {
		m_elements.SetObj(pos,e);
	} // if 
} /* TGLinterface::substitute_element */ 


void TGLinterface::reset(void)
{
	m_elements.Delete();
} /* TGLinterface::reset */ 




TGLbutton::TGLbutton(char *text,TTF_Font *font,float x,float y,float dx,float dy,int ID)
{
	m_text=new char[strlen(text)+1];
	strcpy(m_text,text);
	m_font=font;
	m_x=x;
	m_y=y;
	m_dx=dx;
	m_dy=dy;
	m_ID=ID;
	m_status=0;
	m_enabled=true;
	m_tile=0;
} /* TGLbutton::TGLbutton */ 


TGLbutton::TGLbutton(GLTile *icon,float x,float y,float dx,float dy,int ID)
{
	m_text=0;
	m_font=0;
	m_x=x;
	m_y=y;
	m_dx=dx;
	m_dy=dy;
	m_ID=ID;
	m_status=0;
	m_enabled=true;
	m_tile=icon;
} /* TGLbutton::TGLbutton */ 


TGLbutton::~TGLbutton()
{
	// Only delete the tile if it has been created here:
	if (m_text!=0) {
		if (m_tile!=0) delete m_tile;
		m_tile=0;
	} // if 

	if (m_text!=0) delete []m_text;
	m_text=0;

} /* TGLbutton::~TGLbutton */ 



void TGLbutton::draw(void)
{
	switch(m_status) {
	case 1: glColor3f(0.25f,0.25f,0.25f);
			break;
	case 2: glColor3f(0.5f,0.5f,0.5f);
			break;
	default:glColor3f(0.125f,0.125f,0.125f);
	} // switch

	if (!m_enabled) glColor3f(0.075f,0.075f,0.075f);

	glBegin(GL_POLYGON);
	glVertex3f(m_x+3,m_y,0);
	glVertex3f(m_x+m_dx-3,m_y,0);
	glVertex3f(m_x+m_dx-1,m_y+1,0);
	glVertex3f(m_x+m_dx,m_y+3,0);
	glVertex3f(m_x+m_dx,m_y+m_dy-3,0);
	glVertex3f(m_x+m_dx-1,m_y+m_dy-1,0);
	glVertex3f(m_x+m_dx-3,m_y+m_dy,0);
	glVertex3f(m_x+3,m_y+m_dy,0);
	glVertex3f(m_x+1,m_y+m_dy-1,0);
	glVertex3f(m_x,m_y+m_dy-3,0);
	glVertex3f(m_x,m_y+3,0);
	glVertex3f(m_x+1,m_y+1,0);
	glEnd();

	m_x++;
	m_y++;
	m_dx-=2;
	m_dy-=2;
	switch(m_status) {
	case 1: glColor3f(0.5f,0.5f,0.5f);
			break;
	case 2: glColor3f(0.75f,0.75f,0.75f);
			break;
	default:glColor3f(0.25f,0.25f,0.25f);
	} // switch

	if (!m_enabled) glColor3f(0.125f,0.125f,0.125f);

	glBegin(GL_POLYGON);
	glVertex3f(m_x+3,m_y,0);
	glVertex3f(m_x+m_dx-3,m_y,0);
	glVertex3f(m_x+m_dx-1,m_y+1,0);
	glVertex3f(m_x+m_dx,m_y+3,0);
	glVertex3f(m_x+m_dx,m_y+m_dy-3,0);
	glVertex3f(m_x+m_dx-1,m_y+m_dy-1,0);
	glVertex3f(m_x+m_dx-3,m_y+m_dy,0);
	glVertex3f(m_x+3,m_y+m_dy,0);
	glVertex3f(m_x+1,m_y+m_dy-1,0);
	glVertex3f(m_x,m_y+m_dy-3,0);
	glVertex3f(m_x,m_y+3,0);
	glVertex3f(m_x+1,m_y+1,0);
	glEnd();
	m_x--;
	m_y--;
	m_dx+=2;
	m_dy+=2;

	if (m_tile==0) {
		m_tile=TGLinterface::tile_print_center(m_text,m_font);
	} // if 

	if (m_font!=0) {
		if (m_enabled) m_tile->draw(m_x+m_dx/2,m_y+m_dy/2+TTF_FontHeight(m_font)/2,0,0,1);
				  else m_tile->draw(0.33f,0.33f,0.33f,1,m_x+m_dx/2,m_y+m_dy/2+TTF_FontHeight(m_font)/2,0,0,1);
	} else {
		if (m_enabled) m_tile->draw(m_x+m_dx/2,m_y+m_dy/2,0,0,1);
				  else m_tile->draw(0.33f,0.33f,0.33f,1,m_x+m_dx/2,m_y+m_dy/2,0,0,1);
	} // if 
} /* TGLbutton::draw */ 



bool TGLbutton::check_status(int mousex,int mousey,int button)
{
	if (!m_enabled) return false;

	if (mousex>=m_x && mousex<m_x+m_dx &&
		mousey>=m_y && mousey<m_y+m_dy) {
		if (button==0) {
			m_status=1;
		} else {
			m_status=2;
			return true;
		} // if 
	} else {
		m_status=0;
	} // if 

	return false;
} /* TGLbutton::check_status */ 


TGLframe::TGLframe(float x,float y,float dx,float dy)
{
	m_x=x;
	m_y=y;
	m_dx=dx;
	m_dy=dy;
	m_ID=-1;
	m_enabled=true;
} /* TGLframe::TGLframe */ 


TGLframe::~TGLframe()
{
} /* TGLframe::~TGLframe */ 


void TGLframe::draw(void)
{
	int bar_height=6;
	float old_y;

	glColor4f(0.15f,0.15f,0.15f,0.5f);
	glBegin(GL_POLYGON);
	glVertex3f(m_x+2,m_y,0);
	glVertex3f(m_x+m_dx-2,m_y,0);
	glVertex3f(m_x+m_dx-2,m_y+m_dy,0);
	glVertex3f(m_x+2,m_y+m_dy,0);
	glEnd();
	
	glColor3f(0.5f,0.5f,0.5f);
	glBegin(GL_POLYGON);
	glVertex3f(m_x+2,m_y,0);
	glVertex3f(m_x+m_dx-1,m_y,0);
	glVertex3f(m_x+m_dx,m_y+1,0);
	glVertex3f(m_x+m_dx,m_y+bar_height-1,0);
	glVertex3f(m_x+m_dx-1,m_y+bar_height,0);
	glVertex3f(m_x+2,m_y+bar_height,0);
	glVertex3f(m_x,m_y+bar_height-1,0);
	glVertex3f(m_x,m_y+1,0);
	glEnd();

	m_x++;
	m_dx-=2;
	glColor3f(1,1,1);
	glBegin(GL_POLYGON);
	glVertex3f(m_x+2,m_y,0);
	glVertex3f(m_x+m_dx-1,m_y,0);
	glVertex3f(m_x+m_dx,m_y+1,0);
	glVertex3f(m_x+m_dx,m_y+bar_height-1,0);
	glVertex3f(m_x+m_dx-1,m_y+bar_height,0);
	glVertex3f(m_x+2,m_y+bar_height,0);
	glVertex3f(m_x,m_y+bar_height-1,0);
	glVertex3f(m_x,m_y+1,0);
	glEnd();
	m_x--;
	m_dx+=2;

	old_y=m_y;
	m_y+=m_dy-bar_height;
	glColor3f(0.5f,0.5f,0.f);
	glBegin(GL_POLYGON);
	glVertex3f(m_x+2,m_y,0);
	glVertex3f(m_x+m_dx-1,m_y,0);
	glVertex3f(m_x+m_dx,m_y+1,0);
	glVertex3f(m_x+m_dx,m_y+bar_height-1,0);
	glVertex3f(m_x+m_dx-1,m_y+bar_height,0);
	glVertex3f(m_x+2,m_y+bar_height,0);
	glVertex3f(m_x,m_y+bar_height-1,0);
	glVertex3f(m_x,m_y+1,0);
	glEnd();

	m_x++;
	m_dx-=2;
	glColor3f(1,1,1);
	glBegin(GL_POLYGON);
	glVertex3f(m_x+2,m_y,0);
	glVertex3f(m_x+m_dx-1,m_y,0);
	glVertex3f(m_x+m_dx,m_y+1,0);
	glVertex3f(m_x+m_dx,m_y+bar_height-1,0);
	glVertex3f(m_x+m_dx-1,m_y+bar_height,0);
	glVertex3f(m_x+2,m_y+bar_height,0);
	glVertex3f(m_x,m_y+bar_height-1,0);
	glVertex3f(m_x,m_y+1,0);
	glEnd();
	m_x--;
	m_dx+=2;
	m_y=old_y;

} /* TGLframe::draw */ 


int TGLinterface::update_state(int mousex,int mousey,int button)
{
	int ret_val=-1;
	TGLInterfaceElement *e;

	m_elements.Rewind();
	while(m_elements.Iterate(e)) {
		if (e->check_status(mousex,mousey,button)) ret_val=e->m_ID;
	} // while 

	return ret_val;
} /* TGLinterface::update_state */ 


void TGLinterface::draw(void)
{
	TGLInterfaceElement *e;

	m_elements.Rewind();
	while(m_elements.Iterate(e)) {
		e->draw();
	} // while 

} /* TGLinterface::draw */ 


void TGLinterface::print_left(char *text,TTF_Font *font,float x,float y)
{
	GLTile *tile;
	SDL_Surface *sfc;
	SDL_Color c;
	c.r=255;
	c.g=255;
	c.b=255;

	sfc=TTF_RenderText_Blended(font,text,c);
	tile=new GLTile(sfc);
	tile->set_smooth();
	glNormal3f(0,0,1);
	tile->set_hotspot(0,tile->get_dy());
	tile->draw(x,y,0,0,1);
	delete tile;
} /* TGLinterface::print_left */ 


void TGLinterface::print_center(char *text,TTF_Font *font,float x,float y)
{
	GLTile *tile;
	SDL_Surface *sfc;
	SDL_Color c;
	c.r=255;
	c.g=255;
	c.b=255;

	sfc=TTF_RenderText_Blended(font,text,c);
	tile=new GLTile(sfc);
	tile->set_smooth();
	glNormal3f(0,0,1);
	tile->set_hotspot(tile->get_dx()/2,tile->get_dy());
	tile->draw(x,y,0,0,1);
	delete tile;
} /* TGLinterface::print_center */ 


void TGLinterface::print_left(char *text,TTF_Font *font,float x,float y,float r,float g,float b)
{
	GLTile *tile;
	SDL_Surface *sfc;
	SDL_Color c;
	c.r=(unsigned char)(r*255);
	c.g=(unsigned char)(g*255);
	c.b=(unsigned char)(b*255);

	sfc=TTF_RenderText_Blended(font,text,c);
	tile=new GLTile(sfc);
	tile->set_smooth();
	glNormal3f(0,0,1);
	tile->set_hotspot(0,tile->get_dy());
	tile->draw(x,y,0,0,1);
	delete tile;
} /* TGLinterface::print_left */ 


void TGLinterface::print_center(char *text,TTF_Font *font,float x,float y,float r,float g,float b)
{
	GLTile *tile;
	SDL_Surface *sfc;
	SDL_Color c;
	c.r=(unsigned char)(r*255);
	c.g=(unsigned char)(g*255);
	c.b=(unsigned char)(b*255);

	sfc=TTF_RenderText_Blended(font,text,c);
	tile=new GLTile(sfc);
	tile->set_smooth();
	glNormal3f(0,0,1);
	tile->set_hotspot(tile->get_dx()/2,tile->get_dy());
	tile->draw(x,y,0,0,1);
	delete tile;
} /* TGLinterface::print_center */ 


GLTile *TGLinterface::tile_print_left(char *text,TTF_Font *font)
{
	GLTile *tile;
	SDL_Surface *sfc;
	SDL_Color c;
	c.r=255;
	c.g=255;
	c.b=255;

	sfc=TTF_RenderText_Blended(font,text,c);
	tile=new GLTile(sfc);
	tile->set_smooth();
	tile->set_hotspot(0,tile->get_dy());

	return tile;
} /* TGLinterface::tile_print_left */ 


GLTile *TGLinterface::tile_print_center(char *text,TTF_Font *font)
{
	GLTile *tile;
	SDL_Surface *sfc;
	SDL_Color c;
	c.r=255;
	c.g=255;
	c.b=255;

	sfc=TTF_RenderText_Blended(font,text,c);
	tile=new GLTile(sfc);
	tile->set_smooth();
	tile->set_hotspot(tile->get_dx()/2,tile->get_dy());

	return tile;
} /* TGLinterface::tile_print_center */ 