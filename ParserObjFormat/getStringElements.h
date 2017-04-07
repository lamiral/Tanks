//////////This class take elements from string
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class getStringElements{

public:
	string strcatV(string& , char);
	bool  parseValues(const char* , vector <char *> &,char );
	void getCharValues(vector <string> &, vector <char *> &);
	void parseSTR(string &,vector <char *> &);
};


