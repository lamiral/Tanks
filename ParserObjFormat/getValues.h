/////Class converting string elements in values
#include <vector>
#include <iostream>

using namespace std;

class getValues
{
 public:
    unsigned int get_power_ten(short );
    short getLenght(char* );
    void get_Values(string ,unsigned int* ,short& ,bool );
    void getIntegerValues(vector <char *>,unsigned int* ,short& );
    int t_power(int );
    float t_power_2(int );
    int getCount(char *number);
    void getCountBefore(char* , int& , int& );
    float getValueAfterPoint(char* );
    float getValueBeforePoint(char* );
};

