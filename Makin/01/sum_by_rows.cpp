#include <chrono>
#include <iostream>
#include <random>
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
    const size_t SIZE = 20000;
    
    int **matrix = new int *[SIZE];
    
    for (int i =0; i < SIZE; ++i)
        matrix[i] = new int[SIZE];
    
    //Fill
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            matrix[i][j] = rand()%11 - 5;
        }
    }
    //Sum
    Timer t;
    volatile int result = 0;
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            result += matrix[i][j];
        }
    }
    for(int i = 0; i < SIZE; i++)
        delete []matrix[i];
    delete []matrix;
    
    std::cout<<"Result of sum = "<<result<<std::endl;
      
}
