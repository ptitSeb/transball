#ifndef TGL_LEVEL_PACK
#define TGL_LEVEL_PACK

class LevelPack_Level {
public:
	LevelPack_Level();
	LevelPack_Level(FILE *fp);
	LevelPack_Level(class XMLNode *node);
	~LevelPack_Level();

	void load(XMLNode *node);
	void save(FILE *fp);
	void setName(char* name);

	char *m_map;
	char *m_name;
	char *m_description;
	int m_initial_fuel;
	int m_points;
};


class LevelPack {
public:
	LevelPack();
	LevelPack(FILE *fp);
	~LevelPack();

	void load(XMLNode *node);
	void save(FILE *fp);
	int get_points(int level);

	int getLevelPosition(LevelPack_Level *level);
	int getNLevels();
	LevelPack_Level *getLevel(int n);
	LevelPack_Level *getLevel(char *name);

	char *m_id;
	char *m_name;
	char *m_description;
	char *m_creator_name;
	char *m_creator_nickname;
	List<LevelPack_Level> m_levels;
};

#endif