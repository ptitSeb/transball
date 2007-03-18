#ifndef TGL_PLAYER_PROFILE
#define TGL_PLAYER_PROFILE

class PlayerProfileLevelResult {
public:
	int m_level;
	int m_ship;
	int m_best_time;
	int m_fuel_used;
	int m_shots;
	int m_impacts;
	int m_kills;
};


class PlayerProfileLPProgress {
public:
	char *m_levelpack_id;
	int m_levels_completed;
	List<PlayerProfileLevelResult> m_results;

	PlayerProfileLPProgress();
	~PlayerProfileLPProgress();
};


class PlayerProfile {
public:
	char *m_name;
	bool m_fullscreen;
	int m_resolution_x;
	int m_resolution_y;
	int m_sfx_volume;
	int m_music_volume;
	int m_n_players;
	int m_keys_configuration[MAXLOCAL_PLAYERS][8];
	List<PlayerProfileLPProgress> m_progress;

	int progress_in_levelpack(char *id);
	void level_completed(char *levelpack_id,int level,TGLreplay *replay);
	int get_besttime(char *levelpack_id,int level,int ship);

	PlayerProfile(char *name);
	PlayerProfile(FILE *fp);
	~PlayerProfile();

	bool save(FILE *fp);
};

#endif