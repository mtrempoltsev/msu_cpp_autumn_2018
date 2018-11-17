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
    int arg_n = -1;
    std::stringstream ss;
    for ( ; c = *fmt_ptr; ++fmt_ptr) {
        if(c == '{') {
            if(!in_brackets) {
                *fmt_ptr = '\0';
                ss << fmt_plain;
                in_brackets = true;
                arg_n = -1;
            } else {
                delete[] fmt_copy;
                throw std::runtime_error("format: expected a number in { }");
            }
        } else if(c == '}') {
            if(in_brackets) {
                in_brackets = false;
                fmt_plain = fmt_ptr + 1;
                if(arg_n != -1) {
                    if(!print_arg_n(ss, arg_n, std::forward<Targs>(args)...)) {
                        delete[] fmt_copy;
                        throw std::runtime_error("format: argument out of range");
                    }
                } else {
                    delete[] fmt_copy;
                    throw std::runtime_error("format: expected a number in { }");
                }
            } else {
                delete[] fmt_copy;
                throw std::runtime_error("format: expected '{' before '}'");
            }
        } else if(in_brackets) {
            if(std::isdigit(c)) {
                if(arg_n == -1) {
                    arg_n = 0;
                }
                arg_n = 10 * arg_n + (c - '0');
            } else {
                delete[] fmt_copy;
                throw std::runtime_error("format: expected a number in { }");
            }
        }
    }
    if(!in_brackets) {
        ss << fmt_plain;
    } else {
        delete[] fmt_copy;
        throw std::runtime_error("format: unexpected end of format string");
    }
    delete[] fmt_copy;
    return ss.str();
}
