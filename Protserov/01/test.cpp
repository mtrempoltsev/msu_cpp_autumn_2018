#include <chrono>
#include <iostream>

using std::cout;
using std::endl;

class Timer
{
    using clock_t = std::chrono::high_resolution_clock;
    using microseconds = std::chrono::microseconds;
public:
    Timer(): start_(clock_t::now()) {}
    ~Timer()
    {
        const auto finish = clock_t::now();
        const auto us =
            std::chrono::duration_cast<microseconds>(finish - start_).count();
        std::cout << us << " us" << std::endl;
    }
private:
    const clock_t::time_point start_;
};

int rand_range(int low, int high)
{
    int irandom = rand();
    double drandom = irandom / (RAND_MAX + 1.);
    return (int)(drandom * (high - low)) + low;
}

int main()
{
    constexpr size_t mxsize = 10000;
    constexpr int lowrand = -5000;
    constexpr int highrand = 5000;

    unsigned long long sum = 0;

    int** matrix = new int* [mxsize];
    for (unsigned i = 0; i < mxsize; ++i) {
        matrix[i] = new int[mxsize];
        for (unsigned j = 0; j < mxsize; ++j) {
            matrix[i][j] = rand_range(lowrand, highrand);
        }
    }
    sum = 0;
    {
        Timer by_rows;
        for (unsigned  i = 0; i < mxsize; ++i) {
            for (unsigned j = 0; j < mxsize; ++j) {
                sum += matrix[i][j];
            }
        }
    }
    cout << sum << endl;
    sum = 0;
    {
        Timer by_columns;
        for (unsigned  i = 0; i < mxsize; ++i) {
            for (unsigned j = 0; j < mxsize; ++j) {
                sum += matrix[j][i];
            }
        }
    }
    cout << sum << endl;
    for (unsigned  i = 0; i < mxsize; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}
