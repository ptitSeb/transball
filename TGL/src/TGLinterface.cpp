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


float color1_r=1,color1_g=1,color1_b=1;					// TEXT
float color2_r=0.15f,color2_g=0.15f,color2_b=0.15f;		// FRAME_BG
float color3_r=1,color3_g=1,color3_b=1;					// FRAME_FG

/*
float color4_r=0.25f,color4_g=0.25f,color4_b=0.25f;		// BUTTON STD
float color5_r=0.5f,color5_g=0.5f,color5_b=0.5f;		// BUTTON MOUSE
float color6_r=0.75f,color6_g=0.75f,color6_b=0.75f;		// BUTTON PRESSED
*/


float color4_r=0.16f,color4_g=0.16f,color4_b=0.33f;		// BUTTON STD
float color5_r=0.33f,color5_g=0.33f,color5_b=0.66f;		// BUTTON MOUSE
float color6_r=0.66f,color6_g=0.66f,color6_b=0.85f;		// BUTTON PRESSED



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


int TGLinterface::update_state(int mousex,int mousey,int button,int button_status,KEYBOARDSTATE *k)
{
	int ret_val=-1;
	TGLInterfaceElement *e,*modal=0;
	List<TGLInterfaceElement> to_delete;

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
	while(m_elements.Iterate(e) && modal==0) {		
		if (e->m_modal==true) modal=e;
	} // while 

	if (modal!=0) {
		if (modal->check_status(mousex,mousey,button,button_status,k)) ret_val=modal->m_ID;
		if (modal->m_to_be_deleted) to_delete.Add(e);
	} else {
		m_elements.Rewind();
		while(m_elements.Iterate(e)) {		
			if (e->check_status(mousex,mousey,button,button_status,k)) ret_val=e->m_ID;
			if (e->m_to_be_deleted) to_delete.Add(e);
		} // while 
	} // if

	while(!to_delete.EmptyP()) {
		e = to_delete.ExtractIni();
		m_elements.DeleteElement(e);
		delete e;
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


void TGLinterface::draw(float alpha)
{
	TGLInterfaceElement *e;

	m_elements.Rewind();
	while(m_elements.Iterate(e)) {
		e->draw(alpha);
	} // while 

} /* TGLinterface::draw */ 


void TGLinterface::enable(int ID)
{
	TGLInterfaceElement *e;

	m_elements.Rewind();
	while(m_elements.Iterate(e)) {
		if (e->m_ID==ID) e->m_enabled=true;
	} // while 
} /* TGLinterface::enable */  


void TGLinterface::disable(int ID)
{
	TGLInterfaceElement *e;

	m_elements.Rewind();
	while(m_elements.Iterate(e)) {
		if (e->m_ID==ID) e->m_enabled=false;
	} // while 
} /* TGLinterface::disable */ 


bool TGLinterface::is_enabled(int ID)
{
	TGLInterfaceElement *e;

	m_elements.Rewind();
	while(m_elements.Iterate(e)) {
		if (e->m_ID==ID) return e->m_enabled;
	} // while 

	return false;
} /* TGLinterface::is_enabled */ 


TGLInterfaceElement *TGLinterface::get(int ID)
{
	TGLInterfaceElement *e;

	m_elements.Rewind();
	while(m_elements.Iterate(e)) {
		if (e->m_ID==ID) return e;
	} // while 
	return 0;
} /* TGLinterface::get */ 


/* ------------------------------------------------------------------------------------------------------ */ 



TGLInterfaceElement::TGLInterfaceElement() 
{
	m_ID=-1;
	m_enabled=false;
	m_active=false;
	m_modal=false;
	m_to_be_deleted=false;
	m_x=m_y=m_dx=m_dy=0;
} /* TGLInterfaceElement::TGLInterfaceElement */ 



TGLInterfaceElement::~TGLInterfaceElement() 
{
} /* TGLInterfaceElement::~TGLInterfaceElement */ 


bool TGLInterfaceElement::check_status(int mousex,int mousey,int button,int button_status,KEYBOARDSTATE *k)
{
	return false;
} /* TGLInterfaceElement::check_status */ 


void TGLInterfaceElement::draw(void)
{
} /* TGLInterfaceElement::draw */ 


void TGLInterfaceElement::draw(float alpha)
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


TGLText::TGLText(char *text,TTF_Font *font,float x,float y,bool centered,int ID) : TGLInterfaceElement()
{
	m_ID = ID;
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
		if (m_enabled) TGLinterface::print_center(m_text,m_font,m_x,m_y+TTF_FontHeight(m_font)/2,color1_r,color1_g,color1_b,1);
				  else TGLinterface::print_center(m_text,m_font,m_x,m_y+TTF_FontHeight(m_font)/2,0.33f*color1_r,0.33f*color1_g,0.33f*color1_b,1);
	} else {
		if (m_enabled) TGLinterface::print_left(m_text,m_font,m_x,m_y+TTF_FontHeight(m_font)/2);
				  else TGLinterface::print_left(m_text,m_font,m_x,m_y+TTF_FontHeight(m_font)/2,0.33f,0.33f,0.33f,1);
	} // if 

} /* TGLText::draw */ 


void TGLText::draw(float alpha)
{
	if (m_centered) {
		if (m_enabled) TGLinterface::print_center(m_text,m_font,m_x,m_y+TTF_FontHeight(m_font)/2,color1_r,color1_g,color1_b,alpha);
				  else TGLinterface::print_center(m_text,m_font,m_x,m_y+TTF_FontHeight(m_font)/2,0.33f*color1_r,0.33f*color1_g,0.33f*color1_b,alpha);
	} else {
		if (m_enabled) TGLinterface::print_left(m_text,m_font,m_x,m_y+TTF_FontHeight(m_font)/2,color1_r,color1_g,color1_b,alpha);
				  else TGLinterface::print_left(m_text,m_font,m_x,m_y+TTF_FontHeight(m_font)/2,0.33f*color1_r,0.33f*color1_g,0.33f*color1_b,alpha);
	} // if 

} /* TGLText::draw */ 


void TGLText::set_text(char *text)
{
	if (m_text!=0) 	delete []m_text;
	m_text=new char[strlen(text)+1];
	strcpy(m_text,text);
} /* TGLText::set_text */ 


TGLbutton::TGLbutton(char *text,TTF_Font *font,float x,float y,float dx,float dy,int ID)  : TGLInterfaceElement()
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


TGLbutton::TGLbutton(GLTile *icon,float x,float y,float dx,float dy,int ID)  : TGLInterfaceElement()
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
	draw(1);
} /* TGLbutton::draw */ 


