#include <iostream>
#include <string>
#include "getValues.h"
using namespace std;


 unsigned int getValues::get_power_ten(short count)
{		
	unsigned int value = 1;

	for(short i = 1;i<count;i++)
	{
 		value *=10;
	}
	return value;
}

 short getValues::getLenght(char* word)
{
	short i = 0;
	while(word[i] != '\0' && word[i] != ' ')
	{
		i ++;
	}
	return i;
}

 void getValues:: getIntegerValues(vector <char *> vec,unsigned int *arr,short &iter)
{	
	const short size = vec.size();

	for(short i = 0;i < size;i++)
	{

		short count = 0;
		short number = getLenght(vec[i]);
		short lenght = number;
		unsigned int value = 0;
		char *buff = vec[i];

		while(buff[count] != '\0' && count < lenght)
		{
		    value += ((unsigned int)buff[count] - (unsigned int)'0')*get_power_ten(number);

			number--;
			count ++;
		}
		arr[iter] = value;
		iter++;
	}
}

 void getValues:: get_Values(string str,unsigned int* arr,short &iter,bool bad)
{
	short i = 0;
	short m = 0;
    short j = 0;

	vector <char *> vec;
	char *buff = new char[10];
	const short size = str.size();

	if(bad)
	{
		while(str[i] !='\0' && i < size-1)
		{
  		 if(str[i] == ' ' || i == 0)
			{
				if(i != 0) i++;
				m = 0;
				while(str[i] != '/')
				{
					buff[m] = str[i];
					i++;
					m++;
				}
				buff[m] = '\0';
				vec.push_back(buff);
				buff = new char[10];
			}
			i++;
		}
 	}

 	else if(bad == false)
 	{
 		while(str[i] != '\0' && i < size-1)
 		{
 			if(str[i] == ' ' || i == 0)
 			{
 				if(i != 0) j = i+1;
 				else 	   j = i;
 				m = 0;
 				while(str[j] != ' ' && str[j] != '\0' && j < size-1)
 				{
 					buff[m] = str[j];
 					m++;
 					j++;
 				}
 				buff[m] = '\0';
 				vec.push_back(buff);
 				buff = new char[10];
 			}
 			i++;
 		}
 	}

	getIntegerValues(vec,arr,iter);
}

   int getValues::t_power(int count)
	{
		if(count == 1)
			return 1;

		int val = 1;
		for(int i = 0;i<count;i++)
		{
			val *=10;
		}
		return val;
	}

	float getValues::t_power_2(int count)
	{
		if(count == 1)
			return 0.1;
		else if(count == 2)
			return 0.01;
		else 
			return 0.1;
	}

   int  getValues::getCount(char *number)
	{
		short count = 0;
		short i = 0;

		if(number[0] !='-')
			i = 0;
		else
			i = 1;

		while(number[i] != '.' && number[i] != '\0' && number[i] != ' ' && number[i] != '/') 
		{
			i++;
		}
		count = i;

		return count;
	}

   void  getValues::getCountBefore(char *number, int &pointer, int &count)
	{
		 count = 0;
		 pointer = 0;

		while(number[pointer] != '.')
			pointer++;
		int i  = pointer;
		if(number[pointer+1] != ' ' && number[pointer+2] != ' ')
			count = 2;
		else 
			count = 1;
	}

   float  getValues::getValueAfterPoint(char *number)
	{
		char symvol;
		float val = 0.0f;
		float result = 0.0f;

		short count = getCount(number);

		if(number[0] != '-')
			symvol = '\0';
		else if(number[0] == '-')
			symvol = '-';

		for(int j = 0;j<count;j++)
		{
			for(char i = '0';i <'9'+1;i++)
			{
				if(symvol == '\0')
				{
					if(number[j] == i)
					{
						val = (float)(i-'0')*(float)t_power(count-j-1);
						result +=val;
					}
				}
				else if(symvol != '\0')
				{
					if(number[j+1] == i)
					{
						val = (float)(i-'0')*(float)t_power(count-j-1);
						result +=val;
					}
				}
			}
		}


			if(symvol == '\0')
			{
				return result;
			}
			else 
			{
				return -result;
			}

	}

  float getValues::getValueBeforePoint(char *number)
	{
		char symvol;
		float val = 0.0f;
		float result = 0.0f;

		int count = 0;
		int pointer = 0;

		if(number[0] != '-')
			symvol = '\0';
		else if(number[0] == '-')
			symvol = '-';

		getCountBefore(number,pointer,count);


		for(int j = pointer+1;j<count+pointer+1;j++)
		{
			for(char i = '0';i < '9'+1;i++)
			{
					if(number[j] == i)
					{
						val = (float)(i-'0')*(float)t_power_2(j-pointer);
						result +=val;
					}
			}
		}
		
		if(symvol == '-')
			return -result;
		else 
			return result;
	}

