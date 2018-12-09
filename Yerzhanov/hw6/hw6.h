#pragma once
#include <iostream>
#include <vector>
#include <sstream>


template<typename T>
std::string make_string(T&& item) {
    std::stringstream s;
    s << item;
    return s.str();
}

template<typename... Args>
std::string format(const std::string& inp, Args&&... args) {
    std::vector<std::string> args_list{make_string(std::forward<Args>(args))...};
    std::stringstream s;
    size_t max = sizeof...(args);
    size_t size = inp.size();
    for (size_t i = 0; i < size; ++i) {
        if (inp[i] == '{') {
            size_t tmp = 0;
            ++i;
            while (inp[i] != '}') {
                if (!isdigit(inp[i])) {
                    throw std::runtime_error("");
                }
                tmp = tmp * 10 + inp[i] - '0';
                ++i;
            }
            if (tmp >= max) {
                throw std::runtime_error("");
            }
            s << args_list[tmp];
        } else if (inp[i] == '}') {
            throw std::runtime_error("");
        } else {
            s << inp[i];
        }
    }
    return s.str();
}
