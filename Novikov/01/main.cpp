#include <iostream>
#include <cmath>
#include <set>
#include <stdlib.h>
#include "numbers.dat"
using namespace std;

void write_in(set<int>* simple_numbers) {
	int range = 100000;
	for(int number = 2; number <= range; number++) {
		bool is_allowed = true;
		for(int i = 2; (i <= sqrt(number)) && is_allowed; i++)
			if(number % i == 0) is_allowed = false;
		if(is_allowed) (*simple_numbers).insert(number);
	}
}

int where_is_present(int number, int start, int stop, int order, const int* Data) {
	int i;
	for(i = start; (i != stop) && ((order > 0)?(Data[i] < number):(Data[i] > number)); i += order);
	return (Data[i] == number)?(i):(-1);
}

int counting(int start, int stop, int Size, const int* Data, set<int> simple_numbers) {
	int quantity = 0;
	if((start + 1) && (stop + 1))
		for(int i = start; i <= stop; i++)
			if(simple_numbers.count(Data[i]))
				quantity++;
	return quantity;
}

int main(int argc, char* argv[]) {
	enum { DIRECT = 1, REVERSE = -1 };
	int left_border, right_border;
	//Create a set of simple numbers
	set<int> simple_numbers;
	write_in(&simple_numbers);
	//Input validation:
	if((argc == 1) || (argc % 2 == 0)) return -1;
	//For each pair of numbers that are read from the command line:
	for(int n = 1; n < argc; n += 2) {
		//Search for specified boundaries in the Data array:
		left_border = where_is_present(atoi(argv[n]), 0, Size, DIRECT, Data);
		right_border = where_is_present(atoi(argv[n + 1]), Size - 1, -1, REVERSE, Data);
		//Counting simple numbers between given boundaries:
		cout << counting(left_border, right_border, Size, Data, simple_numbers) << '\n';
	}
    return 0;
}