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
#include "TGLmap.h"
#include "TGLobject.h"

#include "collision.h"

#include "debug.h"


/*
 this method assumes that t1 and t2 are squares of the same size = 32
 
 direction: 0 = left, 1 = up, 2 = right, 3 = down
 */
float TGLmap::smartTileScore(GLTile *t1, GLTile *t2, int direction)
{
	Uint32 pixel;
	Uint8 r,g,b,a;
	Uint32 pixel2;;
	Uint8 r2,g2,b2,a2;

	float score = 0;
	for(int i = 0;i<32;i++) {		
		pixel = 0;
		if (t1!=0) {
			switch(direction) {
				case 0:	pixel = t1->get_pixel(0,i); break;
				case 1:	pixel = t1->get_pixel(i,0); break;
				case 2:	pixel = t1->get_pixel(t1->get_dx()-1,i); break;
				case 3:	pixel = t1->get_pixel(i,t1->get_dy()-1); break;
			}				
			SDL_GetRGBA(pixel, t1->get_tile(0)->format, &r, &g, &b, &a);
			r *= (float)(a/255);
			g *= (float)(a/255);
			b *= (float)(a/255);
		} else {
			r = g = b = a = 0;
		}					
		if (t2!=0) {
			switch(direction) {
				case 0:	pixel2 = t2->get_pixel(t2->get_dx()-1,i); break;
				case 1:	pixel2 = t2->get_pixel(i,t2->get_dy()-1); break;
				case 2:	pixel2 = t2->get_pixel(0,i); break;
				case 3:	pixel2 = t2->get_pixel(i,0); break;
			}
			SDL_GetRGBA(pixel2, t2->get_tile(0)->format, &r2, &g2, &b2, &a2);			
			r2 *= (float)(a2/255);
			g2 *= (float)(a2/255);
			b2 *= (float)(a2/255);
		} else {
			r2 = g2 = b2 = a2 = 0;
		}
		score += (float(r-r2)*float(r-r2) + float(g-g2)*float(g-g2) + float(b-b2)*float(b-b2));
	}
	return score;
}


GLTile *TGLmap::smartTile(int x, int y, float wleft, float wup, float wright, float wdown, List<GLTile> *tiles)
{
	GLTile *best = 0;
	float best_score = 0;
	List<GLTile> l;
	GLTile *tile = 0;
	GLTile *tile_left = 0;
	GLTile *tile_up = 0;
	GLTile *tile_right = 0;
	GLTile *tile_down = 0;
	Uint32 pixel;
	Uint8 r,g,b,a;
	Uint32 pixel2;;
	Uint8 r2,g2,b2,a2;
	
	if (x>0) tile_left = m_fg[(x-1)+y*m_fg_dx];
	if (y>0) tile_up = m_fg[(x)+(y-1)*m_fg_dx];
	if (x<m_fg_dx-1) tile_right = m_fg[(x+1)+y*m_fg_dx];
	if (y<m_fg_dy-1) tile_down = m_fg[(x)+(y+1)*m_fg_dx];
	
	l.Instance(*tiles);
	
	/*
	 #ifdef __DEBUG_MESSAGES
	 output_debug_message("smartTile %i,%i with (%i,%i,%i,%i)\n",x,y,l.PositionRef(tile_left), l.PositionRef(tile_up), l.PositionRef(tile_right), l.PositionRef(tile_down));
	 #endif	
	 */
	
	l.Rewind();
	while(l.Iterate(tile)) {		
		float score_left = 0;
		float score_up = 0;
		float score_right = 0;
		float score_down = 0;
		float score = 0;
		if (wleft>0) score_left = smartTileScore(tile,tile_left,0);
		if (wup>0) score_up = smartTileScore(tile,tile_up,1);
		if (wright>0) score_right = smartTileScore(tile,tile_right,2);
		if (wdown>0) score_down = smartTileScore(tile,tile_down,3);

		score += wleft*score_left + wup*score_up + wright*score_right + wdown*score_down;
		
		if (best==0 || score<best_score) {
			best = tile;
			best_score = score;
		}
	}
	
	return best;
}