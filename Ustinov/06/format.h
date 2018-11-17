#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>


std::string format(const std::string& str) {
    return str;
}

template <class T>
void parse_args(std::vector<std::string>& params, T&& t) {
    std::ostringstream out;
    out << t;
    params.push_back(out.str());
}

template <class T, class... ArgsT>
void parse_args(std::vector<std::string>& params, T&& t, ArgsT&&... args) {
  std::ostringstream out;
  out << t;
  params.push_back(out.str());
  parse_args(params, std::forward<ArgsT>(args)...);
}

template <class... ArgsT>
std::string format(const std::string& str, ArgsT&&... args) {
  std::vector<std::string> params;
  std::ostringstream out;
  int num = 0;

  parse_args(params, std::forward<ArgsT>(args)...);
  for (int i=0; i < str.length(); ++i) {
      if (str[i] == '{') {
          if (i + 2 >= str.length()) {
              throw std::runtime_error("error");
          }
          if ((str[i+1] < '0') || (str[i+1] > '9')) {
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
