#include <chrono>
#include <iostream>

using namespace std;

int main()
{
	const size_t arraySize = 10000;
	
	int** a = new int*[arraySize];
	for(size_t i = 0; i < arraySize; ++i)
		a[i] = new int[arraySize];
	
	volatile long long sum = 0;
	
	using clock_t = chrono::high_resolution_clock;
	using microseconds = chrono::microseconds;
	
	const auto start = clock_t::now();
		
	for (size_t i=0; i<arraySize; ++i)
		for (size_t j=0; j<arraySize; ++j)
			sum += a[i][j];
		
	const auto finish = clock_t::now();
	const auto us = chrono::duration_cast<microseconds>(finish - start).count();
	cout << us << " us" << endl;
	
	for(size_t i = 0; i < arraySize; ++i)
		delete [] a[i];   

	delete [] a;
	
	return 0;
}	
