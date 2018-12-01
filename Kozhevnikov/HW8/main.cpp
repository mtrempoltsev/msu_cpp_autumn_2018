#include <iostream>
#include <thread>
#include <mutex>

std::mutex m;
int turn = 0;

const int reps = 500000;

void ping() {
    int c = reps;

    while (c > 0) {
        if (m.try_lock()) {
            if (turn == 0) {
                c--;
                std::cout << "ping" << std::endl;
                turn  = 1;
            }
            m.unlock();
        }
    }
}

void pong() {
    int c = reps;
    while (c > 0) {
        if (m.try_lock()) {
            if (turn == 1) {
                c--;
                std::cout << "pong" << std::endl;
                turn = 0;
            }
            m.unlock();
        }
    }
}

int main() {
    std::thread t1(ping);
    std::thread t2(pong);

    t1.join();
    t2.join();

    return 0;
}