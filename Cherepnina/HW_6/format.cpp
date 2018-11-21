#include <iostream>
#include <vector>
#include <sstream>

int search(const std::string &str, int &i) {
    int digit = 0;
    for (i; i < str.length(); ++i)
        switch (str[i]) {
            case '0'...'9':
                digit = digit * 10 + str[i] - '0';
                continue;
            case '}':
                return digit;
            default:
                throw std::runtime_error("incorrect input");
        }
    throw std::runtime_error("incorrect input");
}

template<typename... Args>
std::string to_string(Args &&... args) {
    std::stringstream sstr;
    (sstr << ... << args );
    return sstr.str();
}

std::string format(const std::string &str) {
    for (int i = 0; i < str.length(); ++i)
        switch (str[i]) {
            case '{':
            case '}':
                throw std::runtime_error("incorrect number of arguments");
        }
    return str;
}

template<class... Args>
std::string format(std::string &&str, Args &&... args) {

    std::vector<std::string> argv{to_string(std::forward<Args>(args))...};

    size_t new_str_len = str.length();
    for (int i = 0; i < argv.size(); ++i) {
        new_str_len += argv[i].length();
    }

    std::string formatted_string;

    int new_i = 0;
    for (int i = 0; i < str.length(); ++i, ++new_i) {
        if (str[i] == '{') {
            int number = search(str, ++i);
            if (number + 1 > argv.size())
                throw std::runtime_error("incorrect number of arguments");

            formatted_string.append(argv[number]);
        } else if (str[i] == '}')
            throw std::runtime_error("incorrect input");
        else
            formatted_string.push_back(str[i]);
    }
    return formatted_string;
}
