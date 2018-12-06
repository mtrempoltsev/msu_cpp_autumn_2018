#include <iostream>
#include <mutex>
#include <thread>

#define N 500000

std::mutex m;
std::mutex m1;

void ping()
{
    for(int i = 0; i < N; i++)
    {
        m.lock();
        std::cout<<"ping\n";
        m1.unlock();
    }
}

void pong()
{
    for(int i = 0; i < N; i++)
    {
        m1.lock();
        std::cout<<"pong\n";
        m.unlock();
    }
}

int main()
{
    std::thread t(ping);
    std::thread t2(pong);
    m1.lock();
    t.join();
    t2.join();
    return 0;
}
