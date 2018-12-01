#include <thread>
#include <iostream>
#include <atomic>
#include <condition_variable>

const size_t MAX_NUM = 500000;
size_t ball_num = 0;
std::mutex m;
std::condition_variable cond;

void ping() {
    while (ball_num < MAX_NUM) {
        std::unique_lock<std::mutex> lock(m);
        cond.wait(lock, []{return ball_num % 2 == 0;});

        std::cout << "ping" << std::endl;
        ++ball_num;
        cond.notify_all();
    }
}

void pong() {
    while (ball_num < MAX_NUM) {
        std::unique_lock<std::mutex> lock(m);
        cond.wait(lock, []{return ball_num % 2 == 1;});
        
        std::cout << "pong" << std::endl;
        ++ball_num;
        cond.notify_all();
    }
}

int main() {
    std::thread pinger(ping);
    std::thread ponger(pong);
    pinger.join();
    ponger.join();
    return 0;
}
