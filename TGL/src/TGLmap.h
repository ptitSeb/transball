#ifndef _TGL_MAP
#define _TGL_MAP

#define	STARFIELD			256
#define STARFIELD_STARS		32

class TGLmap {
public:
	TGLmap(FILE *fp, GLTManager *GLTM);
	~TGLmap();

	bool cycle(List<class VirtualController> *lk,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume);
	void draw(int focus_x,int focus_y,int dx,int dy,GLTManager *GLTM);

	int get_dx(void) {return m_bg_dx*m_bg_cell_size;};
	int get_dy(void) {return m_bg_dy*m_bg_cell_size+STARFIELD;};

	void add_object(class TGLobject *o);
	void add_object_back(class TGLobject *o);	// Instead of adding at the end of the object list,
												// adds it at the beginning.
	void add_auxiliary_front_object(class TGLobject *o);		// adds at the end of the list
	void insert_auxiliary_front_object(class TGLobject *o);		// adds at the beginning of the list
	void add_auxiliary_back_object(class TGLobject *o);

	void remove_object(class TGLobject *o);
	void remove_auxiliary_front_object(class TGLobject *o);
	void remove_auxiliary_back_object(class TGLobject *o);
	
	bool object_exists(TGLobject *o);
	TGLobject *object_exists(char *type);
	TGLobject *object_exists(char *type,float x1,float y1,float x2,float y2);
	List<TGLobject> *get_objects(char *type);

	bool collision(TGLobject *o,float offsx,float offsy,int offs_alpha);
	bool collision_vector(TGLobject *o,float *vx,float *vy);
	TGLobject *collision_with_object(TGLobject *o);
	bool collision_with_foreground(TGLobject *o,float offsx,float offsy,int offs_alpha);

	void action(int action);

protected:
	int m_cycle;

	static void draw_glow(int triangles,float radius,float r,float g,float b,float a);

	// starfield:
	int m_nstars;
	int *m_star_x,*m_star_y;
	float *m_star_color;

	// background:
	int m_bg_cell_size;		// 64x64
	int m_bg_dx,m_bg_dy;
	GLTile **m_bg;

	// foreground:
	int m_fg_cell_size;		// 32x32
	int m_fg_dx,m_fg_dy;
	GLTile **m_fg;

	// objects
	List<TGLobject> m_fg_objects;
	List<TGLobject> m_fg_ships;	// this is just a cache of the ships (to accelerate searches)

	List<TGLobject> m_auxiliary_front_objects;	// Visual effects, etc. In general, objects that do not interfere in the gameplay
	List<TGLobject> m_auxiliary_back_objects;	// Visual effects, etc. In general, objects that do not interfere in the gameplay

};

#endif

