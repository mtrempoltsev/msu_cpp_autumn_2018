#include <iostream>
#include "calculator_class.h"

using namespace std;

int main(int argc, char *argv[]){
  if (argc != 2) {
    cout << "error" << endl;
    return 1;
  }
  calculator_class calculator (argv[1]);
  try {
    cout << calculator.compute () << endl;
  } catch (const Error &error) {
    cout << "error" << endl;
    return 1;
  }
  return 0;
}
