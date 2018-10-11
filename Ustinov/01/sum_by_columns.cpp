#include <cstdio>
#include <chrono>
#include <iostream>

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

  const size_t arrSize = 10000;
  int** vec = new int* [arrSize];
  for(size_t i=0; i<arrSize; ++i){
      vec[i] = new int [arrSize];
  }
  for(size_t i=0; i<arrSize; ++i){
      for(size_t j=0; j<arrSize; ++j){
          vec[i][j] = i + j;
      }
  }

  int64_t sum = 0;
  {
    Timer t;
    for(size_t i=0; i<arrSize; ++i){
        for(size_t j=0; j<arrSize; ++j){
            sum += vec[i][j];
          }
        }
  }

  for(size_t i=0; i<arrSize; ++i){
      delete[] vec[i];
  }
  delete[] vec;
  cout << sum << endl;




  return 0;
}
