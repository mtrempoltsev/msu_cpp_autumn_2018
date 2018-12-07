#include <iostream>
#include <mutex>
#include <thread>


std::mutex mutex_pong;
std::mutex mutex_ping;
const size_t max_it = 500000;

void pong()
{
    for(size_t it = 0; it < max_it; it++)
    {
        mutex_pong.lock();
        std::cout << "pong\n";
        mutex_ping.unlock();
    }
}

void ping()
{
    for(size_t it = 0; it < max_it; it++)
    {
        mutex_ping.lock();
        std::cout << "ping\n";
        mutex_pong.unlock();
    }
}


int main ()
{
    mutex_pong.lock();
    std::thread t1(ping);
    std::thread t2(pong);
    t1.join();
    t2.join();
    return 0;
}
