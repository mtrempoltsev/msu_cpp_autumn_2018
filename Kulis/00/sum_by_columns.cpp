#include <iostream>
#include <chrono>

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

int main()
{
    const size_t ARR_SIZE = 10000;
    int * values = new int[ARR_SIZE*ARR_SIZE];
    int ans = 0;
    Timer t;

    for (int i = 0; i < ARR_SIZE; ++i) {
        for (int j = 0; j < ARR_SIZE; ++j) {
            ans += values[j * ARR_SIZE + i];
        }
    }

    cout << "Through columns ans =  " << ans << endl;
    delete[] values;
	return 0;
}
