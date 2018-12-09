#include <iostream>
#include <condition_variable>
#include <thread>

constexpr int ITERS = 5e5;

std::condition_variable cond_var;
std::mutex m;
bool done = true;

void ping()
{
    std::unique_lock<std::mutex> lock(m);
    for (int i = 0; i < ITERS; i++) {
        cond_var.wait(lock, [](){ return done; });
        std::cout << "ping\n";
        done = false;
        cond_var.notify_one();
    }
}

void pong()
{
    std::unique_lock<std::mutex> lock(m);
    for (int i = 0; i < ITERS; i++) {
        cond_var.wait(lock, [](){ return !done; });
        std::cout << "pong\n";
        done = true;
        cond_var.notify_one();
    }
}

int main()
{
    std::thread t1(ping);
    std::thread t2(pong);
    t1.join();
    t2.join();
    return 0;
}
