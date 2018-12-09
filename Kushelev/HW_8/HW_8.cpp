#include <iostream>
#include <condition_variable>
#include <thread>

bool ping_time = true;
const int max_iter = 500000;
std::condition_variable cond;
std::mutex m;

void do_ping()
{
    for(int i = 0; i < max_iter; i++)
    {
        std::unique_lock<std::mutex> lock(m);
        cond.wait(lock, []{return ping_time;});
        std::cout << "ping" << "\n";
        ping_time = false;
        cond.notify_one();
    }
}

void do_pong()
{
    for(int i = 0; i < max_iter; i++)
    {
        std::unique_lock<std::mutex> lock(m);
        cond.wait(lock, []{return (!ping_time);});
        std::cout << "pong" << "\n";
        ping_time = true;
        cond.notify_one();
    }
}

int main()
{
    std::thread player1(do_ping);
    std::thread player2(do_pong);
    player1.join();
    player2.join();
    return 0;
}
