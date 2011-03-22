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

extern int SCREEN_X,SCREEN_Y;

extern float color1_r,color1_g,color1_b;
extern float color2_r,color2_g,color2_b;
extern float color3_r,color3_g,color3_b;


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

void TGLTextInputFrame::draw(float alpha)
{
	TGLframe::draw(alpha);
/*
	TGLinterface::print_left(m_editing,m_font,m_x+8,m_y+(m_dy/2)+TTF_FontHeight(m_font)/2,color1_r,color1_g,color1_b,alpha);
	
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
			glColor4f(1,0,0,f*alpha);
			glBegin(GL_POLYGON);
			glVertex3f(float(m_x+8+tdx),m_y+8,0);
			glVertex3f(float(m_x+8+tdx+4),m_y+8,0);
			glVertex3f(float(m_x+8+tdx+4),m_y+24,0);
			glVertex3f(float(m_x+8+tdx),m_y+24,0);
			glEnd();
		}
	} // if
*/
} /* TGLTextInputFrame::draw */ 


void TGLTextInputFrame::draw(void)
{
	TGLframe::draw();

    GLint bb[4];
    glGetIntegerv(GL_SCISSOR_BOX, bb);
	glScissor(int(m_x+4),int(SCREEN_Y-(m_y+4+(m_dy-8))),int(m_dx-8),int(m_dy-8));
    glEnable(GL_SCISSOR_TEST);


	// draw the cursor:
	if (m_focus) {
		float start_x = 0,start_y = 0;
		float cursor_x,cursor_y,cursor_dx,cursor_dy;
		float text_y = m_y+(m_dy/2)+TTF_FontHeight(m_font)/2;
		char tmp[255];
		int tdx=0,tdy=0;

		strcpy(tmp,m_editing);
		tmp[m_editing_position]=0;
		TTF_SizeText(m_font,tmp,&tdx,&tdy);

		cursor_x = m_x+8+tdx;
		cursor_y = text_y-TTF_FontHeight(m_font)+2;
		cursor_dx = 4.0f;
		cursor_dy = TTF_FontHeight(m_font)-4.0f;

		if (cursor_x+cursor_dx>=m_x+m_dx-4) start_x = (m_x+m_dx-4)-(cursor_x + cursor_dx);
		if (cursor_y+cursor_dy>=m_y+m_dy-4) start_y = (m_y+m_dy-4)-(cursor_y + cursor_dy);

		if (m_enabled) {
			TGLinterface::print_left(m_editing,m_font,m_x+8+start_x,text_y+start_y,color1_r,color1_g,color1_b,1);
		} else {
			TGLinterface::print_left(m_editing,m_font,m_x+8+start_x,text_y+start_y,color1_r,color1_g,color1_b,0.5f);
		} // if 

		{
			float f;
			f=float(0.5+0.3*sin((m_cycle)*0.3F));
			glColor4f(1,0,0,f);
			glBegin(GL_POLYGON);
			glVertex3f(cursor_x+start_x,cursor_y+start_y,0);
			glVertex3f(cursor_x+cursor_dx+start_x,cursor_y+start_y,0);
			glVertex3f(cursor_x+cursor_dx+start_x,cursor_y+cursor_dy+start_y,0);
			glVertex3f(cursor_x+start_x,cursor_y+cursor_dy+start_y,0);
			glEnd();
		}
	} else {
		float text_y = m_y+(m_dy/2)+TTF_FontHeight(m_font)/2;
		if (m_enabled) {
			TGLinterface::print_left(m_editing,m_font,m_x+8,text_y,color1_r,color1_g,color1_b,1);
		} else {
			TGLinterface::print_left(m_editing,m_font,m_x+8,text_y,color1_r,color1_g,color1_b,0.5f);
		} // if 
	} // if

    glDisable(GL_SCISSOR_TEST);
	glScissor(bb[0],bb[1],bb[2],bb[3]);

} /* TGLTextInputFrame::draw */ 


bool TGLTextInputFrame::mouse_over(int mousex,int mousey)
{
	if (mousex>=m_x && mousex<m_x+m_dx &&
		mousey>=m_y && mousey<m_y+m_dy) return true;
	return false;
}


bool TGLTextInputFrame::check_status(int mousex,int mousey,int button,int button_status,KEYBOARDSTATE *k)
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

