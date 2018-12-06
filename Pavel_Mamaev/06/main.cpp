#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <exception>

template <typename T>
const std::string to_string(T&& t) {
    std::stringstream out;
    out << t;
    return out.str();
}

template <class... ArgsT>
std::string format(const std::string& in_s, ArgsT&&... args) {
    std::vector <std::string> ptr = { to_string(std::forward <ArgsT> (args))... };
    std::string format_s;
    format_s.clear();
    int pos = 0, last = in_s.size() - 1;
    while(in_s[pos]) {
        if (in_s[pos] == '}') {
            throw std::runtime_error("");
        }
        if (in_s[pos] == '{') {
            ++pos;
            int indx = 0;
            bool number = false;
            while (in_s[pos] >= '0' && in_s[pos] <= '9') {
                indx = indx * 10 + in_s[pos++] - '0';
                number = true;
            }
            if (pos > last || in_s[pos] != '}' || 
                    !number || indx + 1 > ptr.size()) {
                throw std::runtime_error("");
            }
            format_s += ptr[indx];
        } else {
            format_s += in_s[pos];
        }
        ++pos;
    }
    return format_s;
}