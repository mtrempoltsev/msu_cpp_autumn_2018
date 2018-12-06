#include <iostream>
#include <thread>
#include <atomic>


int main()
{
	std::atomic<int> num_of_iters{500000};

	std::thread ping([&num_of_iters](){
		while (num_of_iters > 0) {
			if (num_of_iters % 2 == 0) {
				std::cout << "ping\n";
				num_of_iters--;
			}
		}
	});

	std::thread pong([&num_of_iters](){
		while (num_of_iters > 0) {
			if (num_of_iters % 2 == 1) {
				std::cout << "pong\n";
				num_of_iters--;
			}
		}
	});

	pong.join();
	ping.join();

	return 0;
}