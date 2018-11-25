#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <exception>
using namespace std;

template<typename T>
const std::string to_string(T&& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

template <typename... ArgsT>
std::string format(const std::string& format_str, ArgsT&& ... args)
{
    vector<std::string> ptr = {to_string(std::forward<ArgsT>(args))...};
    std::string res = "";
    int i = 0, last = format_str.length() - 1;
    while(format_str[i]) {
        if (format_str[i] == '}') {
            throw runtime_error("");
        }
        if (format_str[i] == '{') {
            ++i;
            int ind = 0;
            while (isdigit(format_str[i])) {
                ind = ind * 10 + format_str[i++] - '0';
            }
            if (i > last || format_str[i] != '}' || !isdigit(format_str[i - 1])) {
                throw runtime_error("");
            }
            if (ind + 1 > ptr.size()) {
                throw runtime_error("");
            }
            res += ptr[ind];
            ++i;
        } else {
            res += format_str[i++];
        }
    }
    return res;
}
