#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int read_num(std::string str, int &cur) {
    std::string buf;
    int num = 0;
    while (str[cur] != '}') {
        if (cur == str.length()) {
            throw std::runtime_error("invalid branch sequence");
        }
        if ((str[cur] < '0') || (str[cur] > '9')) {
            throw std::runtime_error("invalid number");
        }
        buf += str[cur];
        cur++;
    }
    num = std::stoi(buf);
    return num;
}

template<typename... Args>
std::string to_string(Args &&... args) {
    std::ostringstream sstr;
    (sstr << ... << args );
    return sstr.str();
}

template<class ...Args>
std::string _format(std::string str, Args &&... args) {
    std::vector<std::string> params{to_string(std::forward<Args>(args))...};
    int cur = 0;
    int num = 0;
    std::string result;
    while (cur < str.length()) {
        if (str[cur] == '}') {
            throw std::runtime_error("invalid branch sequence");
        }
        if (str[cur] != '{') {
            result += str[cur];
        } else {
            num = read_num(str, ++cur);
            if (num < params.size()) {
                result += params[num];
            } else {
                throw std::runtime_error("no such parameter");
            }
        }
        cur++;
    }

    return result;
}

std::string format(std::string str) {
    return str;
}

template<class ...Args>
std::string format(std::string str, Args &&... args) {
    return _format(str, std::forward<Args>(args)...);
}