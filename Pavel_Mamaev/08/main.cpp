#include <iostream>
#include <thread>
#include <atomic>

using namespace std;

enum { SHOTS = 500000 };

atomic <int> sem;

void usr1() {
    int cur_shot = 0;
	while (cur_shot < SHOTS) {
        if (sem == 0) {
            cout << "ping" << endl;
            sem = 1;
            cur_shot++;
        }
	}
}

void usr2() {
	int cur_shot = 0;
	while (cur_shot < SHOTS) {
        if (sem == 1) {
            cout << "pong" << endl;
            sem = 0;
            cur_shot++;
        }
	}
}

int main() {
	sem = 0;
	thread first(usr1);
	thread second(usr2);
	first.join();
	second.join();
	return 0;
}