void TGLbutton::draw(float alpha)
{
	if (!m_enabled) alpha/=3;
	switch(m_status) {
	case 1: glColor4f(color5_r*0.5f,color6_g*0.5f,color5_b*0.5f,alpha);
			break;
	case 2: glColor4f(color6_r*0.5f,color5_g*0.5f,color6_b*0.5f,alpha);
			break;
	default:glColor4f(color4_r*0.5f,color4_g*0.5f,color4_b*0.5f,alpha);
	} // switch

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
	case 1: glColor4f(color5_r,color5_g,color5_b,alpha);
			break;
	case 2: glColor4f(color6_r,color6_g,color6_b,alpha);
			break;
	default:glColor4f(color4_r,color4_g,color4_b,alpha);
	} // switch

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
		TGLinterface::print_center(m_text,m_font,m_x+m_dx/2,m_y+m_dy/2+TTF_FontHeight(m_font)/2,color1_r,color1_g,color1_b,alpha);
//		if (m_enabled) TGLinterface::print_center(m_text,m_font,m_x+m_dx/2,m_y+m_dy/2+TTF_FontHeight(m_font)/2,color1_r,color1_g,color1_b,alpha);
//				  else TGLinterface::print_center(m_text,m_font,m_x+m_dx/2,m_y+m_dy/2+TTF_FontHeight(m_font)/2,0.33f*color1_r,0.33f*color1_g,0.33f*color1_b,alpha);
	} else {
		m_tile->draw(color1_r,color1_g,color1_b,alpha,m_x+m_dx/2,m_y+m_dy/2,0,0,1);
//		if (m_enabled) m_tile->draw(color1_r,color1_g,color1_b,alpha,m_x+m_dx/2,m_y+m_dy/2,0,0,1);
//				  else m_tile->draw(0.33f*color1_r,0.33f*color1_g,0.33f*color1_b,alpha,m_x+m_dx/2,m_y+m_dy/2,0,0,1);
	} // if 
} /* TGLbutton::draw */ 



