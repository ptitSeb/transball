#ifdef KITSCHY_DEBUG_MEMORY 
#include "debug_memorymanager.h"
#endif

#ifdef _WIN32
#include <windows.h>
#include "direct.h"
#else
#include <sys/time.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "string.h"
#include <time.h>

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
#include "TGLobject_ball.h"
#include "TGLmap.h"
#include "TGL.h"
#include "TGLapp.h"
#include "XMLparser.h"
#include "TGLreplay.h"

#include "debug.h"

#include "PlayerProfile.h"


/*
// This is the default profile file:

<player-profile>
    <name>default</name>
    <video>
        <mode>window</mode>
        <resolutionx>640</resolutionx>
        <resolutiony>480</resolutiony>
    </video>
    <audio>
        <sfx-volume>128</sfx-volume>
        <music-volume>96</music-volume>
    </audio>
    <input>
        <keyboard player="0">
            <up>113</up>
            <down>97</down>
            <left>111</left>
            <right>112</right>
            <button1>32</button1>
            <button2>13</button2>
            <pause>282</pause>
            <quit>27</quit>
        </keyboard>
    </input>
	<progress>
	</progress>
</player-profile>

*/


PlayerProfileLPProgress::PlayerProfileLPProgress()
{
	m_levelpack_id=0;
	m_levels_completed=0;
} /* PlayerProfileLPProgress::PlayerProfileLPProgress */ 


PlayerProfileLPProgress::~PlayerProfileLPProgress()
{
	if (m_levelpack_id!=0) delete m_levelpack_id;
	m_levelpack_id=0;
} /* PlayerProfileLPProgress::~PlayerProfileLPProgress */ 


PlayerProfile::PlayerProfile(char *name)
{
	if (name!=0) {
		m_name = new char[strlen(name)+1];
		strcpy(m_name,name);
	} else {
		m_name=0;
	} // if 
	m_fullscreen=false;
	m_resolution_x=640;
	m_resolution_y=480;
	m_sfx_volume=MIX_MAX_VOLUME;
	m_music_volume=96;
	m_n_players=1;
	m_keys_configuration[0][KEY_THRUST]=SDLK_q;
	m_keys_configuration[0][KEY_SPECIAL]=SDLK_a;
	m_keys_configuration[0][KEY_LEFT]=SDLK_o;
	m_keys_configuration[0][KEY_RIGHT]=SDLK_p;
	m_keys_configuration[0][KEY_FIRE]=SDLK_SPACE;
	m_keys_configuration[0][KEY_ATTRACTOR]=SDLK_RETURN;
	m_keys_configuration[0][KEY_PAUSE]=SDLK_F1;
	m_keys_configuration[0][KEY_QUIT]=SDLK_ESCAPE;

	m_ships.Add(new int(0));
	m_ships.Add(new int(1));
	m_ships.Add(new int(2));
} /* PlayerProfile::PlayerProfile */ 



