#include <iostream>
#include <thread>
#include <string>
#include <mutex>
#include <atomic>

const size_t count_to_repeat = 500000*2;
std::atomic<size_t> current_repeat{0};

void print_string(size_t order){
    const char* strings[2] = { "ping", "pong" };

    while(current_repeat < count_to_repeat){
        if(current_repeat % 2 == order){
            std::cout << strings[order] << "\n";
            ++current_repeat;
        }
    }
}

int main(){
    std::thread ping_t, pong_t;

    ping_t = std::thread(print_string, 0);
    pong_t = std::thread(print_string, 1);

    ping_t.join();
    pong_t.join();

    return 0;
}