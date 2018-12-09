#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex m;
constexpr size_t limit = 500000;
std::condition_variable condition;
bool flag = true;

void ping() {
    for(size_t i = 0; i < limit; ++i) {
        std::unique_lock<std::mutex> lock(m);
        condition.wait(lock, [](){return flag;});
        std::cout << "ping" << '\n';
        flag = false;
        condition.notify_one();
    }
}

void pong() {
    for(size_t i = 0; i < limit; ++i) {
        std::unique_lock<std::mutex> lock(m);
        condition.wait(lock, [](){return !flag;});
        std::cout << "pong" << '\n';
        flag = true;
        condition.notify_all();
    }    
}

int main() {
    std::thread t1(ping);
    std::thread t2(pong);
    t1.join();
    t2.join();
    return 0;
}
