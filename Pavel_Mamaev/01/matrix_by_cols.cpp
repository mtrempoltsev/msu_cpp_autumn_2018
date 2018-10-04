#include <chrono>
#include <iostream>
#include <stdlib.h>
using namespace std;
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
    int** matrix1 = (int**)malloc(10000 * sizeof(*matrix1));
    for (int i = 0; i < 10000; i++){
        matrix1[i] = (int*) malloc(10000 * sizeof(**matrix1));
    }
    for (int i = 0; i < 10000; ++i){
        for (int j = 0; j < 10000; ++j){
            matrix1[i][j] = i * j;
        }
    }
    long long sum = 0;
    Timer t;
    for (int i = 0; i < 10000; ++i){
        for (int j = 0; j < 10000; ++j){
            sum += matrix1[j][i];
        }
    }
    cout << "SUM = " << sum << endl;
    for (int i = 0; i < 10000; ++i){
        free(matrix1[i]);
    }
    free(matrix1);
    return 0;
}