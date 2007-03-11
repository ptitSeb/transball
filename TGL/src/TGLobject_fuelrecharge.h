#ifndef TGL_OBJECT_FUELRECHARGE
#define TGL_OBJECT_FUELRECHARGE

class TGLobject_fuelrecharge : public TGLobject {
public:
	TGLobject_fuelrecharge(float x,float y);
	virtual ~TGLobject_fuelrecharge();

	virtual bool is_a(char *n);
	virtual bool is_a(Symbol *n);
	virtual const char *get_class(void);

	virtual bool cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume);
	virtual void draw(GLTManager *GLTM);

private:
};

#endif