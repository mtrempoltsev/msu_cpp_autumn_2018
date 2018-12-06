#include <iostream>
#include <thread>
#include <condition_variable>

std::mutex m;
std::condition_variable cv;
bool is_ping = false;

void ping() {
    for (int i = 0; i < 500000; ++i) {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, []{return !is_ping;});
        std::cout << "ping\n";
        is_ping = true;
        cv.notify_one();
    }
}

void pong() {
    for (int i = 0; i < 500000; ++i) {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, []{return is_ping;});
        std::cout << "pong\n";
        is_ping = false;
        cv.notify_one();
    }
}

int main() {
    std::thread thr1(ping);
    std::thread thr2(pong);
    thr1.join();
    thr2.join();
    return 0;
}
