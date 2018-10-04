#include "timer.h"

int main(void)
{
	size_t row_size = 1000;
	size_t col_size = 1000;

	int array[col_size][row_size];

	{
		Timer T;
		volatile int sum = 0;
		for (size_t col = 0; col < col_size; col++)
			for (size_t row = 0; row < row_size; row++)
				sum += array[col][row];
	}

	return 0;
}