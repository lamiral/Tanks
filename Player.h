#include <GL/glew.h>
#include <iostream>

using namespace std;

typedef unsigned int uint;

class Player
{
private:
	uint Texture,player_VAO;
public:
	uint winns;
	uint player_count;
	uint player_lives;
	int type_tank;

	float player_speed;

	Player(int );
	void draw_player();
	void init_player(int ,float ,float );
	void init_player_VAO(float,float);
	const char* get_filepath_texture(int);
	void change_dir_texture(int);
};
