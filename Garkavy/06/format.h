#pragma once
#include <iostream>
#include <sstream>
#include <string.h>

void print_arg_n(std::ostream& os, int n)
{
    throw std::runtime_error("format: argument out of range");
}

template<typename T, typename... Targs>
void print_arg_n(std::ostream& os, int n, T&& first, Targs&&... rest)
// print_arg_n(os, n, ...) печатает n-ый аргумент из (...) в os
{
    if(n == 0) {
        os << first;
    } else if(n > 0) {
        print_arg_n(os, n - 1, std::forward<Targs>(rest)...);
    } else {
        throw std::runtime_error("format: argument out of range");
    }
}

template<typename... Targs>
std::string format(const char* fmt, Targs&&... args)
{
    size_t fmt_len = strlen(fmt);
    char* fmt_copy = new char[fmt_len + 1];
    strncpy(fmt_copy, fmt, fmt_len);
    fmt_copy[fmt_len] = '\0';
    // без этого ^ присваивания в тесте на сайте функция залезает в память вне строки,
    // а при компиляции test.cpp вручную всё проходит удачно
    char* fmt_plain = fmt_copy;
    char* fmt_ptr = fmt_copy;
    char c;
    bool in_brackets = false;
    int buf = -1;
    std::stringstream ss;
    for ( ; c = *fmt_ptr; ++fmt_ptr) {
        if(c == '{') {
            if(!in_brackets) {
                *fmt_ptr = '\0';
                ss << fmt_plain;
                in_brackets = true;
                buf = -1;
            } else {
                throw std::runtime_error("format: expected a number in { }");
            }
        } else if(c == '}') {
            if(in_brackets) {
                in_brackets = false;
                fmt_plain = fmt_ptr + 1;
                if(buf != -1) {
                    print_arg_n(ss, buf, std::forward<Targs>(args)...);
                } else {
                    throw std::runtime_error("format: expected a number in { }");
                }
            } else {
                throw std::runtime_error("format: expected '{' before '}'");
            }
        } else if(in_brackets) {
            if(std::isdigit(c)) {
                if(buf == -1) {
                    buf = 0;
                }
                buf = 10 * buf + (c - '0');
            } else {
                throw std::runtime_error("format: expected a number in { }");
            }
        }
    }
    if(!in_brackets) {
        ss << fmt_plain;
    } else {
        throw std::runtime_error("format: unexpected end of format string");
    }
    delete[] fmt_copy;
    return ss.str();
}