bool TGLbutton::check_status(int mousex,int mousey,int button,int button_status,KEYBOARDSTATE *k)
{
	if (!m_enabled) {
		m_status=0;
		return false;
	} // if

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



TGLbuttonTransparent::TGLbuttonTransparent(char *text,TTF_Font *font,float x,float y,float dx,float dy,int ID) : TGLbutton(text,font,x,y,dx,dy,ID)
{
} /* TGLbuttonTransparent::TGLbuttonTransparent */ 



void TGLbuttonTransparent::draw(float alpha)
{
	if (!m_enabled) alpha/=3;
	switch(m_status) {
	case 1: TGLinterface::print_centered(m_text,m_font,m_x+m_dx/2,m_y+m_dy/2,0,0,0,alpha*0.5f,0,1.25f);
			TGLinterface::print_centered(m_text,m_font,m_x+m_dx/2,m_y+m_dy/2,1,1,1,alpha*0.5f,0,1.1f);
			TGLinterface::print_centered(m_text,m_font,m_x+m_dx/2,m_y+m_dy/2,1,1,1,alpha,0,1.05f);
			TGLinterface::print_centered(m_text,m_font,m_x+m_dx/2,m_y+m_dy/2,0.75f,0.75f,0.75f,alpha,0,1);
			break;				
	case 2: TGLinterface::print_centered(m_text,m_font,m_x+m_dx/2,m_y+m_dy/2,0,0,0,alpha*0.5f,0,1.25f);
			TGLinterface::print_centered(m_text,m_font,m_x+m_dx/2,m_y+m_dy/2,1,1,1,alpha*0.5f,0,1.1f);
			TGLinterface::print_centered(m_text,m_font,m_x+m_dx/2,m_y+m_dy/2,1,1,1,alpha,0,1.05f);
			TGLinterface::print_centered(m_text,m_font,m_x+m_dx/2,m_y+m_dy/2,1,1,1,alpha,0,1);
			break;
	default:TGLinterface::print_centered(m_text,m_font,m_x+m_dx/2,m_y+m_dy/2,0,0,0,alpha*0.5f,0,1.15f);
			TGLinterface::print_centered(m_text,m_font,m_x+m_dx/2,m_y+m_dy/2,0.75f,0.75f,0.75f,alpha,0,1);

	} // switch

} /* TGLbuttonTransparent::draw */ 



TGLframe::TGLframe(float x,float y,float dx,float dy) : TGLInterfaceElement()
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
	if (!m_enabled) draw(0.5f);
			   else draw(1);
} /* TGLframe::draw */ 


