#ifndef TRANSBALL_GL_MAPEDITOR
#define TRANSBALL_GL_MAPEDITOR

// for 640x480
#define PERSPECTIVE_DISTANCE	-895.8F


class TGLmapeditor {
public:
	TGLmapeditor(void);
	~TGLmapeditor();

	bool cycle(KEYBOARDSTATE *k);
	void draw(int SCREEN_X,int SCREEN_Y);

	void save_configuration(void);
	void load_configuration(void);

	void import_ST2_map(FILE *fp);

private:

	GLTile *get_tile_by_name(char *name);
	GLTile *get_object_by_name(char *name);
	Symbol *get_object_symbol_by_name(char *name);
	void print(int x,int y,char *text);

	int m_cycle;
	int m_screen_dx,m_screen_dy;
	float zoom;

	TGLmap *m_map;
	char m_message[32];
	char m_editing_text[32];
	int m_editing_position;

	int m_state;			// 0 : editing, 1: entering load name, 2: entering save name, 3: entering import name
	bool m_mode;			// 0 : tiles, 1 : objects
	List<GLTile> m_tiles;
	List<Symbol> m_tiles_name;

	List<GLTile> m_objects_tile;
	List<Symbol> m_objects_name;

	int m_pointer_x;
	int m_pointer_y;

	TTF_Font *m_font;
	GLTManager *m_GLTM;
};

#endif