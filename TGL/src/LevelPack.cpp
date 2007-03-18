#ifdef KITSCHY_DEBUG_MEMORY 
#include "debug_memorymanager.h"
#endif

#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "List.h"
#include "Symbol.h"
#include "XMLparser.h"
#include "LevelPack.h"

LevelPack_Level::LevelPack_Level(FILE *fp)
{

	m_map=0;
	m_name=0;
	m_description=0;
	m_initial_fuel=100;

	// Load it from a file:
	XMLNode *node;
	node=XMLNode::from_file(fp);
	load(node);
	delete node;

} /* LevelPack_Level::LevelPack_Level */ 


LevelPack_Level::LevelPack_Level(XMLNode *node)
{

	m_map=0;
	m_name=0;
	m_description=0;
	m_initial_fuel=100;

	load(node);

} /* LevelPack_Level::LevelPack_Level */ 


void LevelPack_Level::load(XMLNode *node)
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
	} // while 
	delete children;
} /* LevelPack_Level::load */ 


LevelPack_Level::~LevelPack_Level()
{
	if (m_map!=0) delete []m_map;
	m_map=0;
	if (m_name!=0) delete []m_name;
	m_name=0;
	if (m_description!=0) delete []m_description;
	m_description=0;
} /* LevelPack_Level::~LevelPack_Level */ 


LevelPack::LevelPack(FILE *fp)
{
	m_name=0;
	m_description=0;
	m_creator_name=0;
	m_creator_nickname=0;

	// Load it from a file:
	XMLNode *node;
	node=XMLNode::from_file(fp);
	load(node);
	delete node;

} /* LevelPack::LevelPack */ 


void LevelPack::load(XMLNode *node)
{
	List<XMLNode> *children;
	XMLNode *n;

	if (!node->get_type()->cmp("levelpack")) return;
	children=node->get_children();
	children->Rewind();
	while(children->Iterate(n)) {
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
					m_levels.Add(new LevelPack_Level(n2));
				} // if 
			} // while
			delete children2;
		} // if 
	} // while 
	delete children;
} /* LevelPack::load */ 


LevelPack::~LevelPack()
{
	if (m_name!=0) delete []m_name;
	m_name=0;
	if (m_description!=0) delete []m_description;
	m_description=0;
	if (m_creator_name!=0) delete []m_creator_name;
	m_creator_name=0;
	if (m_creator_nickname!=0) delete []m_creator_nickname;
	m_creator_nickname=0;
} /* LevelPack::~LevelPack */ 
