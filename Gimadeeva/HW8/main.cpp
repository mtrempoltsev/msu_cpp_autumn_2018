#include <iostream>
#include <thread>
#include <atomic>

#define N 500000

using namespace std;

static atomic<int> change (0);

void ping () {
    size_t counter1 = 0;
    while (counter1 < N) {
        if (change == 0) {
            cout << "ping" << endl;
            counter1++;
            change = 1;
        }
    }
}

void pong () {
    size_t counter2 = 0;
    while (counter2 < N) {
        if (change == 1) {
            cout << "pong" << endl;
            counter2++;
            change = 0;
        }
    }
}


int main() {
    thread t1 (ping);
    thread t2 (pong);

    t1.join();
    t2.join();
    return 0;
}
