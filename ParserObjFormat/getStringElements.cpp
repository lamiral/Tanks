#include <iostream>
#include <fstream>
#include <vector>
#include "getStringElements.h"

using namespace std;

	string  getStringElements::strcatV(string &str, char flag)
	{	
		string str2 = str;
		if(flag == 'v' || flag == 'f')
		{
			short i = 2;
			short j = 0;
			int size = str.size();

			while(str[i] != '\0' && j < size-1)
			{
				str2[j] = str[i];
				i++;
				j++;
			}
			str2[j] = '\0';
		}
		else if(flag == 'n')
		{
				int i = 3;
			while(str[i] != '\0')
			{
				str2[i-3] = str[i];
				i++;
			}
			str2[i-3+1] = '\0';
		}
		return str2;

	}

	bool  getStringElements::parseValues(const char *filepath, vector <char *> &mainVec,char flag)
	{	

		ifstream file(filepath);
		if(!file.is_open())
		{
			cout << "Error file!" << endl;
			return false;
		}

		vector <string> strVec;
		string str;
		int count = 0;
		if(flag == 'v')
		{
			while(getline(file,str))
			{	
				if(str[0] == 'v' && str[1] != 'n')
				{
					strVec.push_back(this->strcatV(str,flag));
				}
			}
		}
		else if(flag == 'n')
		{
			while(getline(file,str))
			{	
				if(str[0] == 'v' && str[1] == 'n')
				{
					strVec.push_back(this->strcatV(str,flag));
				}
			}
		}
		short size = strVec.size();

		this->getCharValues(strVec,mainVec);

		return true;
	}

	void getStringElements::getCharValues(vector <string> &vecSTR, vector <char *> &mainVec)
	{
		const short size = vecSTR.size();

			for(short i = 0;i<size;i++)
			{
				string str = vecSTR[i];
				this->parseSTR(str,mainVec);
			}
		}

	void getStringElements::parseSTR(string &str,vector <char *> &mainVec)
	{	
		char *arr[3];
		const short size = str.size();
		short m = 0;
		short lenght = 0;
		short count = 0;
		short j = 0;
		char *pointer = new char[20];

		for(int i = 0;i<size;i++)
			{
				if(str[i] != ' ' && str[i] != '\0')
				{
					lenght ++;
				}
				else if(str[i] == ' ' || str[i] == '\0')
				{
					j = i-lenght;
					if(str[j] == ' ') j++;
					while(str[j] != ' ')
					{
						if(str[j] == '\0') break;
						pointer[count] = str[j];
						j++;
						count ++;
					}
					pointer[count] = '\0';
					m++;
					count = 0;
					j = 0;
					lenght = 0;
					mainVec.push_back(pointer);
					pointer = new char[20];
				}
			}
			return;
	}
