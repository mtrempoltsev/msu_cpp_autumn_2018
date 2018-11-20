#pragma once

#include <string>
#include <vector>
#include <exception>
#include <sstream>

template<class T>
std::string to_string(T&& t) {
    std::ostringstream out;
    out << t;
    return out.str();
}

template<class... Args>
std::string format(const std::string& s, Args&&... args) {
    std::vector<std::string> strArgs{to_string(std::forward<Args>(args))...};
    std::istringstream in(s);
    std::ostringstream out;
    char c;
    in >> std::noskipws;
    while (in >> c) {
        if (c == '{') {
            int n;
            in >> n;
            if (n >= strArgs.size() || !(in >> c) || (c != '}')) {
                throw std::runtime_error("error");
            }
            out << strArgs[n];
        } else if (c == '}') {
            throw std::runtime_error("error");
        } else  {
            out << c;
        }
    }
    return out.str();
}
