#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <cstdint>

uint32_t rounds = 500000;
bool pingTime = true;

std::mutex m;
std::condition_variable played;

void ping() //thread 1
{
    while (rounds)
    {
    	std::unique_lock<std::mutex> lock(m);

        played.wait(lock, [](){ return pingTime; });

        std::cout << "ping\n";
        --rounds;
        pingTime = false;

        played.notify_one();
    }
}

void pong() //thread 2
{
    while (rounds)
    {
    	std::unique_lock<std::mutex> lock(m);

		played.wait(lock, [](){ return !pingTime; });

        std::cout << "pong\n";
        pingTime = true;

        played.notify_one();
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