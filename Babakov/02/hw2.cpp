#include "numbers.dat"
#include <iostream>
#include <cmath>

size_t find_first(const int arr[], int num) {
	for (size_t i = 0; i < Size; i++) {
		if (arr[i] == num) {
			return i;
		}
	}
	return -1;
}

size_t find_last(const int arr[], size_t from, int num) {
	size_t nec_index = -1;
	for (size_t i = from; i < Size; i++) {
		if (arr[i] == num)
			nec_index = i;
	}
	return nec_index;
}

int Prime(int a)
{
   unsigned long i, j, bound;
   if (a == 0 || a == 1)
      return 0;
   if (a == 2 || a == 3 || a == 5)
      return 1;
   if (a%2 == 0 || a%3 == 0 || a%5 == 0)
      return 0;
   bound = sqrt((double)a);
   i = 7; j = 11;
   while (j <= bound && a%i && a%j)
   {
       i += 6; j += 6;
   }
   if (j <= bound || i <= bound && a%i == 0)
      return 0;
   return 1;
}

int prime_out (const int arr[], size_t from, size_t to) {
	int prime_am = 0;
	for (int i = from; i <= to; i++) {
		prime_am += Prime(arr[i]);
	}
	std::cout << prime_am << std::endl;
	return 0;
}

int main(int argc, char* argv[])
{
	if ((argc < 2) || !(argc % 2)){
		//std::cerr << "Wrong amount of parametres" << std::endl;
		return -1;
	}
	int pair_num = argc  / 2;
    for (size_t i = 0; i < pair_num; i++)
    {
        int start = std::atoi(argv[2*i + 1]);
        int end = std::atoi(argv[2*(i + 1)]);
        if (start > end){
        	std::cout << 0 << std::endl;
        	return 0;
        }
        size_t start_index = find_first(Data, start);
        size_t last_index = find_last(Data, start_index, end);
        if (start_index == -1 || last_index == -1)
        	return -1;
        prime_out (Data, start_index, last_index);
    }
    return 0;
}