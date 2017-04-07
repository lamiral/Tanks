#include "map.h"
#include "read_lvl_from_file.h"

class map_controller{
	map *m;
public:
	const char* get_lvl_str(int);
	void create_map(int);
	void draw_map(const char *filepath);
};