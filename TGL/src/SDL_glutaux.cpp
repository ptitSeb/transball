#ifdef KITSCHY_DEBUG_MEMORY 
#include "debug_memorymanager.h"
#endif

#ifdef _WIN32
#include "windows.h"
#endif

#include <GL/gl.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <math.h>

#include "auxiliar.h"
#include "debug.h"

#include "GLTile.h"
#include "SDL_glutaux.h"

int nearest_2pow(int n)
{
	int res=2;

	for(res=2;res<2048;res*=2) {
		if (res>=n) return res;
	} /* for */ 

	return res;
} /* nearest_2pow */ 


GLuint  createTexture(SDL_Surface *sfc,float *tx,float *ty)
{
	unsigned int tname=0;
	int szx,szy;

	if (sfc!=0) {
		SDL_Surface *sfc2=0;

		szx=nearest_2pow(sfc->w);
		szy=nearest_2pow(sfc->h);
		*tx=(sfc->w)/float(szx);
		*ty=(sfc->h)/float(szy);

		sfc2=SDL_CreateRGBSurface(SDL_SWSURFACE,szx,szy,32,RMASK,GMASK,BMASK,AMASK);
		SDL_SetAlpha(sfc, 0, 0);
		SDL_BlitSurface(sfc,0,sfc2,0);

		//glGenTextures(1,&tname);
		if (!Find_Texture(sfc2, sfc->w, sfc->h, false, false, &tname))  {
			glPixelStorei(GL_UNPACK_ALIGNMENT,tname);
			glBindTexture(GL_TEXTURE_2D,tname);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);		 
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,szx,szy,0,GL_RGBA,GL_UNSIGNED_BYTE,sfc2->pixels);
		}
		SDL_FreeSurface(sfc2);

#ifdef __DEBUG_MESSAGES
		output_debug_message("createTexture: %i [%i,%i] -> [%i,%i]\n",tname,sfc->w,sfc->h,szx,szy);
#endif

	} else {
		return 0;
	} /* if */ 

	return tname;
} /* createTexture */ 


GLuint  createTextureClamp(SDL_Surface *sfc,float *tx,float *ty)
{
	unsigned int tname=0;
	int szx,szy;

	if (sfc!=0) {
		SDL_Surface *sfc2=0;

		szx=nearest_2pow(sfc->w);
		szy=nearest_2pow(sfc->h);
		*tx=(sfc->w)/float(szx);
		*ty=(sfc->h)/float(szy);

		sfc2=SDL_CreateRGBSurface(SDL_SWSURFACE,szx,szy,32,RMASK,GMASK,BMASK,AMASK);
		SDL_SetAlpha(sfc, 0, 0);
		SDL_BlitSurface(sfc,0,sfc2,0);

//		glGenTextures(1,&tname);
		if (!Find_Texture(sfc2, sfc->w, sfc->h, true, false, &tname))  {
			glPixelStorei(GL_UNPACK_ALIGNMENT,tname);
			glBindTexture(GL_TEXTURE_2D,tname);
	//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);		 
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,szx,szy,0,GL_RGBA,GL_UNSIGNED_BYTE,sfc2->pixels);
		}
		SDL_FreeSurface(sfc2);

#ifdef __DEBUG_MESSAGES
		output_debug_message("createTextureClamp: %i [%i,%i] -> [%i,%i]\n",tname,sfc->w,sfc->h,szx,szy);
#endif

	} else {
		return 0;
	} /* if */ 

	return tname;
} /* createTextureClamp */ 


