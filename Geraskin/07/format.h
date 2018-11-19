#pragma once

#include <string>
#include <vector>
#include <exception>
#include <sstream>

template<class T>
void parse(std::vector<std::string>& v, T&&t) {
    std::ostringstream out;
    out << t;
    v.push_back(out.str());
}

template<class T, class... Args>
void parse(std::vector<std::string>& v, T&& t, Args&&... args) {
    parse(v, t);
    parse(v, std::forward<Args>(args)...);
}

std::string format(const std::string& s) {
    return s;
}

template<class... Args>
std::string format(const std::string& s, Args&&... args) {
    std::vector<std::string> strArgs;
    parse(strArgs, std::forward<Args>(args)...);
    std::istringstream stream(s);
    std::ostringstream out;
    char c;
    while (stream >> std::noskipws >> c) {
        if (c == '{') {
            int n;
            stream >> std::noskipws >> n;
            if (n >= strArgs.size()) {
                throw std::runtime_error("error");
            }
            if (!(stream >> std::noskipws >> c)) {
                throw std::runtime_error("error");
            }
            if (c != '}') {
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
