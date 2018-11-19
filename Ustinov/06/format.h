#include <iostream>
#include <string>
#include <vector>
#include<sstream>
#include <stdexcept>


std::string format(const std::string& str) {
    return str;
}

template <class T>
std::string arg_to_string (T&& arg)
{
    std::stringstream stream;
    stream << arg;
    return stream.str();
}

template <class... ArgsT>
std::string format(const std::string& str, ArgsT&&... args) {
  std::vector<std::string> params{arg_to_string(std::forward<ArgsT>(args))...};
  std::ostringstream out;
  int num = 0;
  char x;

  for (int i=0; i < str.length(); ++i) {
      if (str[i] == '{') {
          if (i + 2 >= str.length()) {
              throw std::runtime_error("error");
          }
          x = str[i+1];
          if ((x < '0') || (x > '9')) {
              throw std::runtime_error("error");
          }
          if (str[i+2] != '}') {
              throw std::runtime_error("error");
          }
          num = str[i+1] - 48;
          if (num >= params.size()) {
              throw std::runtime_error("error");
          }
          out << params[num];
          i += 2;
      } else {
          if (str[i] == '}') {
              throw std::runtime_error("error");
          }
          out << str[i];
      }
  }
  return out.str();
}
