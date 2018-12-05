#ifndef CALCULATOR_CLASS_H
#define CALCULATOR_CLASS_H

#include <string>

class Error {
public:
  std::string message;
  Error (const std::string &message_arg): message (message_arg) {}
};

class calculator_class{
public:
  calculator_class(const char *str_arg);

  int compute ();

private:
  int expr (int &current);
  int term (int &current);
  int prim (int &current);

  std::string str;
};

#endif // CALCULATOR_CLASS_H
