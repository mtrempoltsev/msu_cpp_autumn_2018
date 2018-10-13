#include <iostream>
#include <cmath>
#include "numbers.dat"
using namespace std;

bool is_integer(const char* str)
{
	size_t i = 0;
	while(str[i] >= '0' && str[i] <= '9')
		i++;
	if(str[i] != '\0')
		return false;
	return true;
}

bool is_prime(unsigned int num)
{
	unsigned int max = sqrt(num);
	if(num == 1)
		return false;
	for(unsigned int i = 2; i <= max; i++)
		if(!(num % i))
			return false;
	return true;
}
int main(int argc, char const *argv[])
{
	unsigned int left = 0;
	unsigned int right = 0;
	unsigned int pos = 0;
	unsigned int res = 0;

	if(!(argc % 2) || argc == 1)
		return -1;

	for(int i = 1; i < argc; i++)
		if(!is_integer(argv[i]))
			return -1;

	while(pos * 2 + 2 < argc)
	{
		left = atoi(argv[pos * 2 + 1]);
		right = atoi(argv[pos * 2 + 2]);
		pos++;
		res = 0;

		if(left > right)
		{
			cout << 0 << endl;
			continue;
		}

		for(unsigned int i = 0; i < Size; i++)
		{
			
			if(Data[i] > right)
			{
				cout << res << endl;
				break;
			}

			if(Data[i] >= left)
				if(is_prime(Data[i]))
					res++;	

			if(i == Size - 1)
				cout << res << endl;
		}
	}
	return 0;
}	