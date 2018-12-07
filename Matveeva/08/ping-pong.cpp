#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

mutex m;
bool flag = true;
const int n = 500000;
condition_variable c;

void ping()
{
	for (int i = 0; i < n; i++)
	{
		unique_lock<mutex> lock(m);
		while (!flag)
        {
            c.wait(lock);
        }
		cout << "ping" << '\n';
		flag = !flag;
		c.notify_one();
	}
}

void pong()
{
	for (int i = 0; i < n; i++)
	{
		unique_lock<mutex> lock(m);
		while (flag)
        {
            c.wait(lock);
        }
		cout << "pong" << '\n';
		flag = !flag;
		c.notify_one();
	}
}

int main()
{
	thread t1(ping);
	thread t2(pong);
	t1.join();
	t2.join();
	return 0;
}
