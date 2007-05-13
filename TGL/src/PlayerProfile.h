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
	int m_points;
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
	List<int> m_ships;

	int progress_in_levelpack(char *id);
	int number_of_times_completed(char *lpid,int level);
	void level_completed(char *levelpack_id,int level,TGLreplay *replay,LevelPack *lp);
	int get_besttime(char *levelpack_id,int level,int ship);
	int get_points(void);
	int get_points(char *lp_id);
	int get_accumulated_time(void);
	int get_accumulated_time(char *lp_id);
	bool has_ship(int ship);

	PlayerProfile(char *name);
	PlayerProfile(FILE *fp);
	~PlayerProfile();

	bool save(FILE *fp);
};

#endif