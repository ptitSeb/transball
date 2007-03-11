#ifndef TGL_OBJECT_BIG_TANK_CANNON
#define TGL_OBJECT_BIG_TANK_CANNON

class TGLobject_big_tank_cannon : public TGLobject_enemy {
public:
	TGLobject_big_tank_cannon(float x,float y,TGLobject_enemy *enemy);
	virtual ~TGLobject_big_tank_cannon();

	virtual bool is_a(char *n);
	virtual bool is_a(Symbol *n);
	virtual const char *get_class(void);

	virtual bool cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume);
	virtual void draw(GLTManager *GLTM);

private:
	TGLobject_enemy *m_tank;
};

#endif