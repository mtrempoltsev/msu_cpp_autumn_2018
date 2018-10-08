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

size_t matrixSumByRows(int** matrix, const size_t& matrixSize)
{
    Timer t;
    size_t sum = 0;
    for (size_t i = 0; i < matrixSize; ++i)
    {
        for (size_t j = 0; j < matrixSize; ++j)
        {
            sum += matrix[i][j];
        }
    }
    return sum;
}

int main()
{
    const size_t matrixSize = 10000;
    int** matrix = new int* [matrixSize];
    for (size_t i = 0; i < matrixSize; ++i)
    {
        matrix[i] = new int[matrixSize];
        for (size_t j = 0; j < matrixSize; ++j)
        {
            matrix[i][j] = i + j + i * j;
        }
    }

    std::cout << matrixSumByRows(matrix, matrixSize) << std::endl;

    return 0;
}		
