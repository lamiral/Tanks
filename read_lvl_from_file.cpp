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
