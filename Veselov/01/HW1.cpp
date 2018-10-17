#include <iostream>
#include <memory>
#include <algorithm>

#include "numbers.dat"


bool *build_primes(const int size);

long long count_primes_in_interval(const int* Data, const int Size, bool *primes,
								   int first, int last);

int binary_search(const int* Data, const int Size, int target,
				  size_t l, size_t r); 


//auto primes = std::make_unique<bool[]>(Size);


bool *build_primes(const int size)
{
	bool *primes = new bool[size];
	primes[0] = primes[1] = false;

	for (size_t i = 2; i < size; i++)
		primes[i] = true;

	for (size_t i = 2; i*i < size; i++)
		if (primes[i])
			for (size_t j = i*i; j < size; j += i)
				primes[j] = false;

	return primes;
}

long long count_primes_in_interval(const int* Data, const int Size, bool *primes,
								   int first, int last)
{
	int ind = binary_search(Data, Size, first, 0 , Size - 1);

	if (ind < 0) {
		//std::cout << "Lower bound is not in the mass" << std::endl;
		return 0;
	}

	long long cnt = 0;

	while (ind < Size && Data[ind] < last) {
		if (primes[Data[ind]])
			cnt++;
		ind++;
	}

	if (ind >= Size || Data[ind] != last) {
		//std::cout << "Upper bound is not in the mass" << std::endl;
		return 0;
	} else {
		while (ind < Size && Data[ind] == last && primes[Data[ind]]) {
			cnt++;
			ind++;
		}
	}

	return cnt;
}

int binary_search(const int* Data, const int Size, int target,
				  size_t l, size_t r) 
{
	if (l == r - 1) {
		if (Data[l] == target)
			return l;
		else if (Data[r] == target)
			return r;
		else
			return -1;
	}

	size_t m = (l + r) / 2;

	if (target <= Data[m])
		return binary_search(Data, Size, target, l, m);
	else
		return binary_search(Data, Size, target, m, r);
}

int main(int argc, char* argv[]) {
	bool *primes = build_primes(Size);

	if (argc % 2 == 0 || argc < 2) {
		//std::cout << "Wrong number of command line parametrs" << std::endl;
		return -1;
	}

	long long primes_in_interval = 0;

	for (int i = 1; i < argc; i+= 2)
    {
        int l = std::atoi(argv[i]);
        int r = std::atoi(argv[i + 1]);

        if (l < 0 || r < 0) {
        	//std::cout << "Values must be integers" << std::endl;
        	return -1;
        }

        primes_in_interval = count_primes_in_interval(Data, Size, primes, l, r);
        if (primes_in_interval == -1) {
        	//std::cout << "Error counting number of primes" << std::endl;
        	return -1;
        } else {
        	std::cout << primes_in_interval << std::endl;
        }
    }

	return 0;
}