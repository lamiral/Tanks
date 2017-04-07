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
#include "map.h"
#include "texture.h"
void map::draw_map(const char *filepath)
{
	texture text;
	Texture = text.load_texture(filepath);

	glBindTexture(GL_TEXTURE_2D,Texture);

	int size = this->blocks.size();


	for(int i = 0;i<size;i++)
	{
		glBindVertexArray(this->blocks[i]);
			glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
		glBindVertexArray(0);
	}

	glBindTexture(GL_TEXTURE_2D,0);
}

#include "map_controller.h"

void map_controller::create_map(int number_lvl)
{
	const char *filepath_lvl = this->get_lvl_str(number_lvl);

	m = new map();
	read_lvl_from_file read(m,filepath_lvl);
	m = read.init_map(filepath_lvl);
}

void map_controller::draw_map(const char *filepath)
{
	m->draw_map(filepath);
}

const char* map_controller::get_lvl_str(int number_lvl)
{
	const char *str;

	switch(number_lvl)
	{
		case 1:str = "lvl/lvl1.txt";break;
		case 2:str = "lvl/lvl2.txt";break;
		case 3:str = "lvl/lvl3.txt";break;
		case 4:str = "lvl/lvl4.txt";break;
		case 5:str = "lvl/lvl5.txt";break;
		case 6:str = "lvl/lvl6.txt";break;
		case 7:str = "lvl/lvl7.txt";break;
		case 8:str = "lvl/lvl8.txt";break;
		case 9:str = "lvl/lvl9.txt";break;

		default :str = "lvl/lvl1.txt";
	}

	return str;
}
#include "Player.h"
#include "texture.h"

Player::Player(int type)
{
	this->player_lives = 3;
	this->player_count = 0;
	this->player_speed = 0.02;
	this->type_tank = type;
}

void Player::init_player(int dir,float pos_x,float pos_y)
{
	this->init_player_VAO(pos_x,pos_y);

	const char *filepath = this->get_filepath_texture(dir);
	texture text;
	Texture = text.load_texture(filepath);
}

void Player::draw_player()
{
	if(player_VAO <=0 || Texture <=0)
	{
		std::cout << "Player is not init!" << std::endl;
		return;
	}

	glBindTexture(GL_TEXTURE_2D,Texture);

	glBindVertexArray(player_VAO);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D,0);

}

void Player::change_dir_texture(int val)
{
	texture text;
	const char *filepath = this->get_filepath_texture(val);
	Texture = text.load_texture(filepath);
}

void Player::init_player_VAO(float pos_x,float pos_y)
{
	uint VBO,IBO;

	float map_verticles[] = 
	{
		pos_x,pos_y,0.0,			0.0,0.0,	
		pos_x,pos_y+0.1,0.0,		0.0,1.0,
		pos_x+0.1,pos_y+0.1,0.0,	1.0,1.0,
		pos_x+0.1,pos_y,0.0,		1.0,0.0
	};

	unsigned int elements[] =
	{
		0,1,2,
		0,2,3
	};

	glGenVertexArrays(1,&player_VAO);
	
	glGenBuffers(1,&VBO);
	glGenBuffers(1,&IBO);
		
	glBindVertexArray(player_VAO);

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

const char* Player::get_filepath_texture(int val)
{
	const char *filepath;
 if(this->type_tank == 1)
 {
	switch(val)
	{
		case 1:filepath = "tanks/tank2/1.png";break;
		case 2:filepath = "tanks/tank2/2.png";break;
		case 3:filepath = "tanks/tank2/3.png";break;
		case 4:filepath = "tanks/tank2/4.png";break;
	}
 }
 else if(this->type_tank == 2)
 {	
 	switch(val)
 	{
 		case 1:filepath = "tanks/turret/1.png";break;
		case 2:filepath = "tanks/turret/2.png";break;
		case 3:filepath = "tanks/turret/3.png";break;
		case 4:filepath = "tanks/turret/4.png";break;
	}
 }
 else if(this->type_tank == 3)
 {
 	switch(val)
 	{
		case 1:filepath = "tanks/tank3/1.png";break;
		case 2:filepath = "tanks/tank3/2.png";break;
		case 3:filepath = "tanks/tank3/3.png";break;
		case 4:filepath = "tanks/tank3/4.png";break;
	}
 }
 else if(this->type_tank == 4)
 {
 	switch(val)
 	{
		case 1:filepath = "tanks/tank4/1.png";break;
		case 2:filepath = "tanks/tank4/2.png";break;
		case 3:filepath = "tanks/tank4/3.png";break;
		case 4:filepath = "tanks/tank4/4.png";break;
	}
 }
 else if(this->type_tank == 5)
 {
 	switch(val)
 	{
		case 1:filepath = "tanks/tank5/1.png";break;
		case 2:filepath = "tanks/tank5/2.png";break;
		case 3:filepath = "tanks/tank5/3.png";break;
		case 4:filepath = "tanks/tank5/4.png";break;
	}
 }
 else if(this->type_tank == 6)
 {
 	switch(val)
 	{
		case 1:filepath = "tanks/tank6/1.png";break;
		case 2:filepath = "tanks/tank6/2.png";break;
		case 3:filepath = "tanks/tank6/3.png";break;
		case 4:filepath = "tanks/tank6/4.png";break;
	}
 }
 else if(this->type_tank == 7)
 {
 	switch(val)
 	{
		case 1:filepath = "tanks/tank7/1.png";break;
		case 2:filepath = "tanks/tank7/2.png";break;
		case 3:filepath = "tanks/tank7/3.png";break;
		case 4:filepath = "tanks/tank7/4.png";break;
	}
 }
	return filepath;
}
#include "read_lvl_from_file.h"

////7

read_lvl_from_file::read_lvl_from_file(map *m,const char *filepath)
{
	mp = m;

	this->count_blocks = 0;
	this->get_count_blocks(filepath);
	return;
}

void read_lvl_from_file::get_count_blocks(const char *filepath)
{
	ifstream file(filepath);

	if(!file.is_open())
	{
		cout << "Bad filepath_lvl_file!" << endl;
		return;
	}

	string str;

	int i = 0;

	while(getline(file,str))
	{
		i++;
		if(i == 7)
			{
				this->count_blocks++;
				i = 0;
			}
	}
}

uint read_lvl_from_file::init_block_vao()
{
	unsigned int VBO,IBO,VAO;

	float map_verticles[] = 
	{
		-0.1,0.2,0.0,	0.0,0.0,	
		-0.1,1.0,0.0,	0.0,1.0,
		0.0,1.0,0.0,	1.0,1.0,
		0.0,0.2,0.0,	1.0,0.0
	};

	unsigned int elements[] =
	{
		0,1,2,
		0,2,3
	};

	glGenVertexArrays(1,&VAO);

	glGenBuffers(1,&VBO);
	glGenBuffers(1,&IBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(verticles),verticles,GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elements),elements,GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));

	glBindVertexArray(0);

	return VAO;
}

