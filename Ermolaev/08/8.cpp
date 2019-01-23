#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

const size_t MAX_ITER = 500000;
mutex ping_mutex, pong_mutex;

void ping() {
    for (size_t i = 0; i < MAX_ITER; ++i) {
        ping_mutex.lock();
        std::cout << "ping" << endl;
        pong_mutex.unlock();
    }
}

void pong() {
    for (size_t i = 0; i < MAX_ITER; ++i) {
        pong_mutex.lock();
        cout << "pong" << endl;
        ping_mutex.unlock();
    }
}

int main() {
    thread ping_thread(ping);
    thread pong_thread(pong);
    ping_thread.join();
    pong_thread.join();
    return 0;
}
