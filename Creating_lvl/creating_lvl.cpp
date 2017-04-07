#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include "../shaders/shaders.h"
#include "../texture.h"
#include "load_lvl_in_file.h"
#include <vector>

using namespace std;

typedef unsigned int uint;

const int W = 1000;
const int H = 1000;


uint main_VAO;

shaders 			program;
	
load_lvl_in_file 	*load;

vector <uint> 		VAOs;

texture 			Texture;

uint 				block_texture;

uint 				uni;

float 				element_verticles[20];

struct position
{
	float x;
	float y;
};

position block_position;

void initShaders()
{
	const char *vsShader =
	"#version 420 core\n"
	"layout(location = 0)in vec3 position;\n"
	"layout(location = 1)in vec2 inCoords;\n"
	"out vec2 coords;\n"
	"uniform vec2 move;\n"
	"void main(){\n"
	"	coords = vec2(inCoords.x,1-inCoords.y);\n"
	" gl_Position = vec4(position,1.0);}";


	const char *fsShader = 
	"#version 420 core\n"
	"in vec2 coords;\n"
	"uniform sampler2D samp;\n"
	"out vec4 color;\n"
	"void main(){\n"
	"	color = texture(samp,coords);}";

	program.load_shaders(vsShader,fsShader);
	program.create_program();

	
	const char *uni_name = "move";
}

void change_VAO()
{
	element_verticles[0] = block_position.x; element_verticles[1] = block_position.y; element_verticles[2] = 0.0; element_verticles[3] = 0.0; element_verticles[4] = 0.0;
	element_verticles[5] = block_position.x; element_verticles[6] = block_position.y+0.1; element_verticles[7] = 0.0; element_verticles[8] = 0.0; element_verticles[9] = 1.0;
	element_verticles[10] = block_position.x+0.1; element_verticles[11] = block_position.y+0.1; element_verticles[12] = 0.0; element_verticles[13] = 1.0; element_verticles[14] = 1.0;
	element_verticles[15] = block_position.x+0.1; element_verticles[16] = block_position.y; element_verticles[17] = 0.0; element_verticles[18] = 1.0; element_verticles[19] = 0.0;
}

void init_texture()
{	
	const char *filepath =  "k1.jpeg";
	block_texture = Texture.load_texture(filepath);
}

void init_VAO()
{
	uint VBO,IBO;

	uint elements[] = 
	{
		0,1,2,
		0,2,3
	};

	glGenVertexArrays(1,&main_VAO);

	glGenBuffers(1,&VBO);
	glGenBuffers(1,&IBO);

	glBindVertexArray(main_VAO);

	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(element_verticles),element_verticles,GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elements),elements,GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));

	glBindVertexArray(0);
}

void draw_vaos()
{
	int size = VAOs.size();

	glUniform2f(uni,0,0);

	glBindTexture(GL_TEXTURE_2D,block_texture);

	for(int i = 0;i<size;i++)
	{
		glBindVertexArray(VAOs[i]);
			glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
		glBindVertexArray(0);
	}

	glBindTexture(GL_TEXTURE_2D,0);
}

void reshape(int x,int y)
{
	glViewport(0,0,x,y);
}

void render()
{
	glClearColor(0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT);


	program.use_program();

		glBindTexture(GL_TEXTURE_2D,block_texture);

		glBindVertexArray(main_VAO);
			glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
		glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D,0);

	draw_vaos();

	glutSwapBuffers();
}

void key(unsigned char k,int x,int y)
{
 	if(k == 'W' || k == 'w')
 	{
 		if(block_position.y < 0.9) block_position.y +=0.1;
 	}
 	else if(k == 'S' || k == 's')
 	{
 		if(block_position.y >-1.0) block_position.y -=0.1;
 	}
 	else if(k == 'D' || k == 'd')
 	{
 		if(block_position.x < 0.9) block_position.x +=0.1;
 	}
 	else if(k == 'A' || k == 'a')
 	{
 		if(block_position.x >-1.0) block_position.x -=0.1;
 	}
 	else if(k == ' ')
 	{
 		load->load_vao_verticles(element_verticles,20);

 		VAOs.push_back(main_VAO);
 	}	
 	 	change_VAO();
 	 	init_VAO();
}

void timer(int value = 0)
{	
	render();
	glutTimerFunc(33,timer,0);
}

void initGL()
{
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowSize(W,H);
	glutCreateWindow("Create_lvl");

	if(glewInit() != GLEW_OK)
	{
		cout << "Error glew!" << endl;
	}

	change_VAO();

	init_VAO();
	initShaders();
	init_texture();

	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutTimerFunc(33,timer,0);

	glutMainLoop();
}

void start(const char *filepath)
{
	block_position.x = 0.0;
	block_position.y = 0.0;

	load = new load_lvl_in_file(filepath);
}

int main(int argc,char **argv)
{	
	const char *filepath = "lvl2.txt";
	
	start(filepath);

	glutInit(&argc,argv);
	initGL();

	return 0;
}