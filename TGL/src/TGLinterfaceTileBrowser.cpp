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

TGLTileBrowser::TGLTileBrowser(float x,float y,float dx,float dy,int ID) : TGLInterfaceElement()
{
	m_x=x;
	m_y=y;
	m_dx=dx;
	m_dy=dy;
	m_enabled=true;
	m_active=false;
	m_ID = ID;

	m_mouse_over = -1;
	m_selected = -1;

	m_slider_pos = 0.0f;
	m_slider_height = 0.0f;

} /* TGLTileBrowser::TGLTileBrowser */ 


TGLTileBrowser::~TGLTileBrowser()
{
	m_entries.ExtractAll();
} /* TGLTileBrowser::~TGLTileBrowser */ 


void TGLTileBrowser::draw(void)
{
	draw(1);
} /* TGLTileBrowser::draw */ 


void TGLTileBrowser::draw(float alpha)
{
	// Compute the height:
	int height = 0;
	{
		GLTile *t;
		List<GLTile> l;
		l.Instance(m_entries);
		while(l.Iterate(t)) height+=t->get_dy()+4;
	}

	glBegin(GL_QUADS);
	glColor4f(0,0,0,alpha*0.5f);
	glVertex3f(m_x,m_y,0);
	glVertex3f(m_x+m_dx,m_y,0);
	glVertex3f(m_x+m_dx,m_y+m_dy,0);
	glVertex3f(m_x,m_y+m_dy,0);

	glVertex3f(m_x,m_y,0);
	glVertex3f(m_x+m_dx,m_y,0);
	glVertex3f(m_x+m_dx,m_y+4,0);
	glVertex3f(m_x,m_y+4,0);

	glVertex3f(m_x,m_y,0);
	glVertex3f(m_x+4,m_y,0);
	glVertex3f(m_x+4,m_y+m_dy,0);
	glVertex3f(m_x,m_y+m_dy,0);

	glVertex3f(m_x+m_dx-4,m_y,0);
	glVertex3f(m_x+m_dx,m_y,0);
	glVertex3f(m_x+m_dx,m_y+m_dy,0);
	glVertex3f(m_x+m_dx-4,m_y+m_dy,0);

	glVertex3f(m_x,m_y+m_dy-4,0);
	glVertex3f(m_x+m_dx,m_y+m_dy-4,0);
	glVertex3f(m_x+m_dx,m_y+m_dy,0);
	glVertex3f(m_x,m_y+m_dy,0);	
	glEnd();

	// Draw the slider:
	{
		if (m_entries.Length()>0) {
			m_slider_height = (m_dy-8)/height;
			if (m_slider_height>=1) {
				m_slider_pos = 0;
				m_slider_height = -1;
			} else {
				m_slider_height = m_slider_height*(m_dy-8);
			    if (m_slider_pos + m_slider_height>m_dy-8) {
					m_slider_pos = m_dy-8 - m_slider_height;
				} // if
			} // if 

		} else {
			m_slider_pos = 0;
			m_slider_height = -1;
		} // if 

		if (m_slider_height>0) {
			glBegin(GL_QUADS);
			glColor4f(0.5f,1,0.5f,alpha*0.5f);
			glVertex3f(m_x+m_dx-20,m_y+4+m_slider_pos,0);
			glVertex3f(m_x+m_dx-4,m_y+4+m_slider_pos,0);
			glVertex3f(m_x+m_dx-4,m_y+4+m_slider_pos+m_slider_height,0);
			glVertex3f(m_x+m_dx-20,m_y+4+m_slider_pos+m_slider_height,0);
			glEnd();
		} // if 
	}

	// Draw the entries:
	{
        GLint bb[4];
        glGetIntegerv(GL_SCISSOR_BOX, bb);
		glScissor(int(m_x+4),int(SCREEN_Y-(m_y+4+(m_dy-8))),int(m_dx-(8 + 20)),int(m_dy-8));
        glEnable(GL_SCISSOR_TEST);

		int i=0;
		int y;
		GLTile *entry;

		y = int(int(m_y) - (m_slider_pos/(m_dy-8))*height);

		m_entries.Rewind();
		while(m_entries.Iterate(entry) && (y<m_y+m_dy)) {

			if (m_mouse_over == i) {
				glBegin(GL_QUADS);
				glColor4f(0.5f,1,0.5f,alpha*0.5f);
				glVertex3f(m_x+4,float(y+4),0);
				glVertex3f(m_x+m_dx-4,float(y+4),0);
				glVertex3f(m_x+m_dx-4,float(y+entry->get_dy()+4),0);
				glVertex3f(m_x+4,float(y+entry->get_dy()+4),0);
				glEnd();
			} // if 

			if (m_selected==i) {
				glPushMatrix();
				glTranslatef(m_x+6,float(y+2),0);
				glBegin(GL_LINES);
				glColor4f(1,1,1,1);
				glVertex3f(0,0,0);
				glVertex3f(float(entry->get_dx()+4),0,0);
				glVertex3f(0,float(entry->get_dy()+4),0);
				glVertex3f(float(entry->get_dx()+4),float(entry->get_dy()+4),0);
				glVertex3f(0,0,0);
				glVertex3f(0,float(entry->get_dy()+4),0);
				glVertex3f(float(entry->get_dx()+4),0,0);
				glVertex3f(float(entry->get_dx()+4),float(entry->get_dy()+4),0);
				glEnd();
				glPopMatrix();
			} // if

			entry->draw(m_x+8,float(y+4),0,0,1);
			y+=entry->get_dy()+4;;
			i++;
		} // while

        glDisable(GL_SCISSOR_TEST);
		glScissor(bb[0],bb[1],bb[2],bb[3]);
	}
} /* TGLTileBrowser::draw */ 


