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
	Timer t;
	
	const size_t arraySize = 10000;
	
	int** a = new int*[arraySize];
	for(int i = 0; i < arraySize; ++i) a[i] = new int[arraySize];
	
	long long sum = 0;
	
	for (int i=0; i<arraySize; ++i)
		for (int j=0; j<arraySize; ++j)
			sum += a[i][j];
			
	std::cout<<sum<<std::endl;
	
	delete [] a;
	
	return 0;
}		
