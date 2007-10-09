#ifndef TGL_OBJECT_LEFTDOOR
#define TGL_OBJECT_LEFTDOOR

class TGLobject_leftdoor : public TGLobject {
public:
	TGLobject_leftdoor(float x,float y,int state,int action);
	virtual ~TGLobject_leftdoor();

	virtual bool is_a(char *n);
	virtual bool is_a(Symbol *n);
	virtual const char *get_class(void);

	virtual bool cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume);
	virtual void draw(GLTManager *GLTM);

	void action(int action);
	int get_action(void) {return m_action;};

private:
	float m_original_x;
	int m_action;
};

#endif