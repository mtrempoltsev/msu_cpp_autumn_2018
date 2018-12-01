#include <thread>
#include <iostream>
#include <atomic>
#include <mutex>

const size_t MAX_NUM = 1000000;
size_t ball_num = 0;
std::mutex m;

void ping() {
    while (ball_num < MAX_NUM) {
        m.lock();
        if (ball_num % 2 == 0) {
            std::cout << "ping" << std::endl;
            ++ball_num;
        }
        m.unlock();
    }
}

void pong() {
    while (ball_num < MAX_NUM) {
        m.lock();
        if (ball_num % 2 == 1) {
            std::cout << "pong" << std::endl;
            ++ball_num;
        }
        m.unlock();
    }
}

int main() {
    std::thread pinger(ping);
    std::thread ponger(pong);
    pinger.join();
    ponger.join();
    return 0;
}
