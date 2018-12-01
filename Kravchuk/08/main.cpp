#include <iostream>
#include <future>
#include <thread>
#include <atomic>

#define N 500000

using namespace std;

static atomic<int> counter (0);

void ping (){
  unsigned int printed_counter = 0;
  while (printed_counter < N){
      if (counter == 0){
          cout << "ping" << endl;
          printed_counter++;
          counter = 1;
        }
    }
}

void pong (){
  unsigned int printed_counter = 0;
  while (printed_counter < N){
      if (counter == 1){
          cout << "pong" << endl;
          printed_counter++;
          counter = 0;
        }
    }
}

int main(){
  thread t1 (ping);
  thread t2 (pong);

  t1.join ();
  t2.join ();
  return 0;
}