map* read_lvl_from_file::init_map(const char *filepath)
{	
	int i = 0;
	int iter = 0;

	while(i < this->count_blocks)
	{
		this->get_verticles(iter,filepath);
		mp->blocks.push_back(this->init_block_vao());
		i++;
	}

	return mp;
}


void read_lvl_from_file::get_verticles(int &iter,const char * filepath)
{
	ifstream file(filepath);

	if(!file.is_open())
	{
		cout << "Bad filepath_lvl_file!" << endl;
		return;
	}

	int i = 0;
	int j = 0;

	vector <char *> char_values;

	string str;

	while(i < 7)
	{
		getline(file,str);

		if(j < iter) j++;
		else
		{
			i++;
			if(!str.empty())
			{
				this->get_str.parseSTR(str,char_values);
			}
		}

	}

	iter+=i;

	i = 0;

	if(char_values.size() <= 0)
		return;

	while(i < 20)
	{
		if(!this->chek_point(char_values[i]))
		{
			verticles[i] = this->get_float_from_str(char_values[i]);
		}
		else
		{
			verticles[i] = get_v.getValueBeforePoint(char_values[i]) + get_v.getValueAfterPoint(char_values[i]);
		}
		//cout << verticles[i] << endl;
		i++;
	}
}

float read_lvl_from_file::get_float_from_str(char *word)
{
	float result;

	result = (float)(word[0] - '0');

	return result;
}

bool read_lvl_from_file::chek_point(char *word)
{
	int i = 0;

	bool ok_pointer = false;

	while(word[i] != '\0')
	{
		if(word[i] == '.')
		{
			ok_pointer = true;
		}

		i++;
	}

	if(ok_pointer) return true;
	else 		   return false;
}
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
#include "texture.h"

unsigned int  texture::load_texture(const char *filepath)
{
	unsigned int textur;
	glGenTextures(1,&textur);
	glBindTexture(GL_TEXTURE_2D,textur);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	int width,height;

	unsigned char *image = SOIL_load_image(filepath,&width,&height,0,SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,image);

	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);

	glBindTexture(GL_TEXTURE_2D,0);

	if(textur <= 0 )
	{
		std::cout << textur << std::endl;
		return 0;
	}

	return textur;
}
#include "texture.h"
#include "UI.h"

uint UI::get_texture(const char *filepath)
{
	texture text;
	return text.load_texture(filepath);
}

void UI::init_UI(const char *filepath_heart,uint loc)
{
	Texture_heart = this->get_texture(filepath_heart);

	location_uni = loc;

	this->init_UI_buffers();
}

