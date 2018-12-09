#include <thread>
#include <iostream>
#include <mutex>
#include <atomic>
#include <condition_variable>

using namespace std;

mutex m;
const size_t N = 1000000;
atomic_int cur_step{0};
condition_variable cond;

void PingPong(const size_t t_num)
{
	while (cur_step < N)
	{
		unique_lock<mutex> lock(m);
		
		while ((cur_step & 1) != t_num)
		{
			cond.wait(lock);
		}
		
		if (t_num == 0)
		{
			std::cout << "ping\n";
		}
		else
		{
			std::cout << "pong\n";
		}
		
		++cur_step;
		cond.notify_one();
	}
}

int main()
{
	thread t1(PingPong, 0);
	thread t2(PingPong, 1);
	t1.join();
	t2.join();
	return 0;
}
