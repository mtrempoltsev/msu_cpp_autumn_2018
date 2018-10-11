#include <chrono>
#include <iostream>

class Timer
{
    using clock_t = std::chrono::high_resolution_clock;
    using microseconds = std::chrono::microseconds;
public:
    Timer()
        : start_(clock_t::now())
    {
    }

    ~Timer()
    {
        const auto finish = clock_t::now();
        const auto us = 
            std::chrono::duration_cast<microseconds>
                (finish - start_).count();
        std::cout << us << " us" << std::endl;
    }

private:
    const clock_t::time_point start_;
};

int main()
{
	const size_t arraySize = 10000;
	int** a;
	a = new int* [arraySize];
	for(size_t i=0;i<arraySize;++i)
		a[i] = new int [arraySize];

	for(size_t i=0;i<arraySize;++i)
		for(size_t j=0;j<arraySize;++j)
			a[i][j] = rand();
			
	volatile long long sum=0;
	
	{
		Timer t;
		for(size_t i=0;i<arraySize;++i)
			for(size_t j=0;j<arraySize;++j)
				sum += a[i][j];
	}
	for(size_t i=0;i<arraySize;++i)
		delete [] a[i];
	delete [] a;
	return 0;
}
