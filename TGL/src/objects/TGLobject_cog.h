#ifndef TGL_OBJECT_COG
#define TGL_OBJECT_COG

class TGLobject_cog : public TGLobject {
public:
	TGLobject_cog(float x,float y, int ao);
	TGLobject_cog(float x,float y, int ao, int dir);
	virtual ~TGLobject_cog();

	virtual bool is_a(char *n);
	virtual bool is_a(Symbol *n);
	virtual const char *get_class(void);

	virtual bool cycle(VirtualController *k,class TGLmap *map,GLTManager *GLTM,SFXManager *SFXM,int sfx_volume);
	virtual bool editor_cycle(TGLmap *map,GLTManager *GLTM);
	virtual void draw(GLTManager *GLTM);

	void set_direction(int d) {
		m_direction = d;
	}
	
	int get_direction() {
		return m_direction;
	}
	
private:
	int m_direction;
};

#endif