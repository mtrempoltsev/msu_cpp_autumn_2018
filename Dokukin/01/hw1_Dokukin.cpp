#include "numbers.dat"
#include <iostream>
#include <set>
#include <memory>
#include <math.h>

using namespace std;

void sieveOfAtkin(const int& MaxPrime, set<int>& prime_numbers)
{
	auto is_prime = make_unique<bool[]>(MaxPrime + 1);

	int sqr_lim = (int)sqrt((long double)MaxPrime);
	is_prime[2] = true;
	is_prime[3] = true;
	
	int x2, y2;

	x2 = 0;
	for (int i = 1; i < sqr_lim + 1; ++i)
	{
		x2 += 2 * i - 1;
		y2 = 0;
		for (int j = 1; j < sqr_lim + 1; ++j)
		{
			y2 += 2 * j - 1;
			
			int n = 4 * x2 + y2;
			
			if ((n <= MaxPrime) && (n % 12 == 1 || n % 12 == 5))
				is_prime[n] = !is_prime[n];
			
			n -= x2;
			if ((n <= MaxPrime) && (n % 12 == 7))
				is_prime[n] = !is_prime[n];
			
			n -= 2 * y2;
			if ((i > j) && (n <= MaxPrime) && (n % 12 == 11))
				is_prime[n] = !is_prime[n];
		}
	}

	for (int i = 5; i < sqr_lim + 1; ++i)
	{
		if (is_prime[i])
		{
			int n = i * i;
			for (int j = n; j < MaxPrime + 1; j += n)
			{
				is_prime[j] = false;
			}
		}
	}
	
	prime_numbers.insert(2);
	prime_numbers.insert(3);
	prime_numbers.insert(5);
	
	for (int i = 6; i < MaxPrime + 1; ++i)
	{
		if ((is_prime[i]) && (i % 3 != 0) && (i % 5 !=  0))
		{
		   prime_numbers.insert(i);
		}
	}
}

int numOfPrimes(const int begin, const int end, const set<int>& prime_numbers, const int Size, const int* Data)
{
	int result = 0;
	int result_temp = 0; // store result if there are several "end" numbers in Data 
	
	int it = 0;
	while (Data[it] != begin)
	{
		++it;
		if (it == Size) return -1;
	}
	
	while (it < Size)
	{
		if (prime_numbers.find(Data[it]) != prime_numbers.end()) ++result;
		if (Data[it] == end) result_temp = result;
		++it;
	}
		
	return result_temp;	
}

int main(int argc, char* argv[])
{	
	if ((argc % 2 != 0) && (argc != 1))
	{ 
		const int MaxPrime = 100000;
		set<int> prime_numbers;
		sieveOfAtkin(MaxPrime, prime_numbers);
	
		for (int i = 1; i < argc; i +=2)
		{
			int begin = std::atoi(argv[i]);
			int end = std::atoi(argv[i+1]);
			
			int num_of_primes = numOfPrimes(begin, end, prime_numbers, Size, Data);
			
			if (num_of_primes > -1) cout << num_of_primes << endl;
			else return -1;			
		}
		
		prime_numbers.clear();
		return 0;
	}
	else return -1;	
}
