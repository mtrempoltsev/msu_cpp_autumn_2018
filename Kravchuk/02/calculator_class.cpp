#include "calculator_class.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "math.h"

calculator_class::calculator_class (const char *str_arg): str (str_arg) {
  str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
}

int calculator_class::compute () {
  int current = 0;
  return expr (current);
}

int calculator_class::expr (int &current) {
  int left = term (current);
  while (true) {
    switch (str[current]) {
      case '+': {
        left += term (++current);
        break;
      }
      case '-': {
        left -= term (++current);
        break;
      }
      default: return left;
    }
  }
}

int calculator_class::term (int &current) {
  int left = prim (current);
  while (true) {
    switch (str[current]) {
      case '*': {
        left *= prim (++current);
        break;
      }
      case '/': {
        int right = prim (++current);
        if (right == 0)
          throw Error ("Devide by zero\n");
        left /= right;
        break;
      }
      default: return left;
    }
  }
}

int calculator_class::prim (int &current) {
  if ('0' <= str[current] && str[current] <= '9') {
    int value = 0;
    if (sscanf (str.c_str () + current, "%d", &value) != 1)
      throw Error ("Wrong input\n");
    for (; '0' <= str[current] && str[current] <= '9'; ++current);
    return value;
  }
  if (str[current] == '-')
    return -prim (++current);
  throw Error ("Wrong input\n");
}
