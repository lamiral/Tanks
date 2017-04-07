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

