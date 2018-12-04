#include <iostream>
#include <mutex>
#include <atomic>
#include <thread>


static constexpr size_t TOTAL_ITERATIONS = 1000000;

static std::atomic_int counter{0};
static std::mutex mutex;

void ping_pong(size_t ping_or_pong) {
    while (true) {
        std::unique_lock<std::mutex> lock(mutex);
        if (counter >= TOTAL_ITERATIONS)
            break;

        if (ping_or_pong == 0 && counter % 2 == 0) {
            std::cout << "ping\n";
            counter++;
        }

        if (ping_or_pong == 1 && counter % 2 == 1) {
            std::cout << "pong\n";
            counter++;
        }
    }
}

int main()
{
    std::thread t1(ping_pong, 0);
    std::thread t2(ping_pong, 1);

    t1.join();
    t2.join();

    return 0;
}