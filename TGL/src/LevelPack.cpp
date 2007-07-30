#ifdef KITSCHY_DEBUG_MEMORY 
#include "debug_memorymanager.h"
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "SDL_rotozoom.h"

#include "List.h"
#include "Symbol.h"
#include "XMLparser.h"
#include "LevelPack.h"

#include "GLTManager.h"
#include "SFXManager.h"
#include "TGLmap.h"


LevelPack_Level::LevelPack_Level(GLTManager *GLTM)
{
	m_map=0;
	m_name=0;
	m_description=0;
	m_initial_fuel=100;
	m_points=1;

	m_map_data = new TGLmap(GLTM);
} /* LevelPack_Level::LevelPack_Level */ 


LevelPack_Level::LevelPack_Level(FILE *fp,GLTManager *GLTM)
{

	m_map=0;
	m_name=0;
	m_description=0;
	m_initial_fuel=100;
	m_points=1;

	// Load it from a file:
	XMLNode *node;
	node=XMLNode::from_file(fp);
	load(node,GLTM);
	delete node;

} /* LevelPack_Level::LevelPack_Level */ 


LevelPack_Level::LevelPack_Level(XMLNode *node,GLTManager *GLTM)
{

	m_map=0;
	m_name=0;
	m_description=0;
	m_initial_fuel=100;
	m_points=1;

	load(node,GLTM);

} /* LevelPack_Level::LevelPack_Level */ 


void LevelPack_Level::load(XMLNode *node,GLTManager *GLTM)
{
	List<XMLNode> *children;
	XMLNode *n;

	if (!node->get_type()->cmp("level")) return;
	children=node->get_children();
	children->Rewind();
	while(children->Iterate(n)) {
		if (n->get_type()->cmp("map")) {
			m_map=new char[strlen(n->get_value()->get())+1];
			strcpy(m_map,n->get_value()->get());
		} // if 
		if (n->get_type()->cmp("name")) {
			m_name=new char[strlen(n->get_value()->get())+1];
			strcpy(m_name,n->get_value()->get());
		} // if 
		if (n->get_type()->cmp("description")) {
			m_description=new char[strlen(n->get_value()->get())+1];
			strcpy(m_description,n->get_value()->get());
		} // if 
		if (n->get_type()->cmp("initial-fuel")) {
			m_initial_fuel=atoi(n->get_value()->get());
		} // if 
		if (n->get_type()->cmp("points")) {
			m_points=atoi(n->get_value()->get());
		} // if 
	} // while 
	delete children;

	m_map_data = new TGLmap(GLTM);
} /* LevelPack_Level::load */ 


LevelPack_Level::~LevelPack_Level()
{
	if (m_map!=0) delete []m_map;
	m_map=0;
	if (m_name!=0) delete []m_name;
	m_name=0;
	if (m_description!=0) delete []m_description;
	m_description=0;

	if (m_map_data!=0) delete m_map_data;
	m_map_data=0;
} /* LevelPack_Level::~LevelPack_Level */ 


void LevelPack_Level::setName(char* name)
{
	if (m_name!=0) delete m_name;
	m_name = new char[strlen(name)+1];
	strcpy(m_name,name);
} /* LevelPack_Level::setName */ 




LevelPack::LevelPack()
{
	m_id=0;
	m_name=0;
	m_description=0;
	m_creator_name=0;
	m_creator_nickname=0;

} /* LevelPack::LevelPack */ 


LevelPack::LevelPack(FILE *fp,GLTManager *GLTM)
{
	m_id=0;
	m_name=0;
	m_description=0;
	m_creator_name=0;
	m_creator_nickname=0;

	// Load it from a file:
	XMLNode *node;
	node=XMLNode::from_file(fp);
	load(node,GLTM);
	delete node;

} /* LevelPack::LevelPack */ 


