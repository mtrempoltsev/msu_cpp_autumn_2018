#include "numbers.dat"
#include <iostream>
#include <ctype.h>
#include <exception>

using namespace std;

bool isPrime(int k)
{
	if (k <= 1)
		return false;
	bool primeFlag = true;
	for (int i = 2; primeFlag && (i * i <= k); ++i)
	{
		primeFlag = (k % i != 0);
	}
	return primeFlag;
}

size_t findFirst(const int* data, size_t dataSize, int elem)
{
	for (size_t i = 0; i < dataSize; ++i)
	{
		if (data[i] == elem)
			return i;
	}
	throw runtime_error("Element not found");
}

size_t findLast(const int* data, size_t dataSize, int elem)
{
	for (size_t i = dataSize; i > 0; --i)
	{
		if (data[i - 1] == elem)
			return i - 1;
	}
	throw runtime_error("Element not found");
}

int main(int argc, char** argv)
{
	if (argc % 2 == 0 || argc == 1)
	{
		cout << -1 << endl;
		return -1;
	}

	for (int i = 0; i < argc / 2; ++i)
	{
		try
		{
			int firstElement = atoi(argv[2 * i + 1]);
			int lastElement = atoi(argv[2 * i + 2]);
			if (firstElement < 0 || firstElement > 100000 || lastElement < 0
					|| lastElement > 100000)
			{
				cout << -1 << endl;
				continue;
			}
			size_t start = findFirst(Data, Size, firstElement);
			size_t last = findLast(Data, Size, lastElement);
			int primeCounter = 0;
			for (size_t j = start; j <= last; ++j)
			{
				if (isPrime(Data[j]))
				{
					primeCounter++;
				}
			}
			cout << primeCounter << endl;
		}
		catch (exception& exc)
		{
			cout << 0;
		}
	}
	return 0;
}

