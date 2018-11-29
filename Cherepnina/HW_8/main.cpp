#include <thread>
#include <iostream>
#include <atomic>

using namespace std;

const int count  = 500000;

atomic<int> flag;

void thread_0() {
    int i = 0;
    while (i<count){
        if (flag == 0) {
            cout << "ping" << endl;
            ++i;
            flag = 1;
        }
        this_thread::yield();
    }
}

void thread_1() {
    int i = 0;
    while (i<count){
        if (flag == 1) {
            cout << "pong" << endl;
            ++i;
            flag = 0;
        }
        this_thread::yield();
    }
}

int main() {

    flag = 0;
    thread t_0(thread_0);
    thread t_1(thread_1);

    t_0.join();
    t_1.join();

    return 0;
}
