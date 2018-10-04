#include <cstdint>
#include "timer.h"

int main(void)
{
	size_t row_size = 10000;
	size_t col_size = 10000;

	int32_t** array = new int32_t*[col_size];
	for (size_t col = 0; col < col_size; col++)
		array[col] = new int32_t[row_size];

	{
		volatile int32_t sum = 0;
		Timer T;
		
		for (size_t col = 0; col < col_size; col++)
			for (size_t row = 0; row < row_size; row++)
				sum += array[col][row];
	}


	for (size_t col = 0; col < col_size; col++)
		delete[] array[col];
	delete[] array;

	return 0;
}