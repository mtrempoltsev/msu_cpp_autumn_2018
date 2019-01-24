#include <thread>
#include <iostream>
#include <atomic>
using namespace std;

const static size_t total_steps = 1000000;
static atomic<size_t> current_step{0};

void ping_pong(size_t whose_step) {
	for(;total_steps > current_step;) {
		if(!(current_step % 2) && !whose_step) {
			cout << "ping\n";
			current_step++;
		}
		if(current_step % 2 && whose_step) {
			cout << "pong\n";
			current_step++;
		}
	}
}

int main() {
	thread thr1(ping_pong, 0);
	thread thr2(ping_pong, 1);
	thr1.join();
	thr2.join();
	return 0;
}