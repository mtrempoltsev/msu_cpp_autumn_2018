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
    const int size = 10000;
    int* a = new int[size*size];
    int sum;
    Timer* t = new Timer;
    for(int j = 0; j < size; j++) {
        for(int i = 0; i < size; i++) {
            sum += a[i*size+j];
        }
    }
    delete t;
    std::cout << sum << std::endl;
    delete[] a;
    return 0;
}
