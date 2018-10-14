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

int main(void) {
    size_t row = 1000, col = 1000;
    int sum = 0;
    int** matrix = new int*[row];
    for (size_t i = 0; i < row; i++) {
        matrix[i] = new int[col];
    }
    for (size_t i = 0; i < row; i++) {
        for (size_t j = 0; j < col; j++) {
            matrix[i][j] = i + j;
        }
    }
    {
        Timer t;
        for (size_t  j = 0; j < col; j++) {
            for (size_t i = 0; i < row; i++) {
                sum += matrix[i][j];
            }
        }
    }
    delete [] matrix;
    std::cout << "sum = " << sum << std::endl;
    return 0;
}