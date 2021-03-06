#pragma once
#include <cstddef> //size_t
#include <cctype> //isdigit
#include <sstream> //ostringstream
#include <stdexcept> //runtime_error
#include <string>
#include <vector>

template <class Arg>
std::string to_string(Arg && arg)
{
    std::ostringstream str;
    str << arg;
    return str.str();
}

size_t readnum(const std::string & str, size_t start, size_t & result)
{
    int i = start;
    for (result = 0; i < str.size() && isdigit(str[i]); ++i)
        result = result * 10 + str[i] - '0';
    if (i == start || i == str.size() || str[i] != '}')
        throw std::runtime_error("error");
    return i + 1;
}

template <class ... Args>
std::string format(const std::string & str, Args &&... args)
{
    std::string result;
    std::vector<std::string> strings{to_string(std::forward<Args>(args))...};

    size_t last_index = 0;
    size_t index = str.find_first_of("{}");
    if (index != std::string::npos && strings.empty())
        throw std::runtime_error("error");

    while (index != std::string::npos) {
        size_t num;
        if (str[index] == '}')
            throw std::runtime_error("error");
        result += str.substr(last_index, index - last_index);
        last_index = readnum(str, index + 1, num);
        index = str.find_first_of("{}", last_index);

        if (num < strings.size())
            result += strings[num];
        else
            throw std::runtime_error("error");
    }

    return result + str.substr(last_index);
}
