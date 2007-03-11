#ifndef TGL_OBJECT_CANNON_LEFT
#define TGL_OBJECT_CANNON_LEFT

class TGLobject_cannon_left : public TGLobject_enemy {
public:
	TGLobject_cannon_left(float x,float y,int type);	// 0 : rock, 1: techno
	virtual ~TGLobject_cannon_left();

	virtual bool is_a(char *n);
	virtual bool is_a(Symbol *n);
	virtual const char *get_class(void);

	virtual bool cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume);
	virtual void draw(GLTManager *GLTM);

private:
	int m_type;
};

#endif