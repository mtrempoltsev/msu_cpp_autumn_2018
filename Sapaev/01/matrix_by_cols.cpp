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
    const int n = 10000;
    int **a = new int*[n];
    for(int j = 0; j < n; ++j) {
        a[j] = new int[n];
    }
    
    int x = 0;
    { // Let Timer count only this
        Timer t;
        for (int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                x += a[j][i];
            }
        }
    }
    
    for(int j = 0; j < n; ++j) {
        delete[] a[j];
    }
    delete[] a;
    
    std::cout << x << std::endl;
    return 0;
}
