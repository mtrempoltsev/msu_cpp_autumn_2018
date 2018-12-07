#include <thread>
#include <mutex>

using namespace std;

const int MAX_ITER = 500000;
mutex pi, po;

void ping() {
    for (int i = 0; i < MAX_ITER; ++i) {
        pi.lock();
        printf("ping\n");
        po.unlock();
    }
}

void pong() {
    for (int i = 0; i < MAX_ITER; ++i) {
        po.lock();
        printf("pong\n");
        pi.unlock();
    }
}

int main() {
    thread t1(ping);
    thread t2(pong);
    t1.join();
    t2.join();
    return 0;
}
