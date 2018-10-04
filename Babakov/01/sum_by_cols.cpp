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

int main() {
    Timer t;
    int SIZE = 1400;
    int matr[SIZE][SIZE];
    long int sum = 0;
    for (int j = 0; j < SIZE; j++)
        for (int i = 0; i < SIZE; i++) {
            matr[i][j] = rand() % 100 - 50;
        }
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            sum += matr[j][i];
    //std::cout << RAND_MAX;
    std::cout << "Sum equals "<< sum <<", time - ";
}