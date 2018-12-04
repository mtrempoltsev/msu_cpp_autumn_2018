#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>


std::mutex mutex;
std::condition_variable cond_var;
bool isready = true;
size_t iters = 500000;

void print_ping() {
  for (size_t i=0; i<iters; ++i) {
      std::unique_lock<std::mutex> lock(mutex);
      cond_var.wait(lock, [](){ return isready; });
      std::cout << "ping" << '\n';
      isready = false;
      cond_var.notify_one();
  }
}

void print_pong() {
  for (size_t i=0; i<iters; ++i) {
      std::unique_lock<std::mutex> lock(mutex);
      cond_var.wait(lock, [](){ return !isready; });
      std::cout << "pong" << '\n';
      isready = true;
      cond_var.notify_one();
  }
}

int main() {
  std::thread t1(print_ping);
  std::thread t2(print_pong);
  t1.join();
  t2.join();
  return 0;
}
