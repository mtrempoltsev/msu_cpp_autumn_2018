#include <iostream>
#include <string>
#include <cstddef>
#include <thread>
#include <atomic>

int main() {
	size_t const N = 500000;
	std::atomic_bool is_ping(false);

	auto msg1 = [&]() {
		for (size_t i = 0; i < N; i++) {
			while (is_ping);

			std::cout << "ping" << std::endl;
			is_ping = true;
		}
	};

	auto msg2 = [&]() {
		for (size_t i = 0; i < N; i++) {
			while (!is_ping);

			std::cout << "pong" << std::endl;
			is_ping = false;
		}
	};

	std::thread t1(msg1);
	std::thread t2(msg2);

	t1.join();
	t2.join();

	return 0;
}