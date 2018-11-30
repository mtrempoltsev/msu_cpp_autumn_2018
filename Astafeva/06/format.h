#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>

template <class T>
std::string to_string(T&& arg) {
    std::ostringstream oss;
    oss << arg;
    return oss.str();
}

std::string format(const std::string& str) {
	return str;
}

template<class... ArgsT>
std::string format(const std::string& str, ArgsT&&... args) {
    std::vector<std::string> a{to_string(std::forward<ArgsT>(args))...};
    int argsz = a.size();
    std::ostringstream res;
    int len = str.length();
    for (int i = 0; i < len; ++i) {
        if (str[i] == '{') {
            int n = 0;
            ++i;
            if (str[i] == '}') {
                throw std::runtime_error("");
            }
            while (i < len && str[i] != '}') {
                if (str[i] >= '0' && str[i] <= '9') {
                    n *= 10;
                    n += str[i] - '0';
                } else {
                    throw std::runtime_error("");
                }
                ++i;
            }
            if (i == len) {
                throw std::runtime_error("");
            }
            if (n >= argsz) {
                throw std::runtime_error("");
            }
            res << a[n];
        } else if (str[i] == '}') {
            throw std::runtime_error("");
        } else {
            res << str[i];
        }
    }
    return res.str();
}
