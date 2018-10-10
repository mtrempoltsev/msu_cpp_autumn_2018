#include <iostream>
#include <chrono>
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
int sum_by_rows(int **array, int N)
{
    int sum = 0;
    Timer t;

    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
            sum += array[i][j];
    }
    return sum;
    //cout << "sum by rows\n" << sum << "\ntime " << endl;
}

int sum_by_cols(int **array, int N)
{
    int sum = 0;
    Timer t;
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
            sum += array[j][i];
    }
    return sum;
    //cout << "sum by cols\n" << sum << "\ntime "<< endl;

}
 int main(void)
{
    const size_t max_size = 1000;
    int **array = new int* [max_size];
    for (int count = 0; count < max_size; count++)
        array[count] = new int [max_size];
    for(int i = 0; i < max_size; i++)
    {
        for(int j = 0; j < max_size; j++)
            array[i][j] = i+j;
    }
    cout<<"sum by rows\n" << sum_by_rows(array, max_size) << endl;
    cout<<"sum by cols\n" << sum_by_cols(array, max_size)<< endl;
    for (int i = 0; i < max_size; i++)
        delete [] array[i];
    delete []array;
    return 0;
}
