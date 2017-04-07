#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include "shaders/shaders.h"
#include "texture.h"
#include "SOIL.h"
#include "Player.h"
#include "shell.h"
#include "UI.h"
#include "map_controller.h"

using namespace std;

typedef unsigned int uint;

const int W = 1000;
const int H = 1000;

uint Texture;
uint VAO;
uint m_uni;

///////MAIN CLASSES

map_controller MAP_CONTROLLER;

shaders program;

UI 		*ui;

Player *player1;
Player *bot;

vector <shell*> shells;

struct pos{
	float x;
	float y;
};
////////PLAYER PARAM
float 	move_step;
int 	dir_player1;
int 	lvl;
pos 	move_p1;


//////////////UI FUNCTIONS
void draw_UI()
{
	if(ui != NULL)
	{
		ui->draw_UI(player1->player_lives,player1->player_lives);
	}
}
//////////////////////DRAW 
void draw_Map()
{	
	const char *filepath = "lvl_textures/wall_texture.png";
	MAP_CONTROLLER.draw_map(filepath);
}

void draw_bots()
{
	bot->draw_player();
}

void draw_players()
{	
	player1->draw_player();
}
/////////////////////SHADERS
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
	"	gl_Position = vec4(position.x+move.x,position.y+move.y,position.z,1.0);}";

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
    m_uni = glGetUniformLocation(program.program,uni_name);
}

void initTexture()
{
	const char *filepath = "tanks/tank2.png";
	texture text;
	Texture = text.load_texture(filepath);
}

void reshape(int x,int y)
{
	glViewport(0,0,x,y);
}

void draw_shells()
{
	for(int i = 0;i<shells.size();i++)
	 	{	
	 		if(shells[i] == NULL)
	 		{
	 			continue;
	 		}

	 		if(shells[i]->move_shell.x > 1.0 || shells[i]->move_shell.x < -1.0 || shells[i]->move_shell.y > 1.0 || shells[i]->move_shell.y < -1.0)
			{
				shells[i] = NULL;
				continue;
			}

	 			shells[i]->draw_shell(m_uni);
	 	}
}


void render()
{
	glClearColor(0.0,0.0,0.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	if(program.program > 0 )
	{	
	 	program.use_program();

	 	glUniform2f(m_uni,0,0);

	 	draw_Map();

	 	glUniform2f(m_uni,move_p1.x,move_p1.y);

	 	draw_players();

	 	glBindTexture(GL_TEXTURE_2D,0);

	 	glUniform2f(m_uni,0,0);

	 	draw_bots();

	 	glBindTexture(GL_TEXTURE_2D,0);

	 	draw_shells();

	 	draw_UI();
	}

	glutSwapBuffers();
}

void key(unsigned char k,int x,int y)
{

	if(k == 'W' || k == 'w') 	
	{
		if(move_p1.y < 1.8)  move_p1.y +=move_step;
		dir_player1 = 1;
	}

	else if(k == 'S' || k == 's') 
	{
		if(move_p1.y > -0.1)  move_p1.y -=move_step;
		dir_player1 = 3;
	}

	else if(k == 'D' || k == 'd') 
	{
		if(move_p1.x < 1.8) move_p1.x +=move_step;
		dir_player1 = 2;
	}

	else if(k == 'A' || k == 'a') 
	{
		if(move_p1.x > -0.1 )move_p1.x -=move_step; 
		dir_player1 = 4;
	}
	else if(k == ' ')
	{
		shell *sh;
		sh = new shell();

		const char *filepath = "Sprites/shell.png";
		sh->instantiate_shell(move_p1.x-0.86,move_p1.y-0.86,dir_player1,filepath);
		shells.push_back(sh);
	}
	player1->change_dir_texture(dir_player1);
}



void timer(int value = 0)
{
	render();
	glutTimerFunc(10,timer,0);
}

void init_start()
{	
	lvl = 2;

	dir_player1 = 1;

	player1 = new Player(6);
	player1->init_player(dir_player1,-0.9,-0.9);

	move_p1.x = 0.0;
	move_p1.y = 0.0;
	move_step = 0.01;

	bot = new Player(6);
	bot->init_player(1,-0.9,-0.9);

	ui = new UI;
	const char *filepath_texture_UI = "Sprites/heart.png";
	ui->init_UI(filepath_texture_UI,m_uni);
	
	MAP_CONTROLLER.create_map(lvl);
}

void gameLooping()
{
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
	glutInitWindowSize(W,H);
	glutCreateWindow("Game");

	if(glewInit() != GLEW_OK)
	{
		cout << "Error glewInit!" << endl;
	}

	initTexture();
	initShaders();
	init_start();

	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutTimerFunc(10,timer,0);

	glutMainLoop();
}

void end_game()
{
	delete player1;
	delete ui;

}

int main(int argc,char **argv)
{	
	

	glutInit(&argc,argv);
	gameLooping();

	end_game();

	return 0;
}