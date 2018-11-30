#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <atomic>

std::mutex m;
std::unique_lock<std::mutex> lock{m};
static std::condition_variable cond;
static std::atomic<int> turn{0};
static std::atomic<bool> finish{false};
static constexpr int turns = 500;

void player() {
    while (true) {
        cond.wait(lock);
        if (turn < turns) {
            if (turn % 2) {
                std::cout << "ping" << std::endl;
            } else {
                std::cout << "pong" << std::endl;
            }
            ++turn;
            cond.notify_one();
        } else {
            if (!finish) {
                finish = !finish;
                cond.notify_one();
                return;
            } else {
                return;
            }
        }
    }
}

int main()
{
    std::thread p1{player};
    std::thread p2{player};
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    cond.notify_one();
    p1.join();
    p2.join();
    return 0;
}
