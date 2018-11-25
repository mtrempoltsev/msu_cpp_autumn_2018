#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>


template<class T>
std::string to_string(T&& arg)
{
    std::stringstream out;
    out << arg;
    return out.str();
}

std::string format(const std::string& s)
{
    if(s.find('{') != std::string::npos || s.find('}') != std::string::npos)
        throw std::runtime_error("Error");

    return s;
}

template<class... ArgsT>
std::string format(const std::string& fmt, ArgsT&&... args)
{
    std::vector<std::string> argv {to_string(std::forward<ArgsT>(args))...};

    std::string res;
    size_t i = 0;
    while(i < fmt.size())
    {
        switch(fmt[i])
        {
            case '{':
            {
                std::string tmp;
                i++;
                while(isdigit(fmt[i]) && i < fmt.size())
                    tmp += fmt[i++];

                if(i == fmt.size() || fmt[i] != '}' || tmp.empty())
                    throw std::runtime_error("Error");

                int index = atoi(tmp.c_str());
                if(index < argv.size() && index >= 0)
                    res += argv[atoi(tmp.c_str())];
                else
                    throw std::runtime_error("Error");

                i++;
                break;
            }
            case '}':
                throw std::runtime_error("Error");
            default:
                res += fmt[i++];
        }
    }

    return res;
}
