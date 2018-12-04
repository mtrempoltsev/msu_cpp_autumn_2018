#include <condition_variable>
#include <thread>
#include <iostream>

constexpr int NUM_OF_ITERS = 5e5;
std::mutex m;
std::condition_variable c1;
std::condition_variable c2;
bool notif1 = false;
bool notif2 = false;

void ping_thr() {
    std::unique_lock<std::mutex> lock(m);
    for (int i = 0; i < NUM_OF_ITERS; ++i) {
        c1.wait(lock, [](){ return notif1; });
        notif1 = false;
        std::cout << "ping\n";
        notif2 = true;
        c2.notify_one();
    }
}

void pong_thr() {
    std::unique_lock<std::mutex> lock(m);
    for (int i = 0; i < NUM_OF_ITERS; ++i) {
        c2.wait(lock, [](){ return notif2; });
        notif2 = false;
        std::cout << "pong\n";
        notif1 = true;
        c1.notify_one();
    }
}

int main() {
    std::thread thr1(ping_thr);
    std::thread thr2(pong_thr);
    notif1 = true;
    c1.notify_one();
    thr1.join();
    thr2.join();
}
