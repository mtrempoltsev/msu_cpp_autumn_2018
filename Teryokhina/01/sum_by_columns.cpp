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
    const size_t matrixSize = 10000;
    int sum = 0;

    int** matrix = new int*[matrixSize];

    for(size_t i = 0; i < matrixSize; ++i)
        matrix[i] = new int[matrixSize];

    for(size_t i= 0; i < matrixSize; ++i)
        for(size_t j = 0; j < matrixSize; ++j)
            matrix[i][j] = i*matrixSize + j;

    {
        Timer timer;
        for(size_t i = 0; i < matrixSize; ++i)
            for(size_t j = 0; j < matrixSize; ++j)
                sum += matrix[j][i];
    }

    std::cout << "Sum: "<< sum <<", time - ";

    for(int i = 0; i < matrixSize; ++i)
        delete[] matrix[i];
    delete[] matrix;
    return 0;
}