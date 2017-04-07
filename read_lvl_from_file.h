#include <fstream>
#include <vector>
#include <iostream>
#include "map.h"
#include <string>
#include "ParserObjFormat/getValues.h"
#include "ParserObjFormat/getStringElements.h"

using namespace std;

typedef unsigned int uint;

class read_lvl_from_file{

public:
	float verticles[20];
	const char *filepath_lvl_file;
	int count_blocks;

	map *mp;
	getValues get_v;
	getStringElements get_str;

	read_lvl_from_file(map*,const char*);

	void create_array_vao();
	void read_from_file();
	void get_verticles(int &,const char *);
	void get_count_blocks(const char *);

	float get_float_from_str(char *);

	bool chek_point(char *);

	map* init_map( const char *);

	uint init_block_vao();
	
};