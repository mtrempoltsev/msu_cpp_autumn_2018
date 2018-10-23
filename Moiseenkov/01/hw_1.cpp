#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "numbers.dat"
using namespace std;


int search_left(const int obj)
{
    int left_index = 0;
    int right_index = Size;
    int mid;
    while (right_index - left_index > 1)
    {
	mid = (right_index + left_index) / 2;
        if (Data[mid] >= obj)
            right_index = mid;
	else
            left_index = mid;
    }
    if (obj == Data[left_index])
	return left_index;
    else if (obj == Data[right_index])
	return right_index;
    else
    return -1;
}


int search_right(const int obj)
{
    int left_index = 0;
    int right_index = Size;
    int mid;
    while (right_index-left_index > 1)
    {	
	mid = (right_index + left_index) / 2;
	if (Data[mid] <= obj)
            left_index = mid;
	else
            right_index = mid;
    }
    if (obj == Data[left_index])
	return left_index;
    else if (obj == Data[right_index])
	return right_index;
    else
    return -1;
}

void prime_numbers(bool* array,const int max)
{
    array[0]=array[1]=false;
    for (int i = 2; i <= max; i++)
	array[i] = true;
    for (int i = 4; i <= max; i++)
    {
	for (int k = 2; k <= int(sqrt(i)); k++)
        {
            if (i % k==0)
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
    int count = 0, a, b;
	
    if ((argc-1)%2!=0 || argc == 1)
	return -1;
    else
	for (int i = 1; i < argc; i += 2)
        {
            a = atoi(argv[i]);
            b = atoi(argv[i + 1]);
            int right_index = search_right(b);
            int left_index = search_left(a);
			
            if (left_index != -1 && right_index != -1)
            {
		for (int i = left_index; i <= right_index; i++)
		count += prime[Data[i]];
            }
			
            cout << count << endl;
            count = 0;
	}
    free(prime);
    return 0;
}