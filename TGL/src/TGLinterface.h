#ifndef TGL_INTERFACE
#define TGL_INTERFACE

class TGLInterfaceElement {
public:
	virtual ~TGLInterfaceElement();

	virtual bool check_status(int mousex,int mousey,int button);
	virtual void draw(void);

	int m_ID;
	bool m_enabled;
};

class TGLbutton : public TGLInterfaceElement {
public:

	TGLbutton(char *text,TTF_Font *font,float x,float y,float dx,float dy,int ID);
	virtual ~TGLbutton();

	virtual bool check_status(int mousex,int mousey,int button);
	virtual void draw(void);

	char *m_text;
	TTF_Font *m_font;
	float m_x,m_y,m_dx,m_dy;
	int m_status;
};


class TGLframe : public TGLInterfaceElement {
public:

	TGLframe(float x,float y,float dx,float dy);
	virtual ~TGLframe();

	virtual void draw(void);

	float m_x,m_y,m_dx,m_dy;
};


class TGLinterface {
public:
	static List<TGLInterfaceElement> m_elements;

	static void add_element(TGLInterfaceElement *b);
	static void reset(void);
	static int update_state(int mousex,int mousey,int button);
	static void draw(void);

	static void print_left(char *text,TTF_Font *font,float x,float y);
	static void print_center(char *text,TTF_Font *font,float x,float y);
	static void print_left(char *text,TTF_Font *font,float x,float y,float r,float g,float b);
	static void print_center(char *text,TTF_Font *font,float x,float y,float r,float g,float b);

};

#endif