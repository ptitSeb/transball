#ifndef TGL_LEVEL_PACK
#define TGL_LEVEL_PACK

class LevelPack_Level {
public:
	LevelPack_Level(FILE *fp);
	LevelPack_Level(class XMLNode *node);
	~LevelPack_Level();

	void load(XMLNode *node);

	char *m_map;
	char *m_name;
	char *m_description;
	int m_initial_fuel;
};


class LevelPack {
public:
	LevelPack(FILE *fp);
	~LevelPack();

	void load(XMLNode *node);

	char *m_id;
	char *m_name;
	char *m_description;
	char *m_creator_name;
	char *m_creator_nickname;
	List<LevelPack_Level> m_levels;
};

#endif