PlayerProfile::PlayerProfile(FILE *fp)
{
	m_name=0;
	m_fullscreen=false;
	m_resolution_x=640;
	m_resolution_y=480;
	m_sfx_volume=MIX_MAX_VOLUME;
	m_music_volume=96;
	m_n_players=1;
	m_keys_configuration[0][KEY_THRUST]=SDLK_q;
	m_keys_configuration[0][KEY_SPECIAL]=SDLK_a;
	m_keys_configuration[0][KEY_LEFT]=SDLK_o;
	m_keys_configuration[0][KEY_RIGHT]=SDLK_p;
	m_keys_configuration[0][KEY_FIRE]=SDLK_SPACE;
	m_keys_configuration[0][KEY_ATTRACTOR]=SDLK_RETURN;
	m_keys_configuration[0][KEY_PAUSE]=SDLK_F1;
	m_keys_configuration[0][KEY_QUIT]=SDLK_ESCAPE;

	XMLNode *node=XMLNode::from_file(fp);
	XMLNode *name,*video,*audio,*input,*ships,*progress;
	XMLNode *tmp;

	name=node->get_children("name");
	video=node->get_children("video");
	audio=node->get_children("audio");
	input=node->get_children("input");
	ships=node->get_children("ships");
	progress=node->get_children("progress");

	if (name!=0) {
		m_name = new char[strlen(name->get_value()->get())+1];
		strcpy(m_name,name->get_value()->get());
	} // if 

	if (video!=0) {
		tmp = video->get_children("mode");
		if (tmp!=0) {
			if (tmp->get_value()->cmp("window")) m_fullscreen=false;
										    else m_fullscreen=true;
		} // if 
		tmp = video->get_children("resolutionx");
		if (tmp!=0) {
			m_resolution_x=atoi(tmp->get_value()->get());
		} // if 
		tmp = video->get_children("resolutiony");
		if (tmp!=0) {
			m_resolution_y=atoi(tmp->get_value()->get());
		} // if 
	} // if 

	if (audio!=0) {
		tmp = video->get_children("mode");
		tmp = video->get_children("sfx-volume");
		if (tmp!=0) m_sfx_volume=atoi(tmp->get_value()->get());		
		tmp = video->get_children("music-volume");
		if (tmp!=0) m_music_volume=atoi(tmp->get_value()->get());		
	} // if 

	if (input!=0) {
		List<XMLNode> *l=input->get_children();
		XMLNode *keyboard,*tmp;

		m_n_players=0;

		l->Rewind();
		while(l->Iterate(keyboard)) {
			tmp=keyboard->get_children("up");
			if (tmp!=0) m_keys_configuration[m_n_players][KEY_THRUST]=atoi(tmp->get_value()->get());			
			tmp=keyboard->get_children("down");
			if (tmp!=0) m_keys_configuration[m_n_players][KEY_SPECIAL]=atoi(tmp->get_value()->get());			
			tmp=keyboard->get_children("left");
			if (tmp!=0) m_keys_configuration[m_n_players][KEY_LEFT]=atoi(tmp->get_value()->get());			
			tmp=keyboard->get_children("right");
			if (tmp!=0) m_keys_configuration[m_n_players][KEY_RIGHT]=atoi(tmp->get_value()->get());			
			tmp=keyboard->get_children("button1");
			if (tmp!=0) m_keys_configuration[m_n_players][KEY_FIRE]=atoi(tmp->get_value()->get());			
			tmp=keyboard->get_children("button2");
			if (tmp!=0) m_keys_configuration[m_n_players][KEY_ATTRACTOR]=atoi(tmp->get_value()->get());			
			tmp=keyboard->get_children("pause");
			if (tmp!=0) m_keys_configuration[m_n_players][KEY_PAUSE]=atoi(tmp->get_value()->get());			
			tmp=keyboard->get_children("quit");
			if (tmp!=0) m_keys_configuration[m_n_players][KEY_QUIT]=atoi(tmp->get_value()->get());			

			m_n_players++;
		} // while 
		delete l;
	} // if 

	if (ships!=0) {
		List<XMLNode> *l=ships->get_children();
		XMLNode *ship;

		l->Rewind();
		while(l->Iterate(ship)) {
			m_ships.Add(new int(atoi(ship->get_value()->get())));
		} // while
		delete l;
	} // if 

	if (progress!=0) {
		List<XMLNode> *l=progress->get_children();
		XMLNode *levelpack,*tmp,*results,*result;
		PlayerProfileLPProgress *lpp;
		PlayerProfileLevelResult *lr;

		l->Rewind();
		while(l->Iterate(levelpack)) {
			lpp=new PlayerProfileLPProgress();

			tmp=levelpack->get_children("id");
			if (tmp!=0) {
				lpp->m_levelpack_id=new char[strlen(tmp->get_value()->get())+1];
				strcpy(lpp->m_levelpack_id,tmp->get_value()->get());
			} // if 
			tmp=levelpack->get_children("levels-completed");
			if (tmp!=0) lpp->m_levels_completed=atoi(tmp->get_value()->get());

			results=levelpack->get_children("results");
			if (results!=0) {
				List<XMLNode> *l2=results->get_children();

				l2->Rewind();
				while(l2->Iterate(result)) {
					lr=new PlayerProfileLevelResult();

					tmp=result->get_children("level");
					if (tmp!=0) lr->m_level=atoi(tmp->get_value()->get());
					tmp=result->get_children("ship");
					if (tmp!=0) lr->m_ship=atoi(tmp->get_value()->get());
					tmp=result->get_children("time");
					if (tmp!=0) lr->m_best_time=atoi(tmp->get_value()->get());
					tmp=result->get_children("fuelused");
					if (tmp!=0) lr->m_fuel_used=atoi(tmp->get_value()->get());
					tmp=result->get_children("shots");
					if (tmp!=0) lr->m_shots=atoi(tmp->get_value()->get());
					tmp=result->get_children("impacts");
					if (tmp!=0) lr->m_impacts=atoi(tmp->get_value()->get());
					tmp=result->get_children("kills");
					if (tmp!=0) lr->m_kills=atoi(tmp->get_value()->get());

					lpp->m_results.Add(lr);
				} // while 
				delete l2;
			} // if 

			m_progress.Add(lpp);
			
		} // while 
		delete l;
	} // if 

	delete node;
} /* PlayerProfile::PlayerProfile */ 


