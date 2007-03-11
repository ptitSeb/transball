#ifndef TGL_OBJECT_TANK_TURRET
#define TGL_OBJECT_TANK_TURRET

class TGLobject_tank_turret : public TGLobject_enemy {
public:
	TGLobject_tank_turret(float x,float y,TGLobject_enemy *tank,int type);
	virtual ~TGLobject_tank_turret();

	virtual bool is_a(char *n);
	virtual bool is_a(Symbol *n);
	virtual const char *get_class(void);

	virtual bool cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume);
	virtual void draw(GLTManager *GLTM);

private:
	TGLobject_enemy *m_tank;
	int m_type;		// 0 : grey, 1: red, 2: green
};

#endif