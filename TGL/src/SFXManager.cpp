#ifdef KITSCHY_DEBUG_MEMORY 
#include "debug_memorymanager.h"
#endif


#ifdef _WIN32
#include "windows.h"
#else
#include "dirent.h"
#endif

#include "SDL.h"
#include "SDL_mixer.h"

#include "stdlib.h"
#include "string.h"
#include "List.h"

#include "Symbol.h"
#include "sound.h"

#include "SFXManager.h"


SFXManagerNode::~SFXManagerNode()
{
	if (m_name!=0) delete m_name;
	m_name=0;
	if (m_sfx!=0) Sound_delete_sound(m_sfx);
	m_sfx=0;
} /* SFXManagerNode::~SFXManagerNode */ 


SFXManager::SFXManager()
{
	m_hash=new List<SFXManagerNode>[SYMBOL_HASH_SIZE];
	
	
} /* SFXManager::SFXManager */ 


SFXManager::~SFXManager()
{
	m_already_played.ExtractAll();
	delete []m_hash;
	m_hash=0;
} /* SFXManager::~SFXManager */ 


Mix_Chunk *SFXManager::get(char *name)
{
	Symbol *s=new Symbol(name);
	Mix_Chunk *t=get(s);
	delete s;
	return t;
} /* SFXManager::get */ 


Mix_Chunk *SFXManager::get(Symbol *name)
{
	SFXManagerNode *n;
	int h=name->hash_function();

	m_hash[h].Rewind();
	while(m_hash[h].Iterate(n)) {
		if (n->m_name->cmp(name)) {
			if (!m_already_played.MemberRefP(n)) {
				m_already_played.Add(n);
				return n->m_sfx;
			} else {
				return 0;
			} // if 
		} // if 
	} /* while */ 

	// load the sfx:
	{
		char filename[256];
		Mix_Chunk *t;

		sprintf(filename,"sound/%s",name->get());		
		t=Sound_create_sound(filename);
		n=new SFXManagerNode();
		n->m_sfx=t;
		n->m_name=new Symbol(name);
		m_hash[h].Add(n);

		m_already_played.Add(n);
		return t;
	}
} /* SFXManager::get */ 


// helper function for the cache
// filter the dir specified for only ogg/wav/mp3
#ifndef _WIN32
#ifdef __APPLE__
	int filter_sfx(struct dirent *de) {
#else
	int filter_sfx(const struct dirent *de) {
#endif //__APPLE__
		if (
		strcmp(".ogg", de->d_name+strlen(de->d_name)-4) == 0 || 
		strcmp(".wav", de->d_name+strlen(de->d_name)-4) == 0 || 
		strcmp(".mp3", de->d_name+strlen(de->d_name)-4) == 0
		) {
		return(1);
	} else {
		return(0);
	}
}
#endif

// helper function for the cache;
// remove file extensions, since Santi's Sound.cpp doesn't like 'em
char *SFXManager::remove_extension(char *filename)
{
	char *p = strrchr(filename, '\\');
	if (p != NULL) {
		p = strrchr(p+1, '.'); 
	} else { 
		p = strrchr(filename, '.'); 
	}
	if (p != NULL) {
		*p = '\0'; 	
	}
	return filename;
}

// read specified dir for sfx and cache them
void SFXManager::cache(char *localpath)
{
	// FIXME: unfortunately the entire game has a hardcoded use of the sound/ dir...
	char path[256];
	char *filename;
	sprintf(path, "sound/%s", localpath);
	
	#ifdef _WIN32
		WIN32_FIND_DATA finfo;
		HANDLE h;
		char pattern[256], tmp[256];
		sprintf(pattern, "%s/*.*", path);
		 
		 h = FindFirstFile(pattern, &finfo);
		 if (h != INVALID_HANDLE_VALUE) {
			 strcpy(tmp, finfo.cFileName);
			 while(FindNextFile(h, &finfo) == TRUE) {
				 if (
					 (
						finfo.cFileName[strlen(finfo.cFileName) - 1] =='g' &&
						finfo.cFileName[strlen(finfo.cFileName) - 2] =='g' &&
						finfo.cFileName[strlen(finfo.cFileName) - 3] =='o'
					 ) ||
					 (
						finfo.cFileName[strlen(finfo.cFileName) - 1] =='3' &&
						finfo.cFileName[strlen(finfo.cFileName) - 2] =='p' &&
						finfo.cFileName[strlen(finfo.cFileName) - 3] =='m'
					) ||
					(
						finfo.cFileName[strlen(finfo.cFileName) - 1] =='v' &&
						finfo.cFileName[strlen(finfo.cFileName) - 2] =='a' &&
						finfo.cFileName[strlen(finfo.cFileName) - 3] =='w'
					 )
					 &&
						finfo.cFileName[strlen(finfo.cFileName) - 4] =='.'
					 ) 
				{					 						
					 filename = remove_extension(finfo.cFileName);
					 sprintf(tmp, "%s/%s", localpath, filename);
					 get(tmp);
				}
			 } 
		 }
	#else
		struct dirent **namelist;
		char tmp[512];
		int i, n;

		// get alpha sorted list of sfx names from "path"
		if ((n = scandir(path, &namelist, filter_sfx, alphasort)) > 0) {
			for (i = 0; i < n; i++) {
				filename = remove_extension(namelist[i]->d_name);
				snprintf(tmp, 512, "%s/%s", localpath, filename);
				get(tmp);
			}
			free(namelist);
		}
	#endif
}

void SFXManager::next_cycle(void)
{
	m_already_played.ExtractAll();
} /* SFXManager::next_cycle */ 

