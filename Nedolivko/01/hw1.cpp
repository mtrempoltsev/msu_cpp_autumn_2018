#include "numbers.dat"
#include <iostream>

typedef int data_t;

bool is_prime(data_t n)
{
	if (n == 1)
		return false;
	for (data_t i = 2; n >= i*i; ++i)
	{
		if (n % i == 0) {
			return false;
		}
	}
	return true;
}

int find_begin(const data_t *data, data_t begin, int size)
{
	int j;
	for (j = 0; (data[j] < begin) && (j < size); j++);
	return j;
}

int main (int argc, char *argv[])
{
	try {
		if ((argc % 2) && (argc > 2))
		{
			for (int i = 1; i < argc; i = i + 2) 
			{
				int count = 0;
				data_t a = std::atoi(argv[i]);
				data_t b = std::atoi(argv[i+1]);
				if (a <= b) {
					int j = find_begin(Data, a, Size);
					bool equ = false;
					for (; (Data[j] <= b) && (j < Size); j++) 
					{
						if (equ) {
							if (Data[j] == Data[j-1]) {
								count++;
								continue;
							} else {
								equ = false;
							}
						}
						if (is_prime(Data[j])){
							equ = true;
							count++;
						}
					}				
				}
				std::cout << count << std::endl;
			}
		} else {
			throw std::invalid_argument("Wrong number of input parameters!");
		}
	}
	catch(...) {
		return -1;
	}	
	return 0;
}
