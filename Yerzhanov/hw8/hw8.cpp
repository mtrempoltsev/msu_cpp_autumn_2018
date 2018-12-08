#include <iostream>
#include <mutex>
#include <thread>

std::mutex pi;
std::mutex po;
const size_t COUNT = 500000;

void pong() {
    for(size_t i = 0; i < COUNT; ++i) {
        pi.lock();
        std::cout << "pong\n";
        po.unlock();
    }
}

void ping() {
    for(size_t i = 0; i < COUNT; ++i) {
        po.lock();
        std::cout << "ping\n";
        pi.unlock();
    }
}


int main () {
    pi.lock();
    std::thread first(ping);
    std::thread second(pong);
    first.join();
    second.join();
    return 0;
}
