#include "texture.h"
#include "shell.h"

shell::shell()
{
	shell_VAO= 0;
	Texture = 0;

	move_shell.x = 0;
	move_shell.y = 0;

	this->speed = 0.02f;
}

void shell::shell_move()
{	
	if(Dir == 1)		move_shell.y += speed;
	else if(Dir == 2)	move_shell.x += speed;
	else if(Dir == 3)	move_shell.y -= speed;
	else if(Dir == 4)	move_shell.x -= speed;
}

void shell::draw_shell(unsigned int uni_loc)
{
	if(shell_VAO <=0 || Texture <=0)
		return;

	this->shell_move();

	glUniform2f(uni_loc,move_shell.x,move_shell.y);

	glBindTexture(GL_TEXTURE_2D,Texture);

		glBindVertexArray(shell_VAO);
			glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
		glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D,0);
}

void shell::init_move_shell(int dir)
{
	if(Dir == 1)
	{
		move_shell.x = 0.0f;
		move_shell.y = 0.01f;
	}
	else if(Dir == 2)
	{
		move_shell.x = 0.01f;
		move_shell.y = 0.0f;
	}
	else if(Dir == 3)
	{
		move_shell.x = 0.0f;
		move_shell.y = -0.01f;
	}
	else if(Dir == 4)
	{
		move_shell.x = -0.01f;
		move_shell.y = 0.0f;
	}
}

void shell::init_shell_VAO(float pos_x,float pos_y)
{
	unsigned int VBO,IBO;

	float map_verticles[] = 
	{
		pos_x,pos_y,0.0,			0.0,0.0,	
		pos_x,pos_y+0.02,0.0,		0.0,1.0,
		pos_x+0.02,pos_y+0.02,0.0,	1.0,1.0,
		pos_x+0.02,pos_y,0.0,		1.0,0.0
	};

	unsigned int elements[] =
	{
		0,1,2,
		0,2,3
	};

	glGenVertexArrays(1,&shell_VAO);
	
	glGenBuffers(1,&VBO);
	glGenBuffers(1,&IBO);
		
	glBindVertexArray(shell_VAO);

	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(map_verticles),map_verticles,GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elements),elements,GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));

	glBindVertexArray(0);
}

void shell::instantiate_shell(float pos_x,float pos_y,int dir,const char *filepath)
{
	if(shell_VAO <=0)
		{
			this->init_shell_VAO(pos_x,pos_y);
			texture text;
			Texture = text.load_texture(filepath);
			Dir = dir;
			this->init_move_shell(dir);
		}
	else
		return;
}