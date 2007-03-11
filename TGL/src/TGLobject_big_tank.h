#ifndef TGL_OBJECT_BIG_TANK
#define TGL_OBJECT_BIG_TANK

class TGLobject_big_tank : public TGLobject_enemy {
public:
	TGLobject_big_tank(float x,float y);
	virtual ~TGLobject_big_tank();

	virtual bool is_a(char *n);
	virtual bool is_a(Symbol *n);
	virtual const char *get_class(void);

	virtual void hit(int power);

	virtual bool cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume);
	virtual void draw(GLTManager *GLTM);

private:
};

#endif