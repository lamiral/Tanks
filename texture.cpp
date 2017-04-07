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