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

  int** vec = new int* [1000];
  for(size_t i=0; i<1000; ++i){
      vec[i] = new int [1000];
  }
  for(size_t i=0; i<1000; ++i){
      for(size_t j=0; j<1000; ++j){
          vec[i][j] = i + j;
      }
  }

  int64_t sum = 0;
  {
    Timer t;
    for(size_t i=0; i<1000; ++i){
        for(size_t j=0; j<1000; ++j){
            sum += vec[i][j];
          }
        }
  }

  for(size_t i=0; i<1000; ++i){
      delete[] vec[i];
  }
  delete[] vec;
  cout << sum << endl;




  return 0;
}
