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
int matrixsize=10000;
int main()
{
	using matrix=int(*)[matrixsize];
	auto data = (matrix) malloc(matrixsize * matrixsize * sizeof(int));
	matrix k = (matrix) data;	
	for(int i=0;i<matrixsize;++i)
	{
		for(int j;j<matrixsize;++j)
		{
			k[i][j]=rand();
		}
	}
	Timer t;
	volatile int a=0;
	for (int i=1;i<matrixsize;++i)
	{
		for (int j=1;j<matrixsize;++j)
		{
			a += k[i][j];
		};
	};
	free(data);
	return 0;
}

