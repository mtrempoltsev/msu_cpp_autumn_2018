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
  int data[100][100];
  for (int i = 0; i < 100; i++)
    for (int j = 0; j < 100; j++)
      data[i][j] = i + j;
  volatile int sum = 0;

  Timer t;
  for (int i = 0; i < 100; i++)
    for (int j = 0; j < 100; j++)
      sum += data[j][i];
}
