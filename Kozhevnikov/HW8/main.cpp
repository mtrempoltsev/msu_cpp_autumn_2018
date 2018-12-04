#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> lock;

const int reps = 500000;

void ping() {
    int c = reps;

    while (c > 0) {
        if (lock.load() == 0) {
            c--;
            std::cout << "ping" << std::endl;
            lock.store(1);
        }
    }
}

void pong() {
    int c = reps;
    while (c > 0) {
        if (lock.load() == 1) {
            c--;
            std::cout << "pong" << std::endl;
            lock.store(0);
        }
    }
}

int main() {
    lock.store(0);
    std::thread t1(ping);
    std::thread t2(pong);

    t1.join();
    t2.join();

    return 0;
}