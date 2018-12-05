#pragma once
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>

std::string format(std::string&& inp) {
    return inp;
}

template<class T>
void print(std::stringstream& s, size_t n, T&& curr) {
    if (n == 0) {
        s << curr;
    } else {
        throw std::runtime_error("");
    }
}

template<class T, class... Args>
void print(std::stringstream& s, size_t n, T&& curr, Args&&... args) {
    if (n == 0) {
        s << curr;
    } else if (n > sizeof...(args) || n < 0) {
        throw std::runtime_error("");
    } else {
        print(s, n - 1, std::forward<Args>(args)...);
    }
}

template<class... Args>
std::string format(std::string&& inp, Args&&... args) {
    std::stringstream s;
    for (size_t i = 0; i < inp.size(); ++i) {
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
            print(s, tmp, std::forward<Args>(args)...);
        } else if (inp[i] == '}') {
            throw std::runtime_error("");
        } else {
            s << inp[i];
        }
    }
    return s.str();
}
