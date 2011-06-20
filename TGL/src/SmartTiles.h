/*
 *  SmartTiles.h
 *  TransballGL
 *
 *  Created by Santi Ontañón on 6/20/11.
 *
 */

#ifndef _TGL_SMARTTILES
#define _TGL_SMARTTILES


class SmartTileCacheEntry {
public:
	GLTile *t1,*t2;
	int direction,windowsize;	
	float result;
	
	int hash() {
		return (int(t1) + int(t2) + direction + windowsize)%1024;
	}
};


extern float smartTileScore(GLTile *t1, GLTile *t2, int direction, int windowSize);
extern float smartTileNumberOfNonZeroPizels(GLTile *t);
extern GLTile *smartTile(TGLmap *map, int x,int y,float wleft,float wup, float wright,float wdown,List<GLTile> *tiles);

#endif