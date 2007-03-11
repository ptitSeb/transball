#ifndef TGL_OBJECT_PIPEHSCREEN
#define TGL_OBJECT_PIPEHSCREEN

class TGLobject_pipehscreen : public TGLobject {
public:
	TGLobject_pipehscreen(float x,float y);
	virtual ~TGLobject_pipehscreen();

	virtual bool is_a(char *n);
	virtual bool is_a(Symbol *n);
	virtual const char *get_class(void);

	virtual void draw(GLTManager *GLTM);

private:
};

#endif