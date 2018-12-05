#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
using namespace std;

mutex m;
bool r = true;
condition_variable c;

void ping(int n_times)
{
	for (int i = 0; i < n_times; ++i)
	{
		unique_lock<mutex> lock(m);
		while (!r)
			c.wait(lock);
		cout << "ping" << '\n';
		r = !r;
		c.notify_one();
	}
}

void pong(int n_times)
{
	for (int i = 0; i < n_times; ++i)
	{
		unique_lock<mutex> lock(m);
		while (r)
			c.wait(lock);
		cout << "pong" << '\n';
		r = !r;
		c.notify_one();
	}
}

int main()
{
	int n_times = 500000;
	thread t_ping(ping, n_times);
	thread t_pong(pong, n_times);
	t_ping.join();
	t_pong.join();
	return 0;
}