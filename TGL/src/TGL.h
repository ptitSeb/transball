#ifndef _TGL_GAME
#define _TGL_GAME


class TGL {
public:
	TGL(char *map, int ship,int sfx_volume, int music_volume, GLTManager *GLTM);
	~TGL();

	bool cycle(List<class VirtualController> *lk,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume);
	void draw(GLTManager *GLTM);

	void set_music_volume(int v);
	void set_sfx_volume(int v);

	char *get_map_name(void);
	List<TGLobject> *get_object_list(void);

	int get_game_result();

protected:
	TTF_Font *m_font;

	int m_cycle;

	int m_game_result;	// 0 : nothing, 1 : level compelted, 2 : failed

	TGLobject *m_ball;
	TGLobject_ship *m_ship;
	int m_focus_x,m_focus_y;
	char *m_map_file;
	TGLmap *m_map;

	// sound:
	int m_sfx_volume,m_music_volume;
	int m_music_channel;
};

#endif

