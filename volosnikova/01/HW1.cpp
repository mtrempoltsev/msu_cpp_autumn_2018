#include <iostream>
#include <cmath>
#include "numbers.dat"

using namespace std;

int
prime_check(int n) {
	for (int i = 2; i * i <= n; ++i) {
		if (!(n % i)) {
			return 0;
		}
	}
	return (n == 1) ? 0 : 1;
}

int
find_first_index(const int* data, size_t size, int n) {
	for (size_t i = 0; i < size; ++i) {
		if (data[i] == n) {
			return i;
		}
	}
	return -1;
}

int
find_last_index(const int* data, size_t size, int n) {
	for (size_t i = size - 1; i >= 0; --i) {
		if (data[i] == n) {
			return i;
		}
	}
	return -1;
}

int main(int argc, char** argv) {
	if ((argc < 3) || !(argc % 2)) {
		return -1;
	}
	for (int i = 0; i < argc / 2; ++i) {
		size_t first = atoi(argv[2 * i + 1]);
 	        size_t last = atoi(argv[2 * (i + 1)]);
 	        if (first > last) {
			cout << 0;
 	  		continue;
 	        }
		int first_index = find_first_index(Data, Size, first);
		int last_index = find_last_index(Data, Size, last);
		if ((first_index == -1) || (last_index == -1))
		{
			return -1;
		}
		int cnt = 0;
		for (int i = first_index; i <= last_index; ++i)
		{
			cnt += prime_check(Data[i]);
		}
		cout << cnt << endl;
	}
	return 0;
}
