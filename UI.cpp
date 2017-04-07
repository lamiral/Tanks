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
