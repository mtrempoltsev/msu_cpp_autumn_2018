#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <atomic>

const size_t each_to_repeat = 500000*2;
std::mutex mutex;
std::atomic<size_t> cur_repeat{0};

void print_string(std::string str, size_t order){
    for(;;){
        std::lock_guard<std::mutex> lock(mutex);
        if(cur_repeat >= each_to_repeat)
            break;

        if(cur_repeat % 2 == 0)
            if(order == 0){
                std::cout << str << "\n";
                ++cur_repeat;
            }

        if(cur_repeat % 2 == 1)
            if(order == 1){
                std::cout << str << "\n";
                ++cur_repeat;
            }
    }
}

int main(){
    std::thread ping_t, pong_t;

    ping_t = std::thread(print_string, "ping", 0);
    pong_t = std::thread(print_string, "pong", 1);

    ping_t.join();
    pong_t.join();

    return 0;
}