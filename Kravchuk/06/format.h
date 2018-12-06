#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

using namespace std;

string format (const string& str) {
  return str;
}

template <class T>
string convert_arg_to_str (T&& value) {
  stringstream stream;
  stream << value;
  return stream.str ();
}

template <class... ArgsT>
string format(const string& str, ArgsT&&... args_) {
  vector<string> args { convert_arg_to_str (forward<ArgsT> (args_))...};
  ostringstream out;
  int len = str.length ();
  for (int i = 0; i < len; ++i) {
    if (str[i] == '{') {
      if (i + 2 >= len || str[i + 1] < '0' || str[i + 1] > '9' || str[i + 2] != '}')
        throw runtime_error("error");
      unsigned int arg_n = str[i + 1] - '0';
      if (arg_n >= args.size ())
        throw runtime_error("error");
      out << args[arg_n];
      i += 2;
    }
    else if (str[i] == '}')
      throw runtime_error("error");
    else
      out << str[i];
  }
  return out.str ();
}
