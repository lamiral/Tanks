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