void UI::init_UI_buffers()
{
	uint VBO,IBO;

	float map_verticles[] = 
	{
		-0.9,0.9,0.0,		0.0,0.0,	
		-0.9,1.0,0.0,		0.0,1.0,
		-1.0,1.0,0.0,		1.0,1.0,
		-1.0,0.9,0.0,		1.0,0.0
	};

	unsigned int elements[] =
	{
		0,1,2,
		0,2,3
	};

	glGenVertexArrays(1,&VAO);
	
	glGenBuffers(1,&VBO);
	glGenBuffers(1,&IBO);
		
	glBindVertexArray(VAO);

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

void UI::draw_UI(uint lives_p1,uint lives_p2)
{
	if(VAO <=0)
		return;

	float uni_param = 0;

	for(short i = 0;i<lives_p1;i++)
	{	
		glBindTexture(GL_TEXTURE_2D,Texture_heart);

		glUniform2f(location_uni,uni_param,0);
		glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D,0);
		uni_param += 0.1;
	}

	uni_param +=1.4;

	for(short i = 0;i<lives_p2;i++)
	{	
		glBindTexture(GL_TEXTURE_2D,Texture_heart);

		glUniform2f(location_uni,uni_param,0);
		glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D,0);
		uni_param += 0.1;
	}
}
include "shaders.h"

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
#include "load_lvl_in_file.h"

load_lvl_in_file::load_lvl_in_file(const char *filepath)
{
    file.open(filepath,ios_base::app);
	if(!file.is_open()){
		cout << "Warning filepath!" << endl;
		return;
	}
}

void load_lvl_in_file::load_vao_verticles(float *verticles,int size)
{
	if(file == NULL)
		return;

	for(int i = 0;i<size;i++)
	{	
		if(i%3 == 0)
		{
			file << endl;
		}
						 file << verticles[i] << " ";
	}
}

load_lvl_in_file::~load_lvl_in_file()
{
	file.close();
}

#include <iostream>
#include <string>
#include "getValues.h"
using namespace std;


 unsigned int getValues::get_power_ten(short count)
{		
	unsigned int value = 1;

	for(short i = 1;i<count;i++)
	{
 		value *=10;
	}
	return value;
}

 short getValues::getLenght(char* word)
{
	short i = 0;
	while(word[i] != '\0' && word[i] != ' ')
	{
		i ++;
	}
	return i;
}

 void getValues:: getIntegerValues(vector <char *> vec,unsigned int *arr,short &iter)
{	
	const short size = vec.size();

	for(short i = 0;i < size;i++)
	{

		short count = 0;
		short number = getLenght(vec[i]);
		short lenght = number;
		unsigned int value = 0;
		char *buff = vec[i];

		while(buff[count] != '\0' && count < lenght)
		{
		    value += ((unsigned int)buff[count] - (unsigned int)'0')*get_power_ten(number);

			number--;
			count ++;
		}
		arr[iter] = value;
		iter++;
	}
}

 void getValues:: get_Values(string str,unsigned int* arr,short &iter,bool bad)
{
	short i = 0;
	short m = 0;
    short j = 0;

	vector <char *> vec;
	char *buff = new char[10];
	const short size = str.size();

	if(bad)
	{
		while(str[i] !='\0' && i < size-1)
		{
  		 if(str[i] == ' ' || i == 0)
			{
				if(i != 0) i++;
				m = 0;
				while(str[i] != '/')
				{
					buff[m] = str[i];
					i++;
					m++;
				}
				buff[m] = '\0';
				vec.push_back(buff);
				buff = new char[10];
			}
			i++;
		}
 	}

 	else if(bad == false)
 	{
 		while(str[i] != '\0' && i < size-1)
 		{
 			if(str[i] == ' ' || i == 0)
 			{
 				if(i != 0) j = i+1;
 				else 	   j = i;
 				m = 0;
 				while(str[j] != ' ' && str[j] != '\0' && j < size-1)
 				{
 					buff[m] = str[j];
 					m++;
 					j++;
 				}
 				buff[m] = '\0';
 				vec.push_back(buff);
 				buff = new char[10];
 			}
 			i++;
 		}
 	}

	getIntegerValues(vec,arr,iter);
}

   int getValues::t_power(int count)
	{
		if(count == 1)
			return 1;

		int val = 1;
		for(int i = 0;i<count;i++)
		{
			val *=10;
		}
		return val;
	}

	float getValues::t_power_2(int count)
	{
		if(count == 1)
			return 0.1;
		else if(count == 2)
			return 0.01;
		else 
			return 0.1;
	}

   int  getValues::getCount(char *number)
	{
		short count = 0;
		short i = 0;

		if(number[0] !='-')
			i = 0;
		else
			i = 1;

		while(number[i] != '.' && number[i] != '\0' && number[i] != ' ' && number[i] != '/') 
		{
			i++;
		}
		count = i;

		return count;
	}

   void  getValues::getCountBefore(char *number, int &pointer, int &count)
	{
		 count = 0;
		 pointer = 0;

		while(number[pointer] != '.')
			pointer++;
		int i  = pointer;
		if(number[pointer+1] != ' ' && number[pointer+2] != ' ')
			count = 2;
		else 
			count = 1;
	}

   float  getValues::getValueAfterPoint(char *number)
	{
		char symvol;
		float val = 0.0f;
		float result = 0.0f;

		short count = getCount(number);

		if(number[0] != '-')
			symvol = '\0';
		else if(number[0] == '-')
			symvol = '-';

		for(int j = 0;j<count;j++)
		{
			for(char i = '0';i <'9'+1;i++)
			{
				if(symvol == '\0')
				{
					if(number[j] == i)
					{
						val = (float)(i-'0')*(float)t_power(count-j-1);
						result +=val;
					}
				}
				else if(symvol != '\0')
				{
					if(number[j+1] == i)
					{
						val = (float)(i-'0')*(float)t_power(count-j-1);
						result +=val;
					}
				}
			}
		}


			if(symvol == '\0')
			{
				return result;
			}
			else 
			{
				return -result;
			}

	}

  float getValues::getValueBeforePoint(char *number)
	{
		char symvol;
		float val = 0.0f;
		float result = 0.0f;

		int count = 0;
		int pointer = 0;

		if(number[0] != '-')
			symvol = '\0';
		else if(number[0] == '-')
			symvol = '-';

		getCountBefore(number,pointer,count);


		for(int j = pointer+1;j<count+pointer+1;j++)
		{
			for(char i = '0';i < '9'+1;i++)
			{
					if(number[j] == i)
					{
						val = (float)(i-'0')*(float)t_power_2(j-pointer);
						result +=val;
					}
			}
		}
		
		if(symvol == '-')
			return -result;
		else 
			return result;
	}

