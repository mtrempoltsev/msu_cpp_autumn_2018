#include <iostream>
#include <string>
#include <stdexcept>

template <class T>
std::string to_string(T&& arg) {
    std::ostringstream buf;
    buf << arg;
    return buf.str();
}

template <class... Args>
std::string format(std::string str, Args&&... arg) {
    std::string args[] = {to_string(std::forward<Args>(arg))...};
    std::istringstream in(str);
    std::ostringstream out;
    std::string result;
    int c;
    while ((c = in.get()) != EOF) {
        if (c == '{') {	
            char number;
            number = in.get();
	    int size = sizeof(args)/sizeof(args[0]);
            if (number - '0' < size) {   
                result += args[number - '0'];
            } else {
                throw std::runtime_error("");
            }
            number = in.get();
            if (number != '}') {
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
