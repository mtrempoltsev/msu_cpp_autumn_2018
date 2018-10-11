#include <chrono>
#include <iostream>
 
enum { BUF_SIZE = 10000 };
 
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
	int **buf = new int *[BUF_SIZE];
	for (int i = 0; i < BUF_SIZE; i++) {
		buf[i] = new int[BUF_SIZE];
	}
	int sum = 0;
	Timer timer;
	for (int j = 0; j < BUF_SIZE; j++) {
		for (int i = 0; i < BUF_SIZE; i++) {
			sum += buf[i][j];
		}
	}
	for (int i = 0; i < BUF_SIZE; i++) {
		delete[] buf[i];
	}
	delete[] buf;
	std::cout << sum << std::endl;
	return 0;
}
