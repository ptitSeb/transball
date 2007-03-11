#ifndef TRANSBALL_GL_MAP
#define TRANSBALL_GL_MAP

// for 640x480
#define PERSPECTIVE_DISTANCE	-895.8F

class ObjectPlaceHolder {
public:
	ObjectPlaceHolder(GLTile *t,Symbol *n,int x,int y) 
	{
		m_x=x;
		m_y=y;
		m_tile=t;
		m_name=n;
		m_nparameters=0;
	}

	~ObjectPlaceHolder()
	{
		m_tile=0;
		m_name=0;
	}

	int m_x,m_y;
	GLTile *m_tile;
	Symbol *m_name;
	int m_nparameters;
	int m_parameters[8];
};


class TGLmap {
	friend class TGLmapeditor;
public:
	TGLmap(int dx,int dy);
	~TGLmap();

	void cycle(void);
	void draw(int startx,int starty,int dx,int dy,GLTManager *GLTM);

	int get_dx(void) {return m_dx;};
	void set_dx(int dx);
	int get_dy(void) {return m_dy;};
	void set_dy(int dy);

	void print(int x,int y,char *text);

private:
	int m_cycle;
	int m_dx,m_dy;
	int m_background;
	GLTile **m_fg_tiles;
	List<ObjectPlaceHolder> m_fg_objects;

	TTF_Font *m_font;

};


#endif