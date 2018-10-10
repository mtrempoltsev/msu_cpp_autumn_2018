#include "header.h"

int_fast64_t** get_matrix(size_t size);

int_fast64_t sum_by_rows(size_t size) 
{

	auto** matrix = get_matrix(size);

	int_fast64_t sum = 0;

	Timer t;
	for (size_t i = 0; i < size; i++)
		for (size_t j = 0; j < size; j++)
			sum += matrix[i][j];

	return sum;
}

int_fast64_t sum_by_cols(size_t size) 
{

	auto** matrix = get_matrix(size);

	int_fast64_t sum = 0;

	Timer t;
	for (size_t i = 0; i < size; i++)
		for (size_t j = 0; j < size; j++)
			sum += matrix[j][i];

	return sum;
}


int_fast64_t** get_matrix(size_t size)
{
	auto** matrix = new int_fast64_t*[size];
	for (size_t i = 0; i < size; i++) {
		matrix[i] = new int_fast64_t[size];
		for (size_t j = 0; j < size; j++) 
			matrix[i][j] = j;
	}

	return matrix;
}