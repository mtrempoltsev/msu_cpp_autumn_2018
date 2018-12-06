#include <iostream>
#include <thread>
#include <mutex>

class PingPong{
private:
    std::mutex ping_mutex;
    std::mutex pong_mutex;
    
    std::thread t1;
    std::thread t2;
    
private:
    void ping(const int n) {
        for (int i = 0; i < n; ++i) {
            ping_mutex.lock();
            printf("%s\n", "ping");
            pong_mutex.unlock();
        }
    }
    
    void pong(const int n) {
        for (int i = 0; i < n; ++i) {
            pong_mutex.lock();
            printf("%s\n", "pong");
            ping_mutex.unlock();
        }
    }
    
public:
    PingPong(const int n = 1){
        pong_mutex.lock();
        t1 = std::thread(&PingPong::ping, this, n);
        t2 = std::thread(&PingPong::pong, this, n);
    }
    
    void launch() {
        t1.join();
        t2.join();
    }
};

int main(){
    PingPong pp(500000);
    pp.launch();
    return 0;
}
