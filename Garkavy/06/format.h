#pragma once
#include <iostream>
#include <sstream>
#include <string.h>

bool print_arg_n(std::ostream& os, int n) noexcept
{
    return false;
}

template<typename T, typename... Targs>
bool print_arg_n(std::ostream& os, int n, T&& first, Targs&&... rest) noexcept
// print_arg_n(os, n, ...) печатает n-ый аргумент из (...) в os
// возвращает: успешно или нет
{
    if(n == 0) {
        os << first;
        return true;
    } else if(n > 0) {
        return print_arg_n(os, n - 1, std::forward<Targs>(rest)...);
    } else {
        return false;
    }
}

template<typename... Targs>
std::string format(const std::string& fmt, Targs&&... args)
{
    bool in_brackets = false;
    int arg_n = -1;
    auto plain_begin = fmt.begin();
    auto plain_end = fmt.end();
    std::stringstream ss;
    for(auto c = fmt.begin(); c != fmt.end(); ++c) {
        if(*c == '{') {
            if(!in_brackets) {
                plain_end = c;
                ss << std::string(plain_begin, plain_end);
                in_brackets = true;
                arg_n = -1;
            } else {
                throw std::runtime_error("format: expected a number in { }");
            }
        } else if(*c == '}') {
            if(in_brackets) {
                in_brackets = false;
                plain_begin = c + 1;
                if(arg_n != -1) {
                    if(!print_arg_n(ss, arg_n, std::forward<Targs>(args)...)) {
                        throw std::runtime_error("format: argument out of range");
                    }
                } else {
                    throw std::runtime_error("format: expected a number in { }");
                }
            } else {
                throw std::runtime_error("format: expected '{' before '}'");;
            }
        } else if(in_brackets) {
            if(std::isdigit(*c)) {
                if(arg_n == -1) {
                    arg_n = 0;
                }
                arg_n = 10 * arg_n + ((*c) - '0');
            } else {
                throw std::runtime_error("format: expected a number in { }");
            }
        }
    }
    if(!in_brackets) {
        ss << std::string(plain_begin, fmt.end());
    } else {
        throw std::runtime_error("format: unexpected end of format string");
    }
    return ss.str();
}
