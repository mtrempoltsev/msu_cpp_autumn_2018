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

static int get_num_and_offset (const char *c, int& num) {
  int i = 0, tmp = 0;
  if (sscanf (c, "%d", &tmp) != 1)
    throw runtime_error("error");
  num = tmp;
  if (tmp == 0)
    return 2;
  for(; tmp > 0; tmp /= 10, ++i);
  return i + 1;
}

template <class... ArgsT>
string format(const string& str, ArgsT&&... args_) {
  vector<string> args { convert_arg_to_str (forward<ArgsT> (args_))...};
  ostringstream out;
  int len = str.length ();
  for (int i = 0; i < len; ++i) {
    if (str[i] == '{') {
      int arg_n = 0;
      int offset = get_num_and_offset (str.c_str () + i + 1, arg_n);
      if (str[i + offset] != '}')
        throw runtime_error("error");
      if (arg_n >= args.size ())
        throw runtime_error("error");
      out << args[arg_n];
      i += offset;
    }
    else if (str[i] == '}')
      throw runtime_error("error");
    else
      out << str[i];
  }
  return out.str ();
}
