#ifdef KITSCHY_DEBUG_MEMORY 
#include "debug_memorymanager.h"
#endif


#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdlib.h>
#include <string.h>
#include "List.h"
#include "debug.h"

#include "auxiliar.h"
#include "2DCMC.h"
#include "Symbol.h"
#include "GLTile.h"

#include "GLTManager.h"


GLTManagerNode::~GLTManagerNode()
{
	if (m_name!=0) delete m_name;
	m_name=0;
	if (m_tile!=0) delete m_tile;
	m_tile=0;
} /* GLTManagerNode::~GLTManagerNode */ 


GLTManager::GLTManager()
{
	m_hash=new List<GLTManagerNode>[SYMBOL_HASH_SIZE];
	m_hash_smooth=new List<GLTManagerNode>[SYMBOL_HASH_SIZE];
} /* GLTManager::GLTManager */ 


GLTManager::~GLTManager()
{
	delete []m_hash;
	m_hash=0;
	delete []m_hash_smooth;
	m_hash_smooth=0;
} /* GLTManager::~GLTManager */ 


void GLTManager::clear(void)
{
	GLTManagerNode *n;
	int h;

#ifdef __DEBUG_MESSAGES
	output_debug_message("GLTManager: Clearing tile cache... showing textures used:\n");
	for(h=0;h<SYMBOL_HASH_SIZE;h++) {
		m_hash[h].Rewind();
		while(m_hash[h].Iterate(n)) {
			output_debug_message("%i -> '%s'\n",n->m_tile->get_texture(0),n->m_name->get());
		} // if 
		m_hash_smooth[h].Rewind();
		while(m_hash_smooth[h].Iterate(n)) {
			output_debug_message("%i -> '%s' [smooth]\n",n->m_tile->get_texture(0),n->m_name->get());
		} // if 
	} // for
#endif

	m_hash->Delete();
	m_hash_smooth->Delete();
} /* GLTManager::clear */ 


GLTile *GLTManager::get(char *name)
{
	Symbol *s=new Symbol(name);
	GLTile *t=get(s);
	delete s;
	return t;
} /* GLTManager::get */ 


GLTile *GLTManager::get(Symbol *name)
{
	GLTManagerNode *n;
	int h=name->hash_function();

	m_hash[h].Rewind();
	while(m_hash[h].Iterate(n)) {
		if (n->m_name->cmp(name)) return n->m_tile;
	} /* while */ 

	// load the tile:
	{
		char filename[256];
		GLTile *t;

		sprintf(filename,"graphics/%s.png",name->get());
		t=new GLTile(filename);	
		t->set_clamp();
		n=new GLTManagerNode();
		n->m_tile=t;
		n->m_name=new Symbol(name);
		m_hash[h].Add(n);

		int dx=20,dy=20;
		float fx=1.0f,fy=1.0f;

		sprintf(filename,"graphics/%s.nfo",name->get());
		{
			FILE *fp;
			fp=fopen(filename,"rb");
			if (fp!=0) {
				char tmp[80],tmp2[80];
				while(!feof(fp)) {
					fgets(tmp,80,fp);
					if (1==sscanf(tmp,"%s",tmp2)) {
						if (strcmp(tmp2,"hs")==0 ||
							strcmp(tmp2,"HS")==0) {
							int hx,hy;
							sscanf(tmp,"%s %i %i",tmp2,&hx,&hy);
							t->set_hotspot((int)((hx/fx)+0.5F),(int)((hy/fy)+0.5F));
						} // if 
						if (strcmp(tmp2,"bb")==0 ||
							strcmp(tmp2,"BB")==0) {
							int x1,y1,x2,y2;
							sscanf(tmp,"%s %i %i %i %i",tmp2,&x1,&y1,&x2,&y2);
							t->set_cmc((int)((x1/fx)+0.5F),(int)((y1/fy)+0.5F),(int)((x2/fx)+0.5F),(int)((y2/fy)+0.5F));
						} // if 
					} // if 
				} // while 
				fclose(fp);
			} // if 
		}

		return t;
	}
} /* GLTManager::get */ 



GLTile *GLTManager::get_smooth(char *name)
{
	Symbol *s=new Symbol(name);
	GLTile *t=get_smooth(s);
	delete s;
	return t;
} /* GLTManager::get_smooth */ 


GLTile *GLTManager::get_smooth(Symbol *name)
{
	GLTManagerNode *n;
	int h=name->hash_function();

	m_hash_smooth[h].Rewind();
	while(m_hash_smooth[h].Iterate(n)) {
		if (n->m_name->cmp(name)) return n->m_tile;
	} /* while */ 

	// load the tile:
	{
		char filename[256];
		GLTile *t;

		sprintf(filename,"graphics/%s.png",name->get());
		t=new GLTile(filename);
		t->set_smooth();
//		t->set_clamp();
		n=new GLTManagerNode();
		n->m_tile=t;
		n->m_name=new Symbol(name);
		m_hash_smooth[h].Add(n);

		sprintf(filename,"graphics/%s.nfo",name->get());
		{
			float fx=1.0f,fy=1.0f;
			FILE *fp;
			fp=fopen(filename,"rb");
			if (fp!=0) {
				char tmp[80],tmp2[80];
				while(!feof(fp)) {
					fgets(tmp,80,fp);
					if (1==sscanf(tmp,"%s",tmp2)) {
						if (strcmp(tmp2,"hs")==0 ||
							strcmp(tmp2,"HS")==0) {
							int hx,hy;
							sscanf(tmp,"%s %i %i",tmp2,&hx,&hy);
							t->set_hotspot((int)((hx/fx)+0.5F),(int)((hy/fy)+0.5F));
						} // if 
						if (strcmp(tmp2,"bb")==0 ||
							strcmp(tmp2,"BB")==0) {
							int x1,y1,x2,y2;
							sscanf(tmp,"%s %i %i %i %i",tmp2,&x1,&y1,&x2,&y2);
							t->set_cmc((int)((x1/fx)+0.5F),(int)((y1/fy)+0.5F),(int)((x2/fx)+0.5F),(int)((y2/fy)+0.5F));
						} // if 
					} // if 
				} // while 
				fclose(fp);
			} // if 
		}

		return t;
	}
} /* GLTManager::get_smooth */ 


char *GLTManager::get_name(GLTile *tile)
{
	int i;
	List<GLTManagerNode> l;
	GLTManagerNode *n;

	for(i=0;i<SYMBOL_HASH_SIZE;i++) {
		l.Instance(m_hash[i]);
		l.Rewind();
		while(l.Iterate(n)) {
			if (n->m_tile==tile) return n->m_name->get();
		} // while
	} // for

	for(i=0;i<SYMBOL_HASH_SIZE;i++) {
		l.Instance(m_hash_smooth[i]);
		l.Rewind();
		while(l.Iterate(n)) {
			if (n->m_tile==tile) return n->m_name->get();
		} // while
	} // for

	return 0;
} /* GLTManager::get_name */ 

