#include <iostream>
#include <cmath>
#include "numbers.dat"

int search_index(int input_range, int flag) {
	for (int i = 0; i < Size; ++i)
		if (Data[i] == input_range) {
			if (!flag)
				return i;
			else {
				int close_range = i;
				while ((close_range + 1 < Size) && (Data[close_range] == Data[close_range + 1]))
					close_range++;
				return close_range;
			}
		}
	return -1;
}

int arg(int index_arg, char *argv[], int flag) {
	int input_range = std::atoi(argv[index_arg]);
	return search_index(input_range, flag);
}

bool is_prime(int number) {
	if (number == 1)
		return false;
	for (int divisor = 2; divisor <= sqrt(number); ++divisor)
		if (number % divisor == 0)
			return false;
	return true;
}

int prime_range(int range_flag1, int range_flag2) {
	int prime_numbers = 0;
	for (int i = range_flag1; i < range_flag2; ++i)
		if (is_prime(Data[i]))
			prime_numbers++;
	return prime_numbers;
}

int main(int argc, char *argv[]) {
	try {
		if ((argc == 1) || (!(argc % 2)))
			throw -1;
		int range_flag1, range_flag2;
		for (int i = 1; i < argc; i += 2) {
			range_flag1 = arg(i, argv, 0);
			range_flag2 = arg(i + 1, argv, 1);
			if ((range_flag1 == -1) || (range_flag2 == -1) || (range_flag1 > range_flag2)) {
				std::cout << "0";
			}
			else
				std::cout << prime_range(range_flag1, range_flag2) << std::endl;
		}
	}
	catch (...) {
		std::cout << "-1";
		return -1;
	}
	return 0;
}
