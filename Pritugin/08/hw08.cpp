#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>

using namespace std;
using Lock = unique_lock<mutex>;


template<int T>
void make(mutex& m, bool& flag, condition_variable& c)
{
	for (int i = 0; i < 500000; ++i)
	{
		Lock lock(m);
		c.wait(lock, [&flag](){ return T ? flag : !flag; });
		cout << (T ? "ping\n" : "pong\n");
		flag = T ? false : true ;
		c.notify_all();
	}
}

int main()
{
	mutex m;
	bool flag = true;
	condition_variable c;
	std::thread first(make<0>, ref(m), ref(flag), ref(c));
	std::thread second(make<1>, ref(m), ref(flag), ref(c));
	first.join();
	second.join();
	return 0;
}
