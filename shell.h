#include <GL/glew.h>

class shell
{
private:
	unsigned int shell_VAO,Texture;
public:
	float speed;
	int Dir;

	shell();
	void draw_shell(unsigned int);
	void instantiate_shell(float ,float ,int ,const char *);
	void init_shell_VAO(float,float);
	void init_move_shell(int);
	void shell_move(); 

	struct
	{
		float x;
		float y;
	}move_shell;
};