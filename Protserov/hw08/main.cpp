#include <mutex>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <string>

static constexpr unsigned players{2};
static constexpr unsigned plays_each{500000};
static std::atomic<bool> notified[players];
static std::mutex m;
static std::condition_variable conds[players];
static const std::string phrases[players] {"ping\n", "pong\n"};

void player(unsigned parity)
{
    std::unique_lock<std::mutex> lock{m};
    for (unsigned i = 0; i < plays_each; ++i) {
        conds[parity].wait(lock, [parity]() {
            return bool{notified[parity]};
        });
        notified[parity] = false;
        std::cout << phrases[parity];
        notified[(parity + 1) % players] = true;
        conds[(parity + 1) % players].notify_one();
    }
}

int main()
{
    for (unsigned i = 0; i < players; ++i) {
        notified[i] = true;
    }
    std::thread t1{player, 0};
    std::thread t2{player, 1};
    conds[0].notify_one();
    t1.join();
    t2.join();

    return 0;
}
