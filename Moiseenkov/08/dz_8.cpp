#include <iostream>
#include <atomic>
#include <thread>

using namespace std;

static atomic_int count{0};
static constexpr size_t all_iterations = 1000000;

void ping_pong(size_t ch_ping_pong) 
{
    while (true) 
    {
        if (count >= all_iterations)
            break;

        if (ch_ping_pong == 0 && count % 2 == 0) 
        {
            cout << "ping\n";
            count++;
        }

        if (ch_ping_pong == 1 && count % 2 == 1) 
        {
            cout << "pong\n";
            count++;
        }
    }
}

int main() 
{
    thread p1(ping_pong, 0);
    thread p2(ping_pong, 1);

    p1.join();
    p2.join();

    return 0;
}