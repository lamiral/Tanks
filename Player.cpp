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