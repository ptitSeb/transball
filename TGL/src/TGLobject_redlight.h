#ifndef TGL_OBJECT_REDLIGHT
#define TGL_OBJECT_REDLIGHT

class TGLobject_redlight : public TGLobject {
public:
	TGLobject_redlight(float x,float y);
	virtual ~TGLobject_redlight();

	virtual bool is_a(char *n);
	virtual bool is_a(Symbol *n);
	virtual const char *get_class(void);

	virtual void draw(GLTManager *GLTM);

private:
};

#endif