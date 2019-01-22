#include <iostream>
#include <cstddef>
#include <thread>
#include <atomic>

int main() {
    const size_t COUNT = 500000;
    std::atomic_bool isPing(false);

    auto thread1 = [&] () {
        for (size_t i = 0; i < COUNT; ++i) {
            while (isPing);

            std::cout << "ping" << std::endl;
            isPing = true;
        }
    };

    auto thread2 = [&] () {
        for (size_t i = 0; i < COUNT; ++i) {
            while (!isPing);

            std::cout << "pong" << std::endl;
            isPing = false;
        }
    };

    std::thread t1(thread1);
    std::thread t2(thread2);

    t1.join();
    t2.join();

    return 0;
}
