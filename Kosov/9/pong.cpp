#include <iostream>
#include <condition_variable>
#include <thread>


std::condition_variable_any cv;
std::mutex cv_m;
bool ping = true;
int NUM = 500000;

void waits(){
    for(int i = 0; i < NUM; i++){
        std::unique_lock<std::mutex> lk(cv_m);
        cv.wait(lk, []{return ping;});
        std::cout << "ping\n";
        ping = false;
        cv.notify_one();
    }
}

void signals(){
    for(int i = 0; i < NUM; i++){
        std::unique_lock<std::mutex> lk(cv_m);
        cv.wait(lk, []{return !ping;});
        std::cout << "pong\n";
        ping = true;
        cv.notify_one();
    }
}

int main(){
    std::thread t1(waits), t2(signals);
    t1.join();
    t2.join();
    return 0;
}
