#include <iostream>
#include <thread>
#include <mutex>

class ping_pong{
private:
    std::mutex m_ping;
    std::mutex m_pong;
    void ping(const int n) {
        if (n != 0) {
            m_ping.lock();
            printf("%s\n", "ping");
            m_pong.unlock();
            ping(n - 1);
        }
    }

    void pong(const int n) {
        if (n != 0) {
            m_pong.lock();
            printf("%s\n", "pong");
            m_ping.unlock();
            pong(n - 1);
        }
    }
public:
    ping_pong(const int n = 1){
        bool is_now_ping = true;
        m_pong.lock();
        std::thread t(&ping_pong::ping, this, n);
        std::thread t1(&ping_pong::pong, this, n);
        t.join();
        t1.join();
    }
};

int main(){
    ping_pong a(500000);
    return 0;
}
