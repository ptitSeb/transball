#ifndef TGL_REPLAY
#define TGL_REPLAY

// one keyframe every 50 standard frames
#define KEYFRAME_PERIOD		50	

class TGLreplay_object_position {
public:
	~TGLreplay_object_position();

	char *m_name;
	float m_x,m_y,m_speed_x,m_speed_y;
	int m_a;
};


class TGLreplay_node {
public:
	bool m_keyframe;				// If "true", then the "m_objects" array contains a dump of all the objects in the game
	List<VirtualController> m_input;
	List<TGLreplay_object_position> m_objects;
};


class TGLreplay {
public:
	TGLreplay(FILE *fp);	// load a replay
	TGLreplay(char *map);	// create a blank replay
	~TGLreplay();

	void add_player(char *player_name);

	void store_cycle(List<VirtualController> *m_input,List<TGLobject> *m_objects);
	bool execute_cycle(List<VirtualController> *m_input,List<TGLobject> *m_objects,List<TGLobject> *m_todelete,List<TGLobject> *m_toadd);
	void rewind(void);	

	bool save(FILE *fp);

	int get_length(void);

private:
	// Version of TRANSBALL with which it was saved:
	char *m_version;

	// Date of the beginning of the game:
	int m_day,m_month,m_year;
	int m_hour,m_minute,m_second;

	// Map:
	char *m_map;

	// Who played the game:
	List<char> m_players;

	// One node per game cycle (thus, the time is m_replay.Length()*18 miliseconds)
	List<TGLreplay_node> m_replay;
};

#endif