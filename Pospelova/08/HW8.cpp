#include <thread>
#include <iostream>
#include <atomic>
#include <mutex>

using namespace std;

constexpr size_t max_count = 500000;
atomic<size_t> count;
mutex m;

void ping() {
	while (count < max_count){
		lock_guard<mutex> lock(m);
		if (!(count % 2)) {
			cout << "ping" << endl;
			count++;
		}
	}
}

void pong() {
	while (count < max_count){
		lock_guard<mutex> lock(m);
		if (count % 2) {
			cout << "pong" << endl;
			count++;
		}
	}
}

int main() {
	count = 0;
	thread t1(ping);
	thread t2(pong);
	t1.join();
	t2.join();
	return 0;
}
