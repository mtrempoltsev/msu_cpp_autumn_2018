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
        std::cout << "Time: " << us << " us" << std::endl;
    }

    private:
    const clock_t::time_point start_;
};

int main() {
    int SIZE = 1500;
    int **matrix=new int*[SIZE];
    for (int i=0; i<SIZE; i++)
        matrix[i]=new int[SIZE];

    for (int i=0; i<SIZE; i++) 
        for (int j=0; j<SIZE; j++) {
            matrix[i][j]=i+j-100;
        }
    long long sum=0;
    Timer t;
    for (int j=0; j<SIZE; j++) 
        for (int i=0; i<SIZE; i++) {
            sum+=matrix[i][j];
        }
    cout<<"Sum= "<<sum<<endl;

    for (int i=0; i<SIZE; i++) 
        delete[]matrix[i];
    
    delete[]matrix;
    
    return 0;
}