GLuint  createTextureSmooth(SDL_Surface *sfc,float *tx,float *ty)
{
	unsigned int tname=0;
	int szx,szy;

	if (sfc!=0) {
		SDL_Surface *sfc2=0;

		szx=nearest_2pow(sfc->w);
		szy=nearest_2pow(sfc->h);
		*tx=(sfc->w)/float(szx);
		*ty=(sfc->h)/float(szy);

		sfc2=SDL_CreateRGBSurface(SDL_SWSURFACE,szx,szy,32,RMASK,GMASK,BMASK,AMASK);
		SDL_SetAlpha(sfc, 0, 0);
		SDL_BlitSurface(sfc,0,sfc2,0);

//		glGenTextures(1,&tname);
		if (!Find_Texture(sfc2, sfc->w, sfc->h, false, true, &tname))  {
			glPixelStorei(GL_UNPACK_ALIGNMENT,tname);
			glBindTexture(GL_TEXTURE_2D,tname);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);		 
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,szx,szy,0,GL_RGBA,GL_UNSIGNED_BYTE,sfc2->pixels);
		}
		SDL_FreeSurface(sfc2);

#ifdef __DEBUG_MESSAGES
		output_debug_message("createTextureSmooth: %i [%i,%i] -> [%i,%i]\n",tname,sfc->w,sfc->h,szx,szy);
#endif

	} else {
		return 0;
	} /* if */ 

	return tname;
} /* createTextureSmooth */ 


GLuint  createTextureClampSmooth(SDL_Surface *sfc,float *tx,float *ty)
{
	unsigned int tname=0;
	int szx,szy;

	if (sfc!=0) {
		SDL_Surface *sfc2=0;

		szx=nearest_2pow(sfc->w);
		szy=nearest_2pow(sfc->h);
		*tx=(sfc->w)/float(szx);
		*ty=(sfc->h)/float(szy);

		sfc2=SDL_CreateRGBSurface(SDL_SWSURFACE,szx,szy,32,RMASK,GMASK,BMASK,AMASK);
		SDL_SetAlpha(sfc, 0, 0);
		SDL_BlitSurface(sfc,0,sfc2,0);

//		glGenTextures(1,&tname);
		if (!Find_Texture(sfc2, sfc->w, sfc->h, true, true, &tname))  {
			glPixelStorei(GL_UNPACK_ALIGNMENT,tname);
			glBindTexture(GL_TEXTURE_2D,tname);
	//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);		 
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,szx,szy,0,GL_RGBA,GL_UNSIGNED_BYTE,sfc2->pixels);
		}
		SDL_FreeSurface(sfc2);

#ifdef __DEBUG_MESSAGES
		output_debug_message("createTextureClampSmooth: %i [%i,%i] -> [%i,%i]\n",tname,sfc->w,sfc->h,szx,szy);
#endif

	} else {
		return 0;
	} /* if */ 

	return tname;
} /* createTextureClampSmooth */ 


GLuint  createTextureFromScreen(int x,int y,int dx,int dy,float *tx,float *ty)
{
	unsigned int tname=0;
	int szx,szy;

	szx=nearest_2pow(dx);
	szy=nearest_2pow(dy);
	*tx=(dx)/float(szx);
	*ty=(dy)/float(szy);

	glGenTextures(1,&tname);
	glPixelStorei(GL_UNPACK_ALIGNMENT,tname);
	glBindTexture(GL_TEXTURE_2D,tname);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);		 

	glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,x,y,dx,dy);

	return tname;
} /* createTextureFromScreen */ 

#define CRC32_POLYNOMIAL     0x04C11DB7

unsigned int CRCTable[ 256 ];

unsigned int Reflect( unsigned int ref, char ch )
{
     unsigned int value = 0;

     // Swap bit 0 for bit 7
     // bit 1 for bit 6, etc.
     for (char i = 1; i < (ch + 1); i++)
     {
          if(ref & 1)
               value |= 1 << (ch - i);
          ref >>= 1;
     }
     return value;
}

void CRC_BuildTable()
{
    unsigned int crc;

    for (unsigned i = 0; i <= 255; i++)
        {
        crc = Reflect( i, 8 ) << 24;
        for (unsigned j = 0; j < 8; j++)
                        crc = (crc << 1) ^ (crc & (1 << 31) ? CRC32_POLYNOMIAL : 0);
        
        CRCTable[i] = Reflect( crc, 32 );
    }
}

