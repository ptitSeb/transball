#ifndef TGL_OBJECT
#define TGL_OBJECT

// #define PRECISION_FACTOR	256

/*

  Note: 
  This game does NOT use any floating point number in its engine calculations, since FP rounding works
  different in each different processor and architecture. Thus, to ensure that the game dynamics stays
  exactly the same in any computer, I only use integers.

  Note 2:
  It remains to be tested what happens with 64bit computers. May be I have to specify all along the code
  that I want 32bit integers.

  */ 

class TGLobject {
public:
	TGLobject(float x,float y);
	virtual ~TGLobject();

	virtual void draw(GLTManager *GLTM);
	virtual bool cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume);
	virtual bool editor_cycle(TGLmap *map,GLTManager *GLTM);

	void exclude_for_collision(TGLobject *o);
	void reconsider_for_collision(TGLobject *o);
	bool check_collision(TGLobject *o);

	bool collision(TGLobject *o);

	void set_controller(int c);
	int get_controller(void);

	GLTile *get_last_tile(void);
	GLTile *get_last_mask(void);

	float get_x(void);
	float get_y(void);
	int get_angle(void);
	float get_scale(void);

	void set_x(float x);
	void set_y(float y);

	int get_state(void);
	void set_state(int s);

	int get_cycle(void);
	void set_cycle(int s);

	int get_animation_offset(void);
	void set_animation_offset(int o);

	virtual bool is_a(char *n);
	virtual bool is_a(Symbol *n);
	virtual const char *get_class(void);

protected:
	int m_controller;	// identifies which controller from the list is it taking input from
	int m_state;
	int m_cycle;
	float m_x,m_y;
	int m_angle;
	float m_scale;
	int m_animation_offset;
	GLTile *m_last_tile;
	GLTile *m_last_mask;

	List<TGLobject> m_excluded_from_collision;
//	GLTileCache *m_transformation_cache[360];		// Note: this assumes that the "m_scale" parameter wont change!!
};

#endif