#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

enum {PING_AMOUNT = 500000};

class Semaphore
{
    std::mutex mutex_;
    std::condition_variable condition_;
    int counter_;
public:
    explicit Semaphore(int initialValue = 1)
        : counter_(initialValue)
    {
    }

    void enter()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, 
            [this]()
            { 
                return counter_ > 0;
            });
        --counter_;
    }

    void leave()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        ++counter_;
        condition_.notify_one();
    }
};

Semaphore s1, s2(0);

void ping_out() {
    for (int i = 0; i < PING_AMOUNT; i++) {
        s1.enter();
        std::cout << "ping\n";
        s2.leave();
    }
}

void pong_out() {
    for (int i = 0; i < PING_AMOUNT; i++) {
        s2.enter();
        std::cout << "pong\n";
        s1.leave();
    }
}

int main() {
    std::thread t1(ping_out);
    std::thread t2(pong_out);

    t1.join();
    t2.join();

    return 0;
}