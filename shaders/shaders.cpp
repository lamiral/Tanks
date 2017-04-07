#include "shaders.h"

shaders::shaders()
{
	this->program = 0;
	vs = 0;
	fs = 0;
}

shaders::~shaders()
{
	glUseProgram(0);
	glDeleteProgram(this->program);
}

void shaders::error_shader(uint &shader)
{
	int compile_status = 0;
	glGetShaderiv(shader,GL_COMPILE_STATUS,&compile_status);
	if(!compile_status)
	{
		char log_buff[512];
		glGetShaderInfoLog(shader,512,NULL,log_buff);
		cout << log_buff << endl;
		shader = -1;
	}
}

void shaders::error_program(uint &program)
{
	int link_status = 0;
	glGetProgramiv(program,GL_LINK_STATUS,&link_status);
	if(!link_status)
	{
		char log_buff[512];
		glGetProgramInfoLog(program,512,NULL,log_buff);
		cout << log_buff << endl;
		this->program = -1;
	}
}

void shaders::load_shaders(const char *vsShader,const char *fsShader)
{
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs,1,&vsShader,NULL);
	glCompileShader(vs);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs,1,&fsShader,NULL);
	glCompileShader(fs);

	this->error_shader(vs);
	this->error_shader(fs);
}

bool shaders::create_program()
{
	this->program = glCreateProgram();
	if(vs <= 0 || fs <= 0)
	{
		cout << "Error program!Shaders is not init!" << endl;
		return false;
	}
	glAttachShader(this->program,vs);
	glAttachShader(this->program,fs);
	glLinkProgram(this->program);

	this->error_program(this->program);

	if(this->program <=0)
		return false;
	return true;
}

void shaders::use_program()
{
	glUseProgram(this->program);
}