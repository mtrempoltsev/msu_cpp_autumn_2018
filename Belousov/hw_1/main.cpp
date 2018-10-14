#include <iostream>
#include <cstddef>
#include <cstdlib>
#include "numbers.dat"

void prepare(bool * prime, size_t const max_size) {

	for (int i = 0; i < max_size; i++) prime[i] = true;

	for (int i = 2; ((i*i) < max_size); i++) {
		if (prime[i])
		for (int j = (i*i); j < max_size; j += i) {
			if (prime[j])
			prime[j] = false;
		}
	}

	prime[0] = false;
	prime[1] = false;

}

size_t binary_search(int const * Data, int value, size_t left, size_t right) {
	size_t middle = (left + right) / 2;

	if (left == right) 
		return left;
	
	if (Data[middle] == value)
		return middle;
	
	if (Data[middle] > value)
		return binary_search(Data, value, left, middle - 1);
	else
		return binary_search(Data, value, middle + 1, right);
}

size_t post_search(int const * Data, size_t idx, int value) {
	while (Data[idx - 1] == value)
		--idx;
	return idx;
}

size_t numbers_of_prime(bool * prime, int const * Data, size_t max_size, int first, int second) {
	size_t begin = post_search(Data, binary_search(Data, first, 0, max_size - 1), first);
	size_t end   = post_search(Data, binary_search(Data, second, 0, max_size - 1), second);
	size_t count = 0;

	if (Data[begin] != first || Data[end] != second)
		return count;

	while (Data[begin] <= second && begin < max_size) {
		if (prime[Data[begin]])
			++count;
		++begin;
	}

	return count;
}

int main(int argc, char * argv[]) {
	size_t const max_size = Size;
	bool * prime = new bool [max_size];
	prepare(prime, max_size);

	if (argc == 1)
		return -1;

	for (size_t i = 1; i < argc; i += 2) {
		int first  = std::atoi(argv[i]);

		if (i + 1 == argc)
			return -1;

		int second = std::atoi(argv[i + 1]);

		std::cout << numbers_of_prime(prime, Data, max_size, first, second) << std::endl;
	}

	delete [] prime;
}