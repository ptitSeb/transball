#ifndef _TGL_APP
#define _TGL_APP


// This is to have a 640x480 screen:
#define PERSPECTIVE_DISTANCE	-895.8F

#define MAXLOCAL_PLAYERS	4

#define TGL_STATE_NONE	-1
#define TGL_STATE_INTRO			1
#define TGL_STATE_TITLE			2
#define TGL_STATE_LEVELPACKSCREEN	3
#define TGL_STATE_GAMESTART		4
#define TGL_STATE_PREGAME		5
#define TGL_STATE_GAME			6
#define TGL_STATE_POSTGAME		7
#define TGL_STATE_SAVEREPLAY	8
#define TGL_STATE_ENDSEQUENCE	9
#define TGL_STATE_INTERLEVEL	10
#define TGL_STATE_HOWTOPLAY		11
#define TGL_STATE_CREDITS		12

#define KEY_THRUST		0
#define KEY_SPECIAL		1
#define KEY_LEFT		2
#define KEY_RIGHT		3
#define KEY_FIRE		4
#define KEY_ATTRACTOR	5
#define KEY_PAUSE		6
#define KEY_QUIT		7

class TGLapp {
public:
	TGLapp();
	~TGLapp();

	bool cycle(KEYBOARDSTATE *k);
	void draw(int SCREEN_X,int SCREEN_Y);

	void save_configuration(void);
	void load_configuration(void);

	void MouseClick(int mx,int my);

protected:
//	int intro_cycle(KEYBOARDSTATE *k);
//	int title_cycle(KEYBOARDSTATE *k);
	int levelpackscreen_cycle(KEYBOARDSTATE *k);
	int gamestart_cycle(KEYBOARDSTATE *k);
	int game_cycle(KEYBOARDSTATE *k);
	int pregame_cycle(KEYBOARDSTATE *k);
	int postgame_cycle(KEYBOARDSTATE *k);
	int savereplay_cycle(KEYBOARDSTATE *k);
//	int endsequence_cycle(KEYBOARDSTATE *k);
//	int howtoplay_cycle(KEYBOARDSTATE *k);
//	int credits_cycle(KEYBOARDSTATE *k);

//	void intro_draw(void);
//	void title_draw(void);
	void levelpackscreen_draw(void);
	void gamestart_draw(void);
	void game_draw(void);
	void pregame_draw(void);
	void postgame_draw(void);
	void savereplay_draw(void);
//	void endsequence_draw(void);
//	void howtoplay_draw(void);
//	void credits_draw(void);

	int screen_x(int x);	/* given a coordinate in 640x480, returns the proper coordinate at the current resolution */ 
	int screen_y(int y);    /* given a coordinate in 640x480, returns the proper coordinate at the current resolution */ 

	// Fade in/out effects:
	void fade_in_alpha(float f);
	void fade_in_squares(float f,float size);
	void fade_in_triangles(float f,float size);

	void string_editor_cycle(char *editing_text,unsigned int *editing_position,unsigned int max_length,KEYBOARDSTATE *k);

	class GLTManager *m_GLTM;
	class SFXManager *m_SFXM;

	TTF_Font *m_font32,*m_font16;

	List<int> m_mouse_click_x;
	List<int> m_mouse_click_y;

	int m_screen_dx,m_screen_dy;
	int m_state;
	int m_state_cycle;
	int m_state_fading;
	int m_state_fading_cycle;
	int m_state_selection;
	TGL *m_game;

	int m_n_players;
	List<VirtualController> m_lvc;

	// configuration:
	int m_keys_configuration[MAXLOCAL_PLAYERS][8];
	int m_sfx_volume,m_music_volume;
	
	// Game state:
	int m_game_state;
	int m_game_state_cycle;
	int m_game_fade_effect;
	int m_game_replay_mode;	// 0 : no replay, 1 : saving replay, 2 : replaying replay
	class TGLreplay *m_game_replay;
	class LevelPack *m_current_levelpack;
	int m_selected_level;
	int m_selected_ship;

	// Save Replay
	char m_replay_name[64];
	unsigned int m_replay_editing_position;
	class TGLInterfaceElement *m_replay_save_button;

	// Game over:
//	int m_gameover_state;

	// Inter level:
//	int m_interlevel_state;
//	int m_interlevel_cycle;

	// How to play:
//	int m_howtoplay_state;
//	int m_howtoplay_cycle;
	TGL *m_test_game;

	// Credits:
//	int m_credits_state;
//	int m_credits_cycle;
	
	// end sequence:
//	int m_endsequence_state;
//	int m_endsequence_cycle;
	
	// Hiscores
//  ...	
};

#endif

