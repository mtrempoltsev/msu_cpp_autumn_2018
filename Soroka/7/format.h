#pragma once
#include <string>
#include <vector>
#include <exception>
#include <sstream>

template<class T>
std::string to_str(T&& temp)
{
    std::ostringstream out;
    out << temp;
    return out.str();
}

template<class... Args>
std::string format(const std::string& str, Args&&... args)
{
    std::ostringstream out;
    std::istringstream in(str);
    std::vector<std::string> arguments{to_str(std::forward<Args>(args))...};
    in >> std::noskipws;
    char ch;
    while (in >> ch) {
        if (ch == '{') {
            int n;
            in >> n;
            if (n >= arguments.size() || !(in >> ch) || (ch != '}')) {
                throw std::runtime_error("error");
            }
            out << arguments[n];
        } else if (ch == '}') {
            throw std::runtime_error("error");
        } else  {
            out << ch;
        }
    }
    return out.str();
}


