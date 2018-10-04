#include <chrono>
#include <iostream>

#define SIZE 2000

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

int main(){
	Timer t;
	int sum = 0;
	int **array = new int*[SIZE];
	for(int i = 0; i < SIZE; i++)
		array[i] = new int[SIZE];
	for(int i = 0; i < SIZE; i++)
		for(int j = 0; j < SIZE; j++)
			sum += array[j][i];
	for(int i = 0; i < SIZE; i++)
		delete	[]array[i];
	delete []array;
	std::cout<<sum<<std::endl;
}
