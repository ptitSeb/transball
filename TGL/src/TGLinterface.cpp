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
#include "keyboardstate.h"

#include "TGLapp.h"

#include "TGLinterface.h"


List<TGLInterfaceElement> TGLinterface::m_elements;


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


int TGLinterface::update_state(int mousex,int mousey,int button,KEYBOARDSTATE *k)
{
	int ret_val=-1;
	TGLInterfaceElement *e;

	if (k->key_press(SDLK_TAB)) {
		TGLInterfaceElement *found=0;
		bool any_active=false;
		int pos=0;

		// Find if the mouse is currently over one of the active components:
		m_elements.Rewind();
		while(m_elements.Iterate(e) && found==0) {
			if (e->m_active) {
				if (mousex>=e->m_x && mousex<e->m_x+e->m_dx &&
					mousey>=e->m_y && mousey<e->m_y+e->m_dy) {
					found=e;
				} // if 
				any_active=true;
			} // if 
		} // while 

		if (any_active) {
			if (found==0) pos=-1;
					 else pos=m_elements.PositionRef(found);

			 do {
				 pos++;
				 if (pos>=m_elements.Length()) pos=0;
				 e=m_elements[pos];
			 } while(!e->m_active);

			 SDL_WarpMouse(int(e->m_x+e->m_dx/2),int(e->m_y+e->m_dy/2));
		} // if 
	} // if 

	m_elements.Rewind();
	while(m_elements.Iterate(e)) {
		if (e->check_status(mousex,mousey,button,k)) ret_val=e->m_ID;
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


/* ------------------------------------------------------------------------------------------------------ */ 



TGLInterfaceElement::~TGLInterfaceElement() 
{
	m_ID=-1;
	m_enabled=false;
	m_active=false;
	m_x=m_y=m_dx=m_dy=0;
} /* TGLInterfaceElement::~TGLInterfaceElement */ 


bool TGLInterfaceElement::check_status(int mousex,int mousey,int button,KEYBOARDSTATE *k)
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
	m_active=false;
} /* TGLText::TGLText */ 


TGLText::~TGLText()
{
	delete []m_text;
	m_text=0;

} /* TGLText::~TGLText */ 


void TGLText::draw(void)
{
	if (m_centered) {
		if (m_enabled) TGLinterface::print_center(m_text,m_font,m_x,m_y+TTF_FontHeight(m_font)/2);
				  else TGLinterface::print_center(m_text,m_font,m_x,m_y+TTF_FontHeight(m_font)/2,0.33f,0.33f,0.33f,1);
	} else {
		if (m_enabled) TGLinterface::print_left(m_text,m_font,m_x,m_y+TTF_FontHeight(m_font)/2);
				  else TGLinterface::print_left(m_text,m_font,m_x,m_y+TTF_FontHeight(m_font)/2,0.33f,0.33f,0.33f,1);
	} // if 

} /* TGLText::draw */ 



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
	m_active=true;

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
	m_active=true;
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
		if (m_enabled) TGLinterface::print_center(m_text,m_font,m_x+m_dx/2,m_y+m_dy/2+TTF_FontHeight(m_font)/2);
				  else TGLinterface::print_center(m_text,m_font,m_x+m_dx/2,m_y+m_dy/2+TTF_FontHeight(m_font)/2,0.33f,0.33f,0.33f,1);
	} else {
		if (m_enabled) m_tile->draw(m_x+m_dx/2,m_y+m_dy/2,0,0,1);
				  else m_tile->draw(0.33f,0.33f,0.33f,1,m_x+m_dx/2,m_y+m_dy/2,0,0,1);
	} // if 
} /* TGLbutton::draw */ 



bool TGLbutton::check_status(int mousex,int mousey,int button,KEYBOARDSTATE *k)
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
	m_active=false;
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


TGLTextInputFrame::TGLTextInputFrame(char *initial_text,int max_characters,TTF_Font *font,float x,float y,float dx,float dy,int ID) : TGLframe(x,y,dx,dy)
{
	if (initial_text==0) {
		m_editing=new char[max_characters+1];
		m_editing[0]=0;
		m_editing_position=0;
	} else {
		m_editing=new char[max_characters+1];
		strcpy(m_editing,initial_text);
		m_editing_position=strlen(m_editing);
	} // if 
	m_font=font;
	m_focus=false;
	m_cycle=0;
	m_max_characters=max_characters;
	m_ID=ID;
	m_active=true;
	m_change_in_last_cycle=false;
} /* TGLTextInputFrame::TGLTextInputFrame */ 


TGLTextInputFrame::~TGLTextInputFrame()
{
	if (m_editing!=0) delete []m_editing;
	m_editing=0;
} /* TGLTextInputFrame::~TGLTextInputFrame */ 

void TGLTextInputFrame::draw(void)
{
	TGLframe::draw();

	TGLinterface::print_left(m_editing,m_font,m_x+8,m_y+(m_dy/2)+TTF_FontHeight(m_font)/2);
	
	// draw the cursor:
	if (m_focus) {
		char tmp[255];
		int tdx=0,tdy=0;

		strcpy(tmp,m_editing);
		tmp[m_editing_position]=0;
		TTF_SizeText(m_font,tmp,&tdx,&tdy);

		{
			float f;
			f=float(0.5+0.3*sin((m_cycle)*0.3F));
			glColor4f(1,0,0,f);
			glBegin(GL_POLYGON);
			glVertex3f(float(m_x+8+tdx),348,0);
			glVertex3f(float(m_x+8+tdx+4),348,0);
			glVertex3f(float(m_x+8+tdx+4),364,0);
			glVertex3f(float(m_x+8+tdx),364,0);
			glEnd();
		}
	} // if

} /* TGLTextInputFrame::draw */ 


bool TGLTextInputFrame::check_status(int mousex,int mousey,int button,KEYBOARDSTATE *k)
{
	m_cycle++;

	m_change_in_last_cycle=false;

	if (m_enabled) {
		if (button!=0) {
			if (mousex>=m_x && mousex<m_x+m_dx &&
				mousey>=m_y && mousey<m_y+m_dy) m_focus=true;
										   else m_focus=false;
		} // if 

		if (m_focus) {
			unsigned int pep=m_editing_position,el=strlen(m_editing);

			TGLapp::string_editor_cycle(m_editing,&m_editing_position,m_max_characters,k);

			if (m_editing_position!=pep || strlen(m_editing)!=el) m_change_in_last_cycle=true;

			if (k->key_press(SDLK_RETURN)) return true;
		} // if 
	} // if

	return false;
} /* TGLTextInputFrame::check_status */ 

