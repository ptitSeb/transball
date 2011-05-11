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

TGLConfirmation::TGLConfirmation(char *message,TTF_Font *font,float x,float y,int ID,bool cancelOption) : TGLInterfaceElement()
{
	m_font = font;
	{
		char tmp[256];
		int i,j=0;
		for(i=0;message[i]!=0;i++) {
			if (message[i]=='\n') {
				tmp[j]=0;
				char *tmp2 = new char[strlen(tmp)+1];
				strcpy(tmp2,tmp);
				m_message_lines.Add(tmp2);
				j = 0;
			} else {
				tmp[j++] = message[i];
			}
		}
		if (j!=0) {
			tmp[j]=0;
			char *tmp2 = new char[strlen(tmp)+1];
			strcpy(tmp2,tmp);
			m_message_lines.Add(tmp2);
		}
	}
	
	m_x=x;
	m_y=y;
	m_ID=ID;
	m_enabled=true;
	m_active=true;
	m_modal=true;
	m_state=0;
	m_cycle=0;

	if (cancelOption) {
		m_ok_button = new TGLbutton("ok",font,x-100,y+10,80,32,-1);
		m_cancel_button = new TGLbutton("cancel",font,x+20,y+10,80,32,-1);
	} else {
		m_ok_button = new TGLbutton("ok",font,x-40,y+10,80,32,-1);
		m_cancel_button = NULL;
	}
} /* TGLConfirmation::TGLConfirmation */ 


TGLConfirmation::~TGLConfirmation()
{
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

	int y = m_y - TTF_FontHeight(m_font)*m_message_lines.Length();
	char *message;
	m_message_lines.Rewind();
	while(m_message_lines.Iterate(message)) {
		TGLinterface::print_center(message,m_font,m_x,y);
		y+=TTF_FontHeight(m_font);
	}

	m_ok_button->draw(alpha);
	if (m_cancel_button!=NULL) m_cancel_button->draw(alpha);

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
	if (m_cancel_button!=NULL && m_cancel_button->check_status(mousex,mousey,button,button_status,k)) {
		m_state = 1;
		m_cycle=0;
	} // if 

	if (m_state == 1 && m_cycle>25) m_to_be_deleted = true;

	return false;
} /* TGLConfirmation::check_status */ 



