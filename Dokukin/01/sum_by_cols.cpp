#include <chrono>
#include <iostream>

int main()
{
	const size_t arraySize = 10000;
	
	int** a = new int*[arraySize];
	for(int i = 0; i < arraySize; ++i) a[i] = new int[arraySize];
	
	long long sum = 0;
	
	using clock_t = std::chrono::high_resolution_clock;
	using microseconds = std::chrono::microseconds;
	
	const auto start = clock_t::now();
		
	for (int i=0; i<arraySize; ++i)
		for (int j=0; j<arraySize; ++j)
			sum += a[j][i];
		
	const auto finish = clock_t::now();
	const auto us = std::chrono::duration_cast<microseconds>(finish - start).count();
	std::cout << us << " us" << std::endl;
	std::cout<<sum<<std::endl;
	
	delete [] a;
	
	return 0;
}		