PlayerProfile::~PlayerProfile()
{
	if (m_name!=0) delete []m_name;
	m_name=0;
} /* PlayerProfile::~PlayerProfile */ 


bool PlayerProfile::save(FILE *fp)
{
	int i,*ip;
	PlayerProfileLPProgress *lpp;
	PlayerProfileLevelResult *lr;

	fprintf(fp,"<player-profile>\n");
	if (m_name!=0) fprintf(fp,"  <name>%s</name>\n",m_name);
	fprintf(fp,"  <video>\n");
	fprintf(fp,"    <mode>%s</mode>\n",(m_fullscreen ? "fullscreen":"window"));
	fprintf(fp,"    <resolutionx>%i</resolutionx>\n",m_resolution_x);
	fprintf(fp,"    <resolutiony>%i</resolutiony>\n",m_resolution_y);
	fprintf(fp,"  </video>\n");
	fprintf(fp,"  <audio>\n");
	fprintf(fp,"    <sfx-volume>%i</sfx-volume>\n",m_sfx_volume);
	fprintf(fp,"    <music-volume>%i</music-volume>\n",m_music_volume);
	fprintf(fp,"  </audio>\n");
	fprintf(fp,"  <input>\n");
	for(i=0;i<m_n_players;i++) {
		fprintf(fp,"      <keyboard player=\"%i\">\n",i);
		fprintf(fp,"        <up>%i</up>\n",m_keys_configuration[i][KEY_THRUST]);
		fprintf(fp,"        <down>%i</down>\n",m_keys_configuration[i][KEY_SPECIAL]);
		fprintf(fp,"        <left>%i</left>\n",m_keys_configuration[i][KEY_LEFT]);
		fprintf(fp,"        <right>%i</right>\n",m_keys_configuration[i][KEY_RIGHT]);
		fprintf(fp,"        <button1>%i</button1>\n",m_keys_configuration[i][KEY_FIRE]);
		fprintf(fp,"        <button2>%i</button2>\n",m_keys_configuration[i][KEY_ATTRACTOR]);
		fprintf(fp,"        <pause>%i</pause>\n",m_keys_configuration[i][KEY_PAUSE]);
		fprintf(fp,"        <quit>%i</quit>\n",m_keys_configuration[i][KEY_QUIT]);
		fprintf(fp,"    </keyboard>\n");
	} // for
	fprintf(fp,"  </input>\n");
	fprintf(fp,"  <ships>\n");
	m_ships.Rewind();
	while(m_ships.Iterate(ip)) {
		fprintf(fp,"    <ship>%i</ship>\n",*ip);
	} // while
	fprintf(fp,"  </ships>\n");
	fprintf(fp,"  <progress>\n");
	m_progress.Rewind();
	while(m_progress.Iterate(lpp)) {
		fprintf(fp,"    <levelpack>\n");
		fprintf(fp,"      <id>%s</id>\n",lpp->m_levelpack_id);
		fprintf(fp,"      <levels-completed>%i</levels-completed>\n",lpp->m_levels_completed);
		fprintf(fp,"      <results>\n");
		lpp->m_results.Rewind();
		while(lpp->m_results.Iterate(lr)) {
			fprintf(fp,"        <result>\n");
			fprintf(fp,"          <level>%i</level>\n",lr->m_level);
			fprintf(fp,"          <ship>%i</ship>\n",lr->m_ship);
			fprintf(fp,"          <time>%i</time>\n",lr->m_best_time);
			fprintf(fp,"          <fuelused>%i</fuelused>\n",lr->m_fuel_used);
			fprintf(fp,"          <shots>%i</shots>\n",lr->m_shots);
			fprintf(fp,"          <impacts>%i</impacts>\n",lr->m_impacts);
			fprintf(fp,"          <kills>%i</kills>\n",lr->m_kills);
			fprintf(fp,"        </result>\n");
		} // while
		fprintf(fp,"      </results>\n");
		fprintf(fp,"    </levelpack>\n");
	} // while 
	fprintf(fp,"  </progress>\n");
	fprintf(fp,"</player-profile>\n");

	return true;
} /* PlayerProfile::save */ 


