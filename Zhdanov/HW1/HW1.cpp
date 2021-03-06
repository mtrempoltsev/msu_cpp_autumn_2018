#include <iostream>
#include "stdio.h"
#include "math.h"
#include "numbers.dat"
using namespace std;

struct range //начальный и конечный индексы
{
	int start;
	int end;
};

range findindex(int first_int,int last_int) //поиск начального и конечного индексов
{
	range cur_range;
	cur_range.start = 0;
	cur_range.end = 0;
	int i = 0;
	if (Data[Size-1] < last_int || Data[Size-1] < first_int)
	{
		cur_range.start = -1;
		cur_range.end = -1;
		return cur_range;
	}
	else
	{
		while (Data[i] < first_int)
		{
			if (Data[i+1] == first_int)
				cur_range.start = i+1;
			++i;
		};
		while (Data[i] <= last_int)
		{
			if (Data[i] == last_int)
				cur_range.end = i;
			++i;
		};
		return cur_range;
	};
};

bool* primenums(int from_num,int to_num) //массив всех простых чисел
{
	bool* cur_primenums = (bool*)malloc((to_num - from_num + 1) * sizeof(bool));
	int i = 0;
	int shift = 0;
	if (from_num == 1)
	{
		cur_primenums[0] = false;
		++i;
		++shift;
	}
	for (i; i < to_num - from_num+1; ++i)
	{
		bool isprime = true;
		for (int j = 2; j < sqrt(from_num + i) + 1; ++j)
			isprime = isprime && (((from_num + i) % j != 0) || ((from_num + i) == j));
		cur_primenums[i] = isprime;
	}
	return cur_primenums;
};

int counter(range cur_range, bool* cur_primenums, int first_int, int last_int) //счетчик простых чисел
{
	int ctr = 0;
	int j = cur_range.start;
	if (cur_range.start == -1)
	{
		ctr = -1;
		return ctr;
	}
	else
	{
		for (int i = 0; i < last_int - first_int+1; ++i)
		{
			if (cur_primenums[i])
			{
				while (Data[j] < i + first_int)
					++j;
				while (Data[j] == i + first_int)
				{
					++j;
					++ctr;
				}
			}
		}
		return ctr;
	}
};

int calcfunc(int first_int, int last_int) //функция вычисления колличества простых чисел
{
	bool* cur_primenums = primenums(first_int, last_int);
	range cur_range = findindex(first_int, last_int);
	int ctr = counter(cur_range, cur_primenums, first_int, last_int);
	free(cur_primenums);
	return ctr;
};

int main(int argc, char* argv[])
{
	if ((argc % 2 != 0) && (argc != 1))
	{
		for (int i = 1; i < argc; i = i + 2)
		{
			int first_int = atoi(argv[i]);
			int last_int = atoi(argv[i + 1]);
			cout << calcfunc(first_int, last_int) << endl;
		}
		return 0;
	}
	return -1;
}