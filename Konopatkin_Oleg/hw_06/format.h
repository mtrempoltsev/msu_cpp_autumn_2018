#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cstring>


int get_label(std::istringstream& s, int& cur) {
    int res = 0;
    // necessary to read space symbols...
    while (((cur = s.get()) != EOF) && isdigit(cur)) {
        res = res * 10 + (cur - '0');
    }
    return res;
}

std::string format(const std::string& fmt) {
    std::istringstream in(fmt);
    std::ostringstream out;
    int cur;
    while ((cur = in.get()) != EOF) {
        if (cur == '{' || cur == '}') {
            throw std::runtime_error("Format error");
        } else {
            out << static_cast<char>(cur);
        }
    }

    return out.str();
}

template <class T, class... ArgsT>
std::string format(const std::string& fmt, T&& fst, ArgsT&&... args) {
    std::istringstream in(fmt);
    std::ostringstream out;
    int cur;
    while ((cur = in.get()) != EOF) {
        if (cur == '{') {
            int label = get_label(in, cur);
            if (cur != '}') {
                throw std::runtime_error("Format error");
            }
            if (label == 0) {
                out << fst;
            } else {
                out << '{' << (label - 1) << '}';
            }
        } else if (cur == '}') {
            throw std::runtime_error("Format error");
        } else {
            out << static_cast<char>(cur);
        }
    }

    return format(out.str(), std::forward<ArgsT>(args)...);
}