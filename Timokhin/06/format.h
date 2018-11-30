#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <sstream>

void getParametrs(std::vector<std::string>& results)
{
    return;
}

template<typename T, typename ...Args>
void getParametrs(std::vector<std::string>& results, const T& t, const Args&... args)
{
    std::stringstream ss;
    ss << t;
    results.push_back(ss.str());
    getParametrs(results, args...);
}

template<typename ...Args>
std::string format(const std::string& f, const Args&... args)
{
    std::string result;
    std::vector<std::string> parametrs;
    getParametrs(parametrs, args...);
    std::string fmt(f);
    size_t paramStart = fmt.find_first_of('{');
    size_t paramEnd = fmt.find_first_of('}');
    while (paramStart != fmt.npos)
    {
        if (paramEnd == fmt.npos || paramStart > paramEnd)
            throw std::runtime_error("Braces is not balanced");
        size_t value = 0;
        for (size_t i = paramStart+1; i < paramEnd; ++i)
        {
            if (!isdigit(fmt[i]))
                throw std::runtime_error("Wrong argument");
            value *= 10;
            value += fmt[i] - '0';
        }
        if (value >= parametrs.size())
            throw std::runtime_error("Wrong argument");
        result += fmt.substr(0, paramStart);
        result += parametrs[value];
        fmt = fmt.substr(paramEnd + 1);
        paramStart = fmt.find_first_of('{');
        paramEnd = fmt.find_first_of('}');
    }
    if (paramEnd != fmt.npos)
            throw std::runtime_error("Braces is not balanced");
    result += fmt;
    return result;
}
