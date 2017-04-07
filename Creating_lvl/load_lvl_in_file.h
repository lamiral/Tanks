#include <fstream>
#include <iostream>

using namespace std;

class load_lvl_in_file{

private:
	ofstream file;
public:
  	load_lvl_in_file(const char *filepath);
  	void load_vao_verticles(float *,int );
  	~load_lvl_in_file();
};