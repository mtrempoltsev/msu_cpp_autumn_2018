#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

enum { NUMBER = 500000 };

mutex m;
int proc = 0;

void proc1()
{
	for (int j = 0; j < NUMBER; j++) {
		while (true) {
			m.lock();
			if (proc == 0) {
				proc = 1;
				cout << "ping\n";
				m.unlock();
				break;
			}
			m.unlock();
		}
	}
}

void proc2()
{
	for (int j = 0; j < NUMBER; j++) {
		while (true) {
			m.lock();
			if (proc == 1) {
				proc = 0;
				cout << "pong\n";
				m.unlock();
				break;
			}
			m.unlock();
		}
	}
}

int main()
{
	thread t1(proc1);
	thread t2(proc2);
	t1.join();
	t2.join();
	return 0;
}
