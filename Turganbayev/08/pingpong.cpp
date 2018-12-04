#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

class PingPong {
private:
    const size_t iters_;
    virtual void ping() = 0;
    virtual void pong() = 0;
public:
    size_t iters() const { return iters_; }
    explicit PingPong(size_t iters_count) : iters_(iters_count) {}
    virtual void run() {
        std::thread tr1(&PingPong::ping, this);
        std::thread tr2(&PingPong::pong, this);
        tr1.join();
        tr2.join();
    }
    virtual ~PingPong() = default;
};

class AtomicPingPong : public PingPong {
private:
    std::atomic<bool> is_ping;
    void ping() override {
        size_t i = 0;
        while (i < iters()) {
            if (is_ping) {
                std::cout << "ping" << std::endl;
                i++;
                is_ping = !is_ping;
            }
        }
    }
    void pong() override {
        size_t i = 0;
        while (i < iters()) {
            if (!is_ping) {
                std::cout << "pong" << std::endl;
                i++;
                is_ping = !is_ping;
            }
        }
    }
public:
    explicit AtomicPingPong(size_t iters_count, bool is_ping_ = true) : 
                PingPong(iters_count), is_ping(is_ping_) {}
};

class MutexPingPong : public PingPong {
private:
    bool is_ping;
    std::mutex mutex;
private:
    void ping() override{
        size_t i = 0;
        while (i < iters()) {
            if (is_ping) {
                std::lock_guard<std::mutex> lock(mutex);
                std::cout << "ping" << std::endl;
                is_ping = !is_ping;
                i++;
            }
        }
    }
    void pong() override {
        size_t i = 0;
        while (i < iters()) {
            if (!is_ping) {
                std::lock_guard<std::mutex> lock(mutex);
                std::cout << "pong" << std::endl;
                is_ping = !is_ping;
                i++;
            }
        }
    }
public:
    explicit MutexPingPong(size_t iters_count) : 
                PingPong(iters_count), is_ping(true) {}
};

int main(int argc, char** argv) {
    AtomicPingPong pingpong(500000);
    //MutexPingPong pingpong(500000);
    pingpong.run();
}