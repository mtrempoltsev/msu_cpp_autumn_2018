#include <thread>
#include <iostream>
#include <mutex>

using namespace std;

mutex m1, m2;

void pingFunction(const size_t N)
{
	for (size_t i = 0; i < N; ++i)
	{
		m1.lock();
		cout << "ping\n";
		m2.unlock();
	}
}

void pongFunction(const size_t N)
{
	for (size_t i = 0; i < N; ++i)
	{
		m2.lock();
		cout << "pong\n";
		m1.unlock();
	}
}

int main()
{
	const size_t N = 500000;
	thread t1(bind(pingFunction, N));
	thread t2(bind(pongFunction, N));
	m2.lock();
	t1.join();
	t2.join();
	m2.unlock();
	return 0;
}
