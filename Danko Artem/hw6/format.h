#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <sstream>

namespace std {
    template<class T>
    std::string to_string(T&& s){
        std::stringstream ss;
        ss<<s;
        return ss.str();
    }
}

template <class... ArgsT>
std::string format(const char* a, ArgsT... args){
    std::vector<std::string> sargs({std::to_string(std::forward<ArgsT>(args))...});
    std::stringstream ss;
    for (auto i = 0; a[i] != '\0'; i++) {
        if (a[i] == '}')
            throw std::runtime_error("where is my {?");
        if (a[i] == '{') {
            int ans = 0;
            while (a[++i] != '}') {
                if (a[i] <= '9' && a[i] >= '0') {
                    ans = ans * 10 + a[i] - '0';
                } else {
                    throw std::runtime_error("where is my }?");
                }
            }
            if (ans >=sargs.size())
                throw std::runtime_error("what is the number?");
            ss << sargs[ans];
        }
        else {
            ss << a[i];
        }
    }
    return ss.str();
}
