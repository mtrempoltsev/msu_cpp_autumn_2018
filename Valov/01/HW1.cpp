
#include <iostream>
#include "numbers.dat"
using namespace std;

bool prime(int n)	//проверка: является ли простым
{
	if (n == 1)
		return 0;
	for (int i = 2; i * i <= n; i++)
	{
		if (n % i == 0)
			return 0;
	}
	return 1;
}
int find(int x, const int* arr, int Size_arr, bool direct)	// индексация простых чисел 
{
	if (direct == 1)
	{
		for (int i = 0; i < Size_arr; i++)
		{
			if (x == arr[i])
				return i;
		}
	}
	if (direct == 0)	
	{
		for (int i = Size_arr - 1; i > 0; i--)
		{
			if(x == arr[i])
				return i;
		}
	}
	return 0;
}
int number_of_prime(int a, int b)	//суммирование простых чисел
{
	int count = 0;
	int index_a = find(a, Data, Size, 1);
	int index_b = find(b, Data, Size, 0);
	if (a <= b)
	{
		if (index_a >= 0 && index_b >= 0)
		{
			for (int i = index_a; i <= index_b; i++)
			{
				if (prime(Data[i]))
					count++;
			}
		}
	}
	return count;
}
int main(int argc, char* argv[])
{
	if (argc % 2 == 1 && argc != 1)		//проверка входных данных
	{
		for (int i = 1; i < argc; i+=2)
			cout << number_of_prime(atoi(argv[i]), atoi(argv[i+1]))<<endl;
	}
	else
	{
		return -1;
	}
	return 0;
}
