#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

template <class T>
std::string as_string(T&& arg) {
    std::ostringstream buffer;
    buffer << arg;
    return buffer.str();
}

template <class... Args>
std::string format(std::string fmt, Args&&... arg) {
    std::istringstream in(fmt);
    std::ostringstream out;
    std::string res;
    int c;

    std::vector<std::string> args = {as_string(std::forward<Args>(arg))...};
    
    while ((c = in.get()) != EOF) {
        if (c == '{') {	
            int num = 0;
            char c;
            c = in.get();
            while (isdigit(c)) {
                num = num * 10 + c - '0';
                c = in.get();
            }
            if (num < args.size()) {   
                res += args[num];
            } else {
                throw std::runtime_error("error");
            }
            if (c != '}') {
                throw std::runtime_error("error");
            }
        } else {
            if (c == '}') {
                throw std::runtime_error("error");
            }
            res += c;
        }
    }
    return result;
}