unsigned int CRC32( unsigned int crc, void *buffer, unsigned int count )
{
  unsigned int orig = crc;
  unsigned char * p = reinterpret_cast<unsigned char*>(buffer);
  while (count--)
    crc = (crc >> 8) ^ CRCTable[(crc & 0xFF) ^ *p++];
  return crc ^ orig;
}

#define MAX_TEX 1024
GLuint texture_id[MAX_TEX];
unsigned int texture_crc[MAX_TEX];
unsigned int texture_flags[MAX_TEX];
int texture_count[MAX_TEX];
int texture_max;

void Init_TexManager()
{
	for (int i=0; i<MAX_TEX; i++) {
		texture_id[i] = 0;
		texture_crc[i]=0;
		texture_flags[i]=0;
		texture_count[i]=0;
	}
	texture_max = 0;
}

bool Find_Texture(SDL_Surface *sfc, int w, int h, bool clamp, bool smooth, GLuint *ID)
{
	// Calc the CRC
	int buff[3];
	buff[0]=(clamp)?1:0+(smooth)?2:0;
	buff[1]=w;
	buff[2]=h;
	
	unsigned int crc, flags;
	
	flags = CRC32(0, (void*)buff, 3*sizeof(int));
	crc = CRC32(0, sfc->pixels, sfc->pitch*sfc->h);
	
	// Search it
	int i, hole;
	bool ret=true;
	hole = -1;
	for (i=0; i<texture_max; i++) {
		if (!texture_count[i]) 
		{ 
			if (hole==-1) hole = i;
		} 
		else
		if ((texture_flags[i]==flags) && (texture_crc[i]==crc))
			break;
	}
	if (i>=texture_max) {	// not found, add a new one
		GLuint id;
		glGenTextures(1, &id);
		if (texture_max==MAX_TEX) {
			*ID=id;
			return false;
		}	// list is full
		if (hole==-1) {
			hole=texture_max++;
		}
		texture_flags[hole]=flags;
		texture_crc[hole]=crc;
		texture_id[hole]=id;
		i = hole;
		ret=false;
	} else {
	}
	texture_count[i]++;
	*ID=texture_id[i];		// found
	return ret;
}

void TM_glDeleteTextures(GLuint n, GLuint *first)
{
	int i;
	for (int j=0; j<n; j++) {
		bool del=false;
		for (i=0; i<texture_max; i++) {
			if (texture_id[i]==first[j]) {
				del=true;
				if ((--texture_count[i])==0) {
					glDeleteTextures(1, &(texture_id[i]));
				}
				break;
			}
		}
		if (!del) {
			glDeleteTextures(1, &(first[j]));
		}
	}
	// now, reduce texture_max
	i=texture_max-1;
	while ((i>=0) && (texture_count[i]==0)) {
		texture_max--; i--;
	}
}


/*

void gl_print_left(int x,int y,char *text,int r,int g,int b,TTF_Font *font)
{
	SDL_Surface *sfc;
	SDL_Color c;
	GLTile *t;

	c.r=r;
	c.g=g;
	c.b=b;

	sfc=TTF_RenderText_Blended(font,text,c);
	t=new GLTile(sfc);
	t->draw(x,y,0,1.0);
	SDL_FreeSurface(sfc);
	delete t;
} // gl_print_left  


void gl_print_center(int x,int y,char *text,int r,int g,int b,TTF_Font *font)
{
	SDL_Surface *sfc;
	SDL_Color c;
	GLTile *t;

	c.r=r;
	c.g=g;
	c.b=b;

	sfc=TTF_RenderText_Blended(font,text,c);
	t=new GLTile(sfc);
	t->set_hotspot(sfc->w/2,0);
	t->draw(x,y,0,1.0);
	SDL_FreeSurface(sfc);
	delete t;
} // gl_print_center  
*/

void gl_line(int x1,int y1,int x2,int y2,float r,float g,float b)
{
	glColor3f(r,g,b);
	glBegin(GL_LINES);
	glVertex3f(float(x1),float(y1),0);
	glVertex3f(float(x2),float(y2),0);
	glEnd();
} /* gl_line */ 
