#include <iostream>
#include <GL/glew.h>

using namespace std;

typedef unsigned int uint;

class shaders{
private:
	uint vs,fs;
public:
	uint program;
	~shaders();
	shaders();
	void load_shaders(const char*,const char*);
	void use_program();
	bool create_program();
	void error_shader(uint &);
	void error_program(uint &);
};