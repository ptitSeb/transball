#ifndef TGL_INTERFACE
#define TGL_INTERFACE

class TGLInterfaceElement {
public:
	TGLInterfaceElement();
	virtual ~TGLInterfaceElement();

	virtual bool check_status(int mousex,int mousey,int button,int button_status,class KEYBOARDSTATE *k);
	virtual void draw(float alpha);
	virtual void draw(void);

	int m_ID;
	bool m_modal;	/* If any element is modal, only him has the control until it is destroyed (the rest of the interface is faded) */ 
	bool m_enabled;
	bool m_active;	/* This indictes wether the component is active or passive, */ 
					/* e.g.: TGLText and TGLframe are passive					*/ 
	bool m_to_be_deleted;

	float m_x,m_y,m_dx,m_dy;
};


class TGLText : public TGLInterfaceElement {
public:

	TGLText(char *text,TTF_Font *font,float x,float y,bool centered);
	TGLText(char *text,TTF_Font *font,float x,float y,bool centered,int ID);
	virtual ~TGLText();

	virtual void draw(float alpha);
	virtual void draw(void);

	void set_text(char *text);

	bool m_centered;
	char *m_text;
	TTF_Font *m_font;
};


class TGLbutton : public TGLInterfaceElement {
public:

	TGLbutton(char *text,TTF_Font *font,float x,float y,float dx,float dy,int ID);
	TGLbutton(GLTile *icon,float x,float y,float dx,float dy,int ID);
	virtual ~TGLbutton();

	virtual bool check_status(int mousex,int mousey,int button,int button_status,KEYBOARDSTATE *k);
	virtual void draw(float alpha);
	virtual void draw(void);

	char *m_text;
	TTF_Font *m_font;
	int m_status;
	GLTile *m_tile;
};


class TGLbuttonTransparent : public TGLbutton {
public:

	TGLbuttonTransparent(char *text,TTF_Font *font,float x,float y,float dx,float dy,int ID);

	virtual void draw(float alpha);
};



class TGLframe : public TGLInterfaceElement {
public:

	TGLframe(float x,float y,float dx,float dy);
	virtual ~TGLframe();

	virtual void draw(float alpha);
	virtual void draw(void);

};


class TGLTextInputFrame : public TGLframe {
public:

	TGLTextInputFrame(char *initial_text,int max_characters,TTF_Font *font,float x,float y,float dx,float dy,int ID);
	virtual ~TGLTextInputFrame();

	virtual bool check_status(int mousex,int mousey,int button,int button_status,KEYBOARDSTATE *k);
	virtual void draw(float alpha);
	virtual void draw(void);

	TTF_Font *m_font;
	char *m_editing;
	int m_max_characters;
	unsigned int m_editing_position;
	bool m_focus;
	int m_cycle;
	bool m_change_in_last_cycle;
};


class TGLslider : public TGLInterfaceElement {
public:

	TGLslider(float x,float y,float dx,float dy,float slider_dx,float slider_dy,int ID);
	virtual ~TGLslider();

	virtual bool check_status(int mousex,int mousey,int button,int button_status,KEYBOARDSTATE *k);
	virtual void draw(float alpha);
	virtual void draw(void);

	float m_slider_dx,m_slider_dy;
	float m_value;
};


class TGLBrowser : public TGLInterfaceElement {
public:

	TGLBrowser(TTF_Font *font,float x,float y,float dx,float dy,int ID);
	virtual ~TGLBrowser();

	void clear(void);
	void addEntry(char *);
	char *getEntry(int i);
	void deleteEntry(int i);
	void setSelected(int i);
	int getSelected(void);

	virtual bool check_status(int mousex,int mousey,int button,int button_status,KEYBOARDSTATE *k);
	virtual void draw(float alpha);
	virtual void draw(void);

	TTF_Font *m_font;
	List<char> m_entries;
	float m_slider_pos,m_slider_height;
	int m_selected,m_mouse_over;
	int m_old_mouse_x,m_old_mouse_y;

};


class TGLTileBrowser : public TGLInterfaceElement {
public:

	TGLTileBrowser(float x,float y,float dx,float dy,int ID);
	virtual ~TGLTileBrowser();

	void clear(void);
	void addEntry(GLTile *tile);
	GLTile *getEntry(int i);
	void deleteEntry(int i);
	void setSelected(int i);
	int getSelected(void);

	virtual bool check_status(int mousex,int mousey,int button,int button_status,KEYBOARDSTATE *k);
	virtual void draw(float alpha);
	virtual void draw(void);

	List<GLTile> m_entries;
	float m_slider_pos,m_slider_height;
	int m_selected,m_mouse_over;

};



class TGLConfirmation : public TGLInterfaceElement {
public:

	TGLConfirmation(char *message,TTF_Font *font,float x,float y,int ID);
	virtual ~TGLConfirmation();

	virtual bool check_status(int mousex,int mousey,int button,int button_status,KEYBOARDSTATE *k);
	virtual void draw(float alpha);
	virtual void draw(void);

	TTF_Font *m_font;
	char *m_message;
	int m_state,m_cycle;
	TGLbutton *m_ok_button,*m_cancel_button;
};





class TGLinterface {
public:
	static List<TGLInterfaceElement> m_elements;

	static void add_element(TGLInterfaceElement *b);
	static void remove_element(TGLInterfaceElement *b);
	static void remove_element(int ID);
	static void substitute_element(TGLInterfaceElement *old,TGLInterfaceElement *n);
	static void reset(void);
	static int update_state(int mousex,int mousey,int button,int button_status,KEYBOARDSTATE *k);
	static void draw(float alpha);
	static void draw(void);
	static TGLInterfaceElement *get(int ID);
	static void enable(int ID);
	static void disable(int ID);
	static bool is_enabled(int ID);

	static void clear_print_cache(void);
	static void print_left(char *text,TTF_Font *font,float x,float y);
	static void print_center(char *text,TTF_Font *font,float x,float y);
	static void print_left(char *text,TTF_Font *font,float x,float y,float r,float g,float b,float a);
	static void print_center(char *text,TTF_Font *font,float x,float y,float r,float g,float b,float a);

	// Centered in both X and Y:
	static void print_centered(char *text,TTF_Font *font,float x,float y,float r,float g,float b,float a,float angle,float scale);
};

#endif