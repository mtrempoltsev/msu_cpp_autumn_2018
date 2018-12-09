#include <iostream>
#include <thread>
#include <atomic>

using namespace std;

enum { NUMBER = 500000 };

atomic <int> proc;

void proc1()
{
	for (int j = 0; j < NUMBER; j++) {
		if (proc == 1) {
			cout << "ping" << endl;
			proc = 2;
		} else j--; 
	}
}

void proc2()
{
	for (int j = 0; j < NUMBER; j++) {
		if (proc == 2) {
			cout << "pong" << endl;
			proc = 1;
		}
		else j--;
	}
}

int main()
{
	proc = 1;
	thread t1(proc1);
	thread t2(proc2);
	t1.join();
	t2.join();
	return 0;
}
