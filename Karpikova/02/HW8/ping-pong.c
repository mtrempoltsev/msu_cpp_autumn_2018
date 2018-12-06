#include <iostream>
#include <thread>

using namespace std;

enum { N = 500000 };

int flag = 0;

void ping()
{
	for (int i = 0; i < N; ++i) {
		if (!flag) {
			cout << "ping" << endl;
			flag = 1;
		} else --i;
	}
}

void pong()
{
	for (int i = 0; i < N; ++i) {
		if (flag) {
			cout << "pong" << endl;
			flag = 0;
		}
		else --i;
	}
}

int main()
{
	thread th1(ping), th2(pong);
	th1.join();
	th2.join();
	return 0;
}
