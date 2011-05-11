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

#include "debug.h"

#include "TGLapp.h"

#include "TGLinterface.h"


#define TEXT_TILE_BUFFER_SIZE	100

class PrintBuffer {
public:
	~PrintBuffer() {
		delete []text;
		delete tile;
	} //

	TTF_Font *font;
	char *text;
	GLTile *tile;
};

// This stores the last text messages we printed so that we are not generating new tiles all the time (and save CPU time):
List<PrintBuffer> text_tile_buffer;

void TGLinterface::clear_print_cache(void)
{
#ifdef __DEBUG_MESSAGES
		output_debug_message("TGLinterface: Clearing print cache... showing textures used:\n");
		{
			PrintBuffer *pb;
			text_tile_buffer.Rewind();
			while(text_tile_buffer.Iterate(pb)) {
				output_debug_message("%i -> '%s'\n",int(pb->tile->get_texture(0)),pb->text);
			} // while 
		}
#endif

	
	text_tile_buffer.Delete();
} /* TGLinterface::clear_print_cache */ 



GLTile *get_text_tile(char *text,TTF_Font *font)
{
	PrintBuffer *pb;

	text_tile_buffer.Rewind();
	while(text_tile_buffer.Iterate(pb)) {
#ifdef __DEBUG_MESSAGES
		if (pb->tile->get_texture(0)<0) {
				output_debug_message("TGLinterface: Weird stuff just happened!\n");
		} // if 
#endif
		if (pb->font==font && strcmp(pb->text,text)==0) {
			// Move it the text to the top of the list to indicate that it has been used:
			text_tile_buffer.DeleteElement(pb);
			text_tile_buffer.Insert(pb);
#ifdef __DEBUG_MESSAGES
			if (pb->tile->get_texture(0)<0) {
					output_debug_message("TGLinterface: Weird stuff just happened (2)!\n");
			} // if 
#endif
			return pb->tile;
		} // if 
	} // if 

//#ifdef __DEBUG_MESSAGES
//		output_debug_message("New text message '%s', current number ot text tiles: %i\n",text,text_tile_buffer.Length());
//#endif

	while(text_tile_buffer.Length()>=TEXT_TILE_BUFFER_SIZE) {
		pb=text_tile_buffer.Extract();
		delete pb;
	} // while 

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

		pb=new PrintBuffer();
		pb->font=font;
		pb->text=new char[strlen(text)+1];
		strcpy(pb->text,text);
		pb->tile=tile;
		text_tile_buffer.Insert(pb);

#ifdef __DEBUG_MESSAGES
		if (pb->tile->get_texture(0)<0) {
			output_debug_message("TGLinterface: Even weirdest stuff just happened!\n");
		} // if 
#endif

		return tile;
	}



} /* get_text_tile */ 


void TGLinterface::print_left(char *text,TTF_Font *font,float x,float y)
{
	GLTile *tile;

	tile=get_text_tile(text,font);
	glNormal3f(0,0,1);
	tile->set_hotspot(0,tile->get_dy());
	tile->draw(x,y,0,0,1);
} /* TGLinterface::print_left */ 


void TGLinterface::print_center(char *text,TTF_Font *font,float x,float y)
{
	GLTile *tile;

	tile=get_text_tile(text,font);
	glNormal3f(0,0,1);
	tile->set_hotspot(tile->get_dx()/2,tile->get_dy());
	tile->draw(x,y,0,0,1);
} /* TGLinterface::print_center */ 


void TGLinterface::print_left(char *text,TTF_Font *font,float x,float y,float r,float g,float b,float a)
{
	GLTile *tile;

	tile=get_text_tile(text,font);
	glNormal3f(0,0,1);
	tile->set_hotspot(0,tile->get_dy());
	tile->draw(r,g,b,a,x,y,0,0,1);
} /* TGLinterface::print_left */ 


void TGLinterface::print_center(char *text,TTF_Font *font,float x,float y,float r,float g,float b,float a)
{
	GLTile *tile;

	tile=get_text_tile(text,font);
	glNormal3f(0,0,1);
	tile->set_hotspot(tile->get_dx()/2,tile->get_dy());
	tile->draw(r,g,b,a,x,y,0,0,1);
} /* TGLinterface::print_center */ 


void TGLinterface::print_centered(char *text,TTF_Font *font,float x,float y,float r,float g,float b,float a,float angle,float scale)
{
	GLTile *tile;

	tile=get_text_tile(text,font);
	glNormal3f(0,0,1);
	tile->set_hotspot(tile->get_dx()/2,tile->get_dy()/2);
	tile->draw(r,g,b,a,x,y,0,angle,scale);
} /* TGLinterface::print_center */ 


