#ifndef TGL_OBJECT_SHIP
#define TGL_OBJECT_SHIP

class TGLobject_ship : public TGLobject {
public:
	TGLobject_ship(float x,float y,int initial_fuel);
	virtual ~TGLobject_ship();

	virtual bool is_a(char *n);
	virtual bool is_a(Symbol *n);
	virtual const char *get_class(void);

	virtual void draw(GLTManager *GLTM);

	int get_fuel(void);
	void recharge_fuel(void);

	float get_speedx(void);
	float get_speedy(void);

	void set_speedx(float speedx);
	void set_speedy(float speedy);

protected:
	class TGLobject_ball *m_ball;
	float m_speed_x,m_speed_y;
	int m_fuel,m_max_fuel;
	int m_thrust_channel;
};

#endif