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
#include "SDL_rotozoom.h"

#include "List.h"
#include "Symbol.h"
#include "auxiliar.h"
#include "GLTile.h"
#include "keyboardstate.h"

#include "TGLapp.h"

#include "TGLinterface.h"

TGLConfirmation::TGLConfirmation(char *message,TTF_Font *font,float x,float y,int ID) : TGLInterfaceElement()
{
	m_font = font;
	m_message=new char[strlen(message)+1];
	strcpy(m_message,message);
	m_x=x;
	m_y=y;
	m_ID=ID;
	m_enabled=true;
	m_active=true;
	m_modal=true;
	m_state=0;
	m_cycle=0;

	m_ok_button = new TGLbutton("ok",font,x-100,y+10,80,32,-1);
	m_cancel_button = new TGLbutton("cancel",font,x+20,y+10,80,32,-1);
} /* TGLConfirmation::TGLConfirmation */ 


TGLConfirmation::~TGLConfirmation()
{
	if (m_message!=0) delete []m_message;
	m_message=0;

	delete m_ok_button;
	delete m_cancel_button;

} /* TGLConfirmation::~TGLConfirmation */ 


void TGLConfirmation::draw(void)
{
	draw(1);
} /* TGLConfirmation::draw */ 


void TGLConfirmation::draw(float alpha)
{
	if (!m_enabled) alpha*=0.33f;

	if (m_state==0 && m_cycle<25) alpha*=(m_cycle/25.0f);
	if (m_state==1) {
		if (m_cycle<25) {
			alpha*=((25-m_cycle)/25.0f);
		} else {
			alpha=0;
		} // if 
	} // if

    {
        glEnable(GL_COLOR_MATERIAL);
        {
            float f = alpha*0.75f; 
            glColor4f(0, 0, 0, f);
        }
        glNormal3f(0.0, 0.0, 1.0);

        glBegin(GL_QUADS);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 480, 0);
        glVertex3f(640, 480, 0);
        glVertex3f(640, 0, 0);
        glEnd();
    }

	TGLinterface::print_center(m_message,m_font,m_x,m_y);

	m_ok_button->draw(alpha);
	m_cancel_button->draw(alpha);

} /* TGLConfirmation::draw */ 



bool TGLConfirmation::check_status(int mousex,int mousey,int button,int button_status,KEYBOARDSTATE *k)
{
	if (!m_enabled) {
		return false;
	} // if

	m_cycle++;

	if (m_ok_button->check_status(mousex,mousey,button,button_status,k)) {
		m_state = 1;
		m_cycle=0;
		return true;
	} // if 
	if (m_cancel_button->check_status(mousex,mousey,button,button_status,k)) {
		m_state = 1;
		m_cycle=0;
	} // if 

	if (m_state == 1 && m_cycle>25) m_to_be_deleted = true;

	return false;
} /* TGLConfirmation::check_status */ 



