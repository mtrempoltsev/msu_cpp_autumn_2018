#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "numbers.dat"
using namespace std;

int bin_search_right(const int obj)
{
	int left = 0, right = Size, middle;
	while (right-left>1)
	{	
		middle = (right + left) / 2;
		if (Data[middle] <= obj)
			left = middle;
		else
			right = middle;
	}
	if (obj == Data[left])
		return left;
	if (obj == Data[right])
		return right;
	return -1;
}

int bin_search_left(const int obj)
{
	int left = 0, right = Size, middle;
	while (right - left>1)
	{
		middle = (right + left) / 2;
		if (Data[middle] >= obj)
			right = middle;
		else
			left = middle;
	}
	if (obj == Data[left])
		return left;
	if (obj == Data[right])
		return right;
	return -1;
}

void prime_numbers(bool* array,const int max)
{
	array[0]=array[1]=false;
	for (int i = 2; i <= max; i++)
		array[i] = true;
	for (int i = 4; i <= max; i++)
	{
		for (int j = 2; j<=int(sqrt(i)); j++)
		{
			if (i%j==0)
			{
				array[i] = false;
				break;
			}
		}
	}
}

int main(int argc, char *argv[])
{	
	const int max = 100000;
	bool* prime = (bool*)malloc(max+1);
	prime_numbers(prime, max);
	int counter = 0, a, b;
	
	if ((argc-1)%2!=0 || argc == 1)
		return -1;
	else
		for (int i = 1; i < argc; i += 2)
		{
			a = atoi(argv[i]);
			b = atoi(argv[i + 1]);
			int right = bin_search_right(b);
			int left = bin_search_left(a);
			
			if (left != -1 && right != -1)
			{
				for (int i = left; i <= right; i++)
					counter += prime[Data[i]];
			}
			
			cout << counter << endl;
			counter = 0;
		}
	free(prime);
	return 0;
}
