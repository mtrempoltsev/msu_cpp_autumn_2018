#include <iostream>
#include <cmath>
#include <set>
#include <stdlib.h>
#include "numbers.dat"

#define RANGE 100000

using namespace std;

void write_in(set<int> &prime_numbers, size_t range) {
	for(int number = 2; number <= range; number++) {
		bool is_allowed = true;
		for(int i = 2; (i <= sqrt(number)) && is_allowed; i++)
			if(number % i == 0) is_allowed = false;
		if(is_allowed) prime_numbers.insert(number);
	}
}

int where_is_present(int number, int start, int stop, int order, const int* Data) {
	int i;
	for(i = start; (i != stop) && ((order > 0)?(Data[i] < number):(Data[i] > number)); i += order);
	return (Data[i] == number)?(i):(-1);
}

int counting(int start, int stop, int Size, const int* Data, set<int> &prime_numbers) {
	int quantity = 0;
	if((start != -1) && (stop != -1))
		for(int i = start; i <= stop; i++)
			if(prime_numbers.count(Data[i]))
				quantity++;
	return quantity;
}

int main(int argc, char* argv[]) {
	enum { DIRECT = 1, REVERSE = -1 };
	int left_border, right_border;
	//Create a set of simple numbers
	set<int> prime_numbers;
	write_in(prime_numbers, RANGE);
	//Input validation:
	if((argc == 1) || (argc % 2 == 0)) return -1;
	//For each pair of numbers that are read from the command line:
	for(int n = 1; n < argc; n += 2) {
		//Search for specified boundaries in the Data array:
		left_border = where_is_present(atoi(argv[n]), 0, Size, DIRECT, Data);
		right_border = where_is_present(atoi(argv[n + 1]), Size - 1, -1, REVERSE, Data);
		//Counting simple numbers between given boundaries:
		cout << counting(left_border, right_border, Size, Data, prime_numbers) << '\n';
	}
    return 0;
}
