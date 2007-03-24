#ifdef KITSCHY_DEBUG_MEMORY 
#include "debug_memorymanager.h"
#endif

#ifdef _WIN32
#include "windows.h"
#endif

#include "stdio.h"
#include "math.h"
#include "stdlib.h"
#include "string.h"

#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

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

#include "LevelPack.h"


int TGLapp::mainmenu_cycle(KEYBOARDSTATE *k)
{
	if (m_current_levelpack!=0) delete m_current_levelpack;
	{
		FILE *fp;
		fp=fopen("maps/st2.lp","r+");
//		fp=fopen("maps/sa.lp","r+");
//		fp=fopen("maps/expert.lp","r+");
//		fp=fopen("maps/tutorial.lp","r+");
		if (fp!=0) {
			m_current_levelpack=new LevelPack(fp);
			fclose(fp);

//			return TGL_STATE_REPLAYBROWSER;
			return TGL_STATE_LEVELPACKSCREEN;
		} else {
			return TGL_STATE_NONE;
		} // if 
	} 

} /* TGLapp::mainmenu_cycle */ 

void TGLapp::mainmenu_draw(void)
{
	glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
} /* TGLapp::mainmenu_draw */ 