void TGLframe::draw(float alpha)
{
	int bar_height=6;
	float old_y;

	glColor4f(color2_r,color2_g,color2_b,0.5f*alpha);
	glBegin(GL_POLYGON);
	glVertex3f(m_x+2,m_y,0);
	glVertex3f(m_x+m_dx-2,m_y,0);
	glVertex3f(m_x+m_dx-2,m_y+m_dy,0);
	glVertex3f(m_x+2,m_y+m_dy,0);
	glEnd();
	
	glColor4f(color3_r*0.5f,color3_g*0.5f,color3_b*0.5f,alpha);
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
	glColor4f(color3_r,color3_g,color3_b,alpha);
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
	glColor4f(color3_r*0.5f,color3_g*0.5f,color3_b*0.5f,alpha);
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
	glColor4f(color3_r,color3_g,color3_b,alpha);
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


TGLslider::TGLslider(float x,float y,float dx,float dy,float slider_dx,float slider_dy,int ID) : TGLInterfaceElement()
{
	m_x=x;
	m_y=y;
	m_dx=dx;
	m_dy=dy;
	m_slider_dx=slider_dx;
	m_slider_dy=slider_dy;
	m_ID=ID;
	m_value=0;
	m_active=true;
	m_enabled=true;

} /* TGLslider::TGLslider */ 


TGLslider::~TGLslider()
{
} /* TGLslider::~TGLslider */ 


bool TGLslider::check_status(int mousex,int mousey,int button,int button_status,KEYBOARDSTATE *k)
{
	if (!m_enabled) return false;

	if (button_status!=0) {
		if (mousex>=m_x && mousex<=m_x+m_dx &&
			mousey>=m_y && mousey<=m_y+m_dy) {
			m_value = float(mousex-m_x)/float(m_dx);
			return true;
		} // if 
	} // if 

	return false;
} /* TGLslider::check_status */ 


void TGLslider::draw(float alpha)
{
	float x=m_x;
	float y=m_y+(m_dy/2)-2;
	float dx=m_dx;
	float dy=4;

	glColor4f(color4_r*0.8f,color4_g*0.8f,color4_b*0.8f,alpha);
	if (!m_enabled) glColor4f(color4_r*0.5f,color4_g*0.5f,color4_b*0.5f,alpha);
	glBegin(GL_POLYGON);
	glVertex3f(x+3,y,0);
	glVertex3f(x+dx-3,y,0);
	glVertex3f(x+dx-1,y+1,0);
	glVertex3f(x+dx,y+3,0);
	glVertex3f(x+dx,y+dy-3,0);
	glVertex3f(x+dx-1,y+dy-1,0);
	glVertex3f(x+dx-3,y+dy,0);
	glVertex3f(x+3,y+dy,0);
	glVertex3f(x+1,y+dy-1,0);
	glVertex3f(x,y+dy-3,0);
	glVertex3f(x,y+3,0);
	glVertex3f(x+1,y+1,0);
	glEnd();

	x=m_x+(m_value*m_dx)-m_slider_dx/2;
	y=m_y+(m_dy/2)-m_slider_dy/2;
	dx=m_slider_dx;
	dy=m_slider_dy;

	glColor4f(color4_r*0.5f,color4_g*0.5f,color4_b*0.5f,alpha);
	if (!m_enabled) glColor4f(color4_r*0.25f,color4_g*0.25f,color4_b*0.25f,alpha);

	glBegin(GL_POLYGON);
	glVertex3f(x+3,y,0);
	glVertex3f(x+dx-3,y,0);
	glVertex3f(x+dx-1,y+1,0);
	glVertex3f(x+dx,y+3,0);
	glVertex3f(x+dx,y+dy-3,0);
	glVertex3f(x+dx-1,y+dy-1,0);
	glVertex3f(x+dx-3,y+dy,0);
	glVertex3f(x+3,y+dy,0);
	glVertex3f(x+1,y+dy-1,0);
	glVertex3f(x,y+dy-3,0);
	glVertex3f(x,y+3,0);
	glVertex3f(x+1,y+1,0);
	glEnd();

	x++;
	y++;
	dx-=2;
	dx-=2;
	glColor4f(color4_r,color4_g,color4_b,alpha);
	if (!m_enabled) glColor4f(color4_r*0.5f,color4_g*0.5f,color4_b*0.5f,alpha);

	glBegin(GL_POLYGON);
	glVertex3f(x+3,y,0);
	glVertex3f(x+dx-3,y,0);
	glVertex3f(x+dx-1,y+1,0);
	glVertex3f(x+dx,y+3,0);
	glVertex3f(x+dx,y+dy-3,0);
	glVertex3f(x+dx-1,y+dy-1,0);
	glVertex3f(x+dx-3,y+dy,0);
	glVertex3f(x+3,y+dy,0);
	glVertex3f(x+1,y+dy-1,0);
	glVertex3f(x,y+dy-3,0);
	glVertex3f(x,y+3,0);
	glVertex3f(x+1,y+1,0);
	glEnd();
} /* TGLslider::draw */ 


void TGLslider::draw(void)
{
	draw(1);
} /* TGLslider::draw */ 



