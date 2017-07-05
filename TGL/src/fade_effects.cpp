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
#include "TGLobject_ship.h"
#include "TGLmap.h"
#include "TGL.h"
#include "TGLapp.h"
#include "TGLreplay.h"


void TGLapp::fade_in_alpha(float f)
{
	glColor4f(0,0,0,f);
	glNormal3f(0.0,0.0,1.0);
	glBegin(GL_QUADS);
	glVertex3f(0,0,0);
	glVertex3f(0,480,0);
	glVertex3f(640,480,0);
	glVertex3f(640,0,0);
	glEnd();
} /* TGLapp::fade_in_alpha */ 


void TGLapp::fade_in_squares(float f,float size)
{
	int i,j;
	float s=size*f*1.2f;

	glColor4f(0,0,0,1);
	glNormal3f(0.0,0.0,1.0);

	for(i=0;i<int(640+size*2);i+=int(size*2)) {
		for(j=0;j<int(480+size*2);j+=int(size*2)) {
			glBegin(GL_QUADS);
			glVertex3f(i-s,j-s,0);
			glVertex3f(i-s,j+s,0);
			glVertex3f(i+s,j+s,0);
			glVertex3f(i+s,j-s,0);
			glVertex3f(i+size-s,j+size-s,0);
			glVertex3f(i+size-s,j+size+s,0);
			glVertex3f(i+size+s,j+size+s,0);
			glVertex3f(i+size+s,j+size-s,0);
			glEnd();			

		} // for
	} // for
} /* TGLapp::fade_in_squares */ 


void TGLapp::fade_in_triangles(float f,float size)
{
	int i,j;
	float s=size*f*1.5f;

	glColor4f(0,0,0,1);
	glNormal3f(0.0,0.0,1.0);

	for(i=0;i<640+size*2;i+=int(size*2)) {
		for(j=0;j<480+size*2;j+=int(size*2)) {			
			glBegin(GL_TRIANGLES);
			glVertex3f(float(i),j-s,0);
			glVertex3f(i+s,j+s,0);
			glVertex3f(i-s,j+s,0);

			glVertex3f(i+size,j+size+s,0);
			glVertex3f(i+size-s,j+size-s,0);
			glVertex3f(i+size+s,j+size-s,0);
			glEnd();			
		} // for
	} // for
} /* TGLapp::fade_in_triangles */ 

