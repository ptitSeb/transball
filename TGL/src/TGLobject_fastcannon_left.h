#ifndef TGL_OBJECT_FASTCANNON_LEFT
#define TGL_OBJECT_FASTCANNON_LEFT

class TGLobject_fastcannon_left : public TGLobject_enemy {
public:
	TGLobject_fastcannon_left(float x,float y);	
	virtual ~TGLobject_fastcannon_left();

	virtual bool is_a(char *n);
	virtual bool is_a(Symbol *n);
	virtual const char *get_class(void);

	virtual bool cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume);
	virtual void draw(GLTManager *GLTM);

private:
};

#endif