void LevelPack::load(XMLNode *node,GLTManager *GLTM)
{
	List<XMLNode> *children;
	XMLNode *n;

	if (!node->get_type()->cmp("levelpack")) return;
	children=node->get_children();
	children->Rewind();
	while(children->Iterate(n)) {
		if (n->get_type()->cmp("id")) {
			m_id=new char[strlen(n->get_value()->get())+1];
			strcpy(m_id,n->get_value()->get());
		} // if 
		if (n->get_type()->cmp("name")) {
			m_name=new char[strlen(n->get_value()->get())+1];
			strcpy(m_name,n->get_value()->get());
		} // if 
		if (n->get_type()->cmp("description")) {
			m_description=new char[strlen(n->get_value()->get())+1];
			strcpy(m_description,n->get_value()->get());
		} // if 
		if (n->get_type()->cmp("creator")) {
			List<XMLNode> *children2;
			XMLNode *n2;

			children2=n->get_children();
			children2->Rewind();
			while(children2->Iterate(n2)) {
				if (n2->get_type()->cmp("name")) {
					m_creator_name=new char[strlen(n2->get_value()->get())+1];
					strcpy(m_creator_name,n2->get_value()->get());
				} // if 
				if (n2->get_type()->cmp("nickname")) {
					m_creator_nickname=new char[strlen(n2->get_value()->get())+1];
					strcpy(m_creator_nickname,n2->get_value()->get());
				} // if 
			} // while
			delete children2;
		} // if 
		if (n->get_type()->cmp("levels")) {
			List<XMLNode> *children2;
			XMLNode *n2;

			children2=n->get_children();
			children2->Rewind();
			while(children2->Iterate(n2)) {
				if (n2->get_type()->cmp("level")) {
					m_levels.Add(new LevelPack_Level(n2,GLTM));
				} // if 
			} // while
			delete children2;
		} // if 
	} // while 
	delete children;
} /* LevelPack::load */ 


LevelPack::~LevelPack()
{
	if (m_id!=0) delete []m_id;
	m_id=0;
	if (m_name!=0) delete []m_name;
	m_name=0;
	if (m_description!=0) delete []m_description;
	m_description=0;
	if (m_creator_name!=0) delete []m_creator_name;
	m_creator_name=0;
	if (m_creator_nickname!=0) delete []m_creator_nickname;
	m_creator_nickname=0;
} /* LevelPack::~LevelPack */ 


int LevelPack::get_points(int level)
{
	if (level>=0 && level<m_levels.Length()) return m_levels[level]->m_points;

	return 0;
} /* LevelPack::get_points */ 


int LevelPack::getLevelPosition(LevelPack_Level *level)
{
	return m_levels.PositionRef(level);
} /* LevelPack::getLevelPosition */ 


int LevelPack::getNLevels()
{
	return m_levels.Length();
} /* LevelPack::getNLevels */ 


LevelPack_Level *LevelPack::getLevel(int n)
{
	return m_levels[n];
} /* LevelPack::getLevel */ 


LevelPack_Level *LevelPack::getLevel(char *name)
{
	LevelPack_Level *l;

	m_levels.Rewind();
	while(m_levels.Iterate(l)) {
		if (strcmp(l->m_name,name)==0) return l;
	} // while

	return 0;
} /* LevelPack::getLevel */ 


void LevelPack::save(FILE *fp)
{
	LevelPack_Level *l;

	fprintf(fp,"<levelpack>\n");
	fprintf(fp,"  <id>%s</id>\n",m_id);
	fprintf(fp,"  <name>%s</name>\n",m_name);
	fprintf(fp,"  <description>%s</description>\n",m_description);
	fprintf(fp,"  <creator>\n");
    if (m_creator_name!=0) fprintf(fp,"    <name>%s</name>\n",m_creator_name);
    if (m_creator_nickname!=0) fprintf(fp,"    <nickname>%s</nickname>\n",m_creator_nickname);
	fprintf(fp,"  </creator>\n");
	fprintf(fp,"  <levels>\n");
	
	m_levels.Rewind();
	while(m_levels.Iterate(l)) l->save(fp);

	fprintf(fp,"  </levels>\n");
	fprintf(fp,"</levelpack>\n");
} /* LevelPack::save */ 


/*
        <level>
            <map>maps/secondassault/sa-level1.tgl</map>
            <name>Wellcome back!</name>
            <description>This level is just to warm up again...</description>
            <points>2</points>
        </level>
*/

void LevelPack_Level::save(FILE *fp)
{
	fprintf(fp,"    <level>\n");
	if (m_map!=0) fprintf(fp,"      <map>%s</map>\n",m_map);
	if (m_name!=0) fprintf(fp,"      <name>%s</name>\n",m_name);
	if (m_description!=0) fprintf(fp,"      <description>%s</description>\n",m_description);
	fprintf(fp,"      <points>%i</points>\n",m_points);
	fprintf(fp,"    </level>\n");
} /* LevelPack_Level::save */ 

