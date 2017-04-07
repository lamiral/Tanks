
#ifndef MAP_H
#define MAP_H

#include <GL/glew.h>
#include <iostream>
#include <vector>

using namespace std;

class map
{
private:
	unsigned int Texture,VAO1,VAO2;
public:
	float x1_stone;
	float x2_stone;

	void draw_map(const char*);

	struct block{
		uint VAO;
		float *block_position;
		int type_block;
	};
};

#endif