int PlayerProfile::progress_in_levelpack(char *id)
{
	PlayerProfileLPProgress *lpp;

	m_progress.Rewind();
	while(m_progress.Iterate(lpp)) {
		if (strcmp(id,lpp->m_levelpack_id)==0) return lpp->m_levels_completed;
	} // while 
	return 0;
} /* PlayerProfile::progress_in_levelpack */ 



void PlayerProfile::level_completed(char *levelpack_id,int level,TGLreplay *replay)
{
	PlayerProfileLPProgress *lpp,*selected=0;
	PlayerProfileLevelResult *lr,*selected_l=0;

	m_progress.Rewind();
	while(m_progress.Iterate(lpp)) {
		if (strcmp(levelpack_id,lpp->m_levelpack_id)==0) selected=lpp;
	} // while 

	if (selected==0) {
		selected=new PlayerProfileLPProgress();
		m_progress.Add(selected);
		selected->m_levelpack_id=new char[strlen(levelpack_id)+1];
		strcpy(selected->m_levelpack_id,levelpack_id);
	} // if 

	if (level+1>selected->m_levels_completed) selected->m_levels_completed=level+1;

	selected->m_results.Rewind();
	while(selected->m_results.Iterate(lr)) {
		if (lr->m_level==level && lr->m_ship==replay->get_playership(m_name)) selected_l=lr;
	} // while 

	if (selected_l==0) {
		selected_l=new PlayerProfileLevelResult();
		selected_l->m_level=level;
		selected_l->m_ship=replay->get_playership(m_name);
		selected_l->m_best_time=-1;
		selected_l->m_fuel_used=0;
		selected_l->m_shots=0;
		selected_l->m_impacts=0;
		selected_l->m_kills=0;
		selected->m_results.Add(selected_l);
	} // if 

	if (selected_l->m_best_time==-1 || selected_l->m_best_time>replay->get_length()) {
		selected_l->m_best_time=replay->get_length()*18;
		selected_l->m_fuel_used=0;
		selected_l->m_shots=0;
		selected_l->m_impacts=0;
		selected_l->m_kills=0;

		// Save replay:
		{
			FILE *fp;
			char tmp[256];

			sprintf(tmp,"players/%s/%s-level-%i-%i.rpl",m_name,levelpack_id,level,replay->get_playership(m_name));

			fp=fopen(tmp,"w+");
			if (fp==0) {
				// assume the folder does not exist:
				char tmp2[256];
				sprintf(tmp2,"players/%s",m_name);
				_mkdir(tmp2);

				fp=fopen(tmp,"w+");
			} // if 

			if (fp!=0) {
				replay->save(fp);
				fclose(fp);
			} // if 
		}
	} // if 

} /* PlayerProfile::level_completed */ 


int PlayerProfile::get_besttime(char *levelpack_id,int level,int ship)
{
	PlayerProfileLPProgress *lpp,*selected=0;
	PlayerProfileLevelResult *lr,*selected_l=0;

	m_progress.Rewind();
	while(m_progress.Iterate(lpp)) {
		if (strcmp(levelpack_id,lpp->m_levelpack_id)==0) selected=lpp;
	} // while 

	if (selected==0) return -1;

	selected->m_results.Rewind();
	while(selected->m_results.Iterate(lr)) {
		if (lr->m_level==level && lr->m_ship==ship) selected_l=lr;
	} // while 

	if (selected_l==0) return -1;

	return selected_l->m_best_time;
} /* PlayerProfile::get_besttime */ 

