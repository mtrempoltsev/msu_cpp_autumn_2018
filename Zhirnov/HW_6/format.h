#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <stdlib.h>

template <class T>
std::string to_string(T&& arg) {
    std::ostringstream os;
    os << arg;
    return os.str();
}

template<class... ArgsT>
std::string format(const std::string& str, ArgsT&&... args) {
    
    std::vector<std::string> strArgs{to_string(std::forward<ArgsT>(args))...};
    
    std::ostringstream result;
    
    for (int i = 0; i < str.length(); ++i) {
        
        if (str[i] == '{') {
            int acc = 0;
            ++i;
            if (str[i] == '}') {
                throw std::runtime_error("error");
            }
            
            while (i < str.length() && str[i] != '}') {
            
                if (isdigit(str[i])) {
                    acc *= 10;
                    acc += str[i] - '0';
                } else {
                    throw std::runtime_error("error");
                }
                ++i;
            
            }
            if (i == str.length()) {
                throw std::runtime_error("error");
            }
            if (acc >= strArgs.size()) {
                throw std::runtime_error("error");
            }
            result << strArgs[acc];
        } else if (str[i] == '}') {
        
            throw std::runtime_error("error");
        
        } else {
            result << str[i];
        }
    }
    return result.str();
}
