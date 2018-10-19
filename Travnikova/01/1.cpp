#include <iostream>
#include "numbers.dat"

using namespace std;

enum { MAX_NUMBER = 100000 };

char is_prime(int x) {
	if (x == 2) {
		return true;
	}
	for (int i = 2; i * i <= x; i++) {
		if (x % i == 0) {
			return 0;
		}
	}
	if (x < 2) {
		return false;
	}
	return true;
}

int *array_first_pos(int size, int size_data, const int *data)
{
	int *pos_first = (int*)malloc(size * sizeof(*pos_first));
	for (int i = 0; i < size; i++) {
		pos_first[i] = -1;
	}
	for (int i = 0; i < size_data; i++) {
		if (pos_first[data[i]] == -1) {
			pos_first[data[i]] = i;
		}
	}
	return pos_first;
}

int *array_last_pos(int size, int size_data, const int *data)
{
	int *pos_last = (int*)malloc(size * sizeof(*pos_last));
	for (int i = 0; i < size; i++) {
		pos_last[i] = -1;
	}
	for (int i = 0; i < size_data; i++) {
		pos_last[data[i]] = i;
	}
	return pos_last;
}

char *array_primes(int size, int size_data, const int *data)
{
	char *primes = (char*)malloc(size);
	for (int i = 0; i < size_data; i++) {
		primes[data[i]] = is_prime(data[i]);
	}
	return primes;
}

int main(int argc, char **argv)
{

	if ((argc & 1) == 0 || argc == 1) {
		return -1;
	}
	int *pos_first = array_first_pos(MAX_NUMBER + 1, Size, Data);
	int *pos_last = array_last_pos(MAX_NUMBER + 1, Size, Data);
	char *primes = array_primes(MAX_NUMBER + 1, Size, Data);
	for (int i = 1; i < argc; i += 2) {
		int first = atoi(argv[i]);
		int last = atoi(argv[i + 1]);
		int ans = 0;
		if ((pos_first[first] != -1) && (pos_last[last] != -1) && (pos_first[first] <= pos_last[last])) {
			for (int j = pos_first[first]; j <= pos_last[last]; j++) {
				if (primes[Data[j]]) {
					ans++;
				}
			}
		}
		cout << ans << endl;
	}
	free(pos_first);
	free(pos_last);
	free(primes);
	return 0;
}
