#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

template <class T>
std::string to_string(T&& arg) {
    std::ostringstream buf;
    buf << arg;
    return buf.str();
}

template <class... Args>
std::string format(std::string str, Args&&... arg) {
    std::vector<std::string> args = {to_string(std::forward<Args>(arg))...};
    std::istringstream in(str);
    std::ostringstream out;
    std::string result;
    int c;
    while ((c = in.get()) != EOF) {
        if (c == '{') {	
            int number = 0;
            char c;
            c = in.get();
            while ((c - '0' >= 0) && (c - '0' <= 9)) {
                number = number * 10 + c - '0';
                c = in.get();
            }
            if (number < args.size()) {   
                result += args[number];
            } else {
                throw std::runtime_error("");
            }
            if (c != '}') {
                throw std::runtime_error("");
            }
        } else {
            if (c == '}') {
                throw std::runtime_error("");
            }
            result += c;
        }
    }
    return result;
}