#include <iostream>
#include <fstream>
#include <vector>
#include "getStringElements.h"

using namespace std;

	string  getStringElements::strcatV(string &str, char flag)
	{	
		string str2 = str;
		if(flag == 'v' || flag == 'f')
		{
			short i = 2;
			short j = 0;
			int size = str.size();

			while(str[i] != '\0' && j < size-1)
			{
				str2[j] = str[i];
				i++;
				j++;
			}
			str2[j] = '\0';
		}
		else if(flag == 'n')
		{
				int i = 3;
			while(str[i] != '\0')
			{
				str2[i-3] = str[i];
				i++;
			}
			str2[i-3+1] = '\0';
		}
		return str2;

	}

	bool  getStringElements::parseValues(const char *filepath, vector <char *> &mainVec,char flag)
	{	

		ifstream file(filepath);
		if(!file.is_open())
		{
			cout << "Error file!" << endl;
			return false;
		}

		vector <string> strVec;
		string str;
		int count = 0;
		if(flag == 'v')
		{
			while(getline(file,str))
			{	
				if(str[0] == 'v' && str[1] != 'n')
				{
					strVec.push_back(this->strcatV(str,flag));
				}
			}
		}
		else if(flag == 'n')
		{
			while(getline(file,str))
			{	
				if(str[0] == 'v' && str[1] == 'n')
				{
					strVec.push_back(this->strcatV(str,flag));
				}
			}
		}
		short size = strVec.size();

		this->getCharValues(strVec,mainVec);

		return true;
	}

	void getStringElements::getCharValues(vector <string> &vecSTR, vector <char *> &mainVec)
	{
		const short size = vecSTR.size();

			for(short i = 0;i<size;i++)
			{
				string str = vecSTR[i];
				this->parseSTR(str,mainVec);
			}
		}

	void getStringElements::parseSTR(string &str,vector <char *> &mainVec)
	{	
		char *arr[3];
		const short size = str.size();
		short m = 0;
		short lenght = 0;
		short count = 0;
		short j = 0;
		char *pointer = new char[20];

		for(int i = 0;i<size;i++)
			{
				if(str[i] != ' ' && str[i] != '\0')
				{
					lenght ++;
				}
				else if(str[i] == ' ' || str[i] == '\0')
				{
					j = i-lenght;
					if(str[j] == ' ') j++;
					while(str[j] != ' ')
					{
						if(str[j] == '\0') break;
						pointer[count] = str[j];
						j++;
						count ++;
					}
					pointer[count] = '\0';
					m++;
					count = 0;
					j = 0;
					lenght = 0;
					mainVec.push_back(pointer);
					pointer = new char[20];
				}
			}
			return;
	}
