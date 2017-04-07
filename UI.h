#include <GL/glew.h>
#include <iostream>

using namespace std;

typedef unsigned int uint;

class UI
{
private:
	uint Texture_heart,VAO;

	uint winns_p1;
	uint winns_p2;

	uint location_uni;
public:
	void draw_UI(uint,uint);
	void init_UI(const char *,uint);
	void change_UI();
	uint get_texture(const char*);
	void init_UI_buffers();
};