bool TGLTileBrowser::mouse_over(int mousex,int mousey)
{
	if (mousex>=m_x && mousex<m_x+m_dx &&
		mousey>=m_y && mousey<m_y+m_dy) return true;
	return false;
}


bool TGLTileBrowser::check_status(int mousex,int mousey,int button,int button_status,KEYBOARDSTATE *k)
{
	int height = 0;
	{
		GLTile *t;
		List<GLTile> l;
		l.Instance(m_entries);
		while(l.Iterate(t)) height+=t->get_dy()+4;
	}

	if (mousex>m_x && mousex<m_x+(m_dx-20) && mousey>m_y && mousey<m_y+m_dy) {
		int s=-1;
		{
			int i = 0;
			int y = int(int(m_y) - (m_slider_pos/(m_dy-8))*height);
			int new_y;
			GLTile *t;
			List<GLTile> l;
			l.Instance(m_entries);
			while(l.Iterate(t)) {
				new_y=y+t->get_dy()+4;
				if (mousey>=y && mousey<new_y) s = i;
				i++;
				y=new_y;
			} // while
		}

		if (s<m_entries.Length()) {
			m_mouse_over = s;
		} else {
			m_mouse_over = -1;
		} // if 
	} else {
		m_mouse_over = -1;
	} // if 

	if (button_status!=0 && mousex>m_x+(m_dx-20) && mousex<m_x+m_dx && mousey>m_y && mousey<m_y+m_dy) {
		float p = (mousey-m_y-(m_slider_height/2))/(m_dy-8-m_slider_height);
		if (p<0) p=0;
		if (p>1) p=1;
		m_slider_pos = p*(m_dy-8-m_slider_height);
	} // if

	if (m_mouse_over != -1 && button!=0) {
		m_selected = m_mouse_over;

		return true;
	} // if 

	if (button!=0 && mousex>=m_x && mousey>=m_y && mousex<=m_x+m_dx && mousey<=m_y+m_dy) return true;

	return false;
} /* TGLTileBrowser::check_status */ 


void TGLTileBrowser::clear(void)
{
	m_entries.Delete();
} /* TGLTileBrowser::clear */ 


void TGLTileBrowser::addEntry(GLTile  *t)
{
	m_entries.Add(t);
} /* TGLTileBrowser::addEntry */  


GLTile *TGLTileBrowser::getEntry(int i)
{
	return m_entries[i];
} /* TGLTileBrowser::addEntry */  


void TGLTileBrowser::deleteEntry(int i)
{
	m_entries.DeletePosition(i);
} /* TGLTileBrowser::addEntry */  



void TGLTileBrowser::setSelected(int i)
{
	if (i<-1) i=-1;
	if (i>=m_entries.Length()) i = m_entries.Length()-1;
	m_selected = i;
	
	int height = 0;
	{
		GLTile *t;
		List<GLTile> l;
		l.Instance(m_entries);
		while(l.Iterate(t)) height+=t->get_dy()+4;
	}
	int first_y = int(-(m_slider_pos/(m_dy-8))*height);

	int current = 0;
	int selected_y = 0;
	int selected_height = 0;
	GLTile *t;
	List<GLTile> l;
	l.Instance(m_entries);
	while(l.Iterate(t)) {
		if (current == m_selected) {
			selected_height = t->get_dy()+4;

			break;
		}
		current++;
		selected_y=selected_y+t->get_dy()+4;
	} // while

	if (selected_y+first_y < 0) {
		m_slider_pos = (float(selected_y)*(m_dy-8))/height;
	} else if (selected_y+first_y+selected_height > m_dy) {
		m_slider_pos = (float(selected_y+selected_height - (m_dy-8))*(m_dy-8))/height;
	}		

	
} /* TGLTileBrowser::setSelected */ 


int TGLTileBrowser::getSelected(void)
{
	return m_selected;
} /* TGLTileBrowser::getSelected */ 


int TGLTileBrowser::getNEntries() 
{
	return m_entries.Length();
}

