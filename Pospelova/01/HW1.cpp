#include <iostream>
#include <cmath>
#include "numbers.dat"

int Search_index(int v, int flag) {
	for (int i = 0; i < Size; ++i)
		if (Data[i] == v) {
			if (!flag)
				return i;
			else {
				int j = i;
				while ((j + 1 < Size) && (Data[j] == Data[j + 1]))
					j++;
				return j;
			}
		}
	return -1;
}

int Arg(int i, char *argv[], int flag) {
	int v = std::atoi(argv[i]);
	return Search_index(v, flag);
}

bool Prime(int n) {
	if (n == 1)
		return false;
	for (int i = 2; i <= sqrt(n); ++i)
		if (n % i == 0)
			return false;
	return true;
}

int Prime_range(int range_flag1, int range_flag2) {
	int prime_numbers = 0;
	for (int i = range_flag1; i < range_flag2; ++i) {
		if (Prime(Data[i]))
			prime_numbers++;
	}
	return prime_numbers;
}

int main(int argc, char *argv[]) {
	try {
		if ((argc == 1) || (!(argc % 2)))
			throw -1;
		int range_flag1, range_flag2;
		for (int i = 1; i < argc; i = i + 2) {
			range_flag1 = Arg(i, argv, 0);
			range_flag2 = Arg(i + 1, argv, 1);
			if ((range_flag1 == -1) || (range_flag2 == -1) || (range_flag1 > range_flag2)) {
				std::cout << "0";
			}
			else
				std::cout << Prime_range(range_flag1, range_flag2) << std::endl;
		}
	}
	catch (...) {
		std::cout << "-1";
		return -1;
	}
	return 0;
}