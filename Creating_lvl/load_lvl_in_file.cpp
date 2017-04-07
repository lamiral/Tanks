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

