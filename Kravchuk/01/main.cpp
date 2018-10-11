#include <chrono>
#include <iostream>
#include <string.h>

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

int main ()
{
  double *matrix = new double [10000 * 10000];
  memset (matrix, 1, 10000 * 10000 * sizeof (double));

  std::cout << "Columns first" << std::endl;

  for (int test_num = 0; test_num < 10; ++test_num)
    {
      std::cout << "Test " << test_num << std::endl;
      double sum = 0;
      int i, j;
      Timer t;
      for (i = 0; i < 10000; ++i)
        for (j = 0; j < 10000; j += 10)
          sum += matrix[i * 10000 + j] + matrix[i * 10000 + j + 1] + matrix[i * 10000 + j + 2] + matrix[i * 10000 + j + 3] + matrix[i * 10000 + j + 4] +
              matrix[i * 10000 + j + 5] + matrix[i * 10000 + j + 6] + matrix[i * 10000 + j + 7] + matrix[i * 10000 + j + 8] + matrix[i * 10000 + j + 9];
      std::cout << sum << std::endl << std::endl;
    }

  std::cout << "Rows first" << std::endl;

  for (int test_num = 0; test_num < 10; ++test_num)
    {
      std::cout << "Test " << test_num << std::endl;
      double sum = 0;
      int i, j;
      Timer t;
      for (i = 0; i < 10000; ++i)
        for (j = 0; j < 10000; ++j)
          sum += matrix[i * 10000 + j];
      std::cout << sum << std::endl << std::endl;
    }
}
