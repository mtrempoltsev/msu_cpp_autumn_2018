#include <iostream>
#include <sstream>
#include <string>
#include <vector>

template<class ArgT>
std::string to_string(const ArgT& arg)
{
    std::stringstream tmp;
    tmp << arg;
    return tmp.str();
}

int my_atoi (const std::string& str)
{
    int tmp = 0;
    for (size_t i = 0; i < str.size(); ++i) {
        if (!isdigit(str[i])) {
            throw std::runtime_error("Incorrect symbol in number");
        }
        tmp = 10 * tmp + str[i] - '0';
    }
    return tmp;
}

std::string format_with_vector(const std::string& str,
                               const std::vector<std::string>& formats)
{
    enum State {NORMAL, BRACKET};
    State state = NORMAL;
    std::stringstream sstr;
    std::string buf = "";
    
    for (size_t i = 0; i < str.size(); ++i) {
        int vi;
        switch (state) {
            case NORMAL:
                if (str[i] == '{') {
                    state = BRACKET;
                    buf = "";
                    continue;
                } else if (str[i] == '}') {
                    throw std::runtime_error("Brackets are not opened yet");
                } else {
                    sstr << str[i];
                }
                break;
                
            case BRACKET:
                if (str[i] == '}') {
                    vi = my_atoi(buf.c_str());
                    if (vi >= formats.size()) {
                        throw std::runtime_error("Out of Range");
                    }
                    sstr << formats[vi];
                    buf = "";
                    state = NORMAL;
                    continue;
                } else if (str[i] == '{') {
                    throw std::runtime_error("Brackets are already opened");
                } else {
                    buf += str[i];
                }
                break;
        }
    }
    if (state == BRACKET) {
        throw std::runtime_error("Brackets are not closed");
    }
    return sstr.str();
}

template<class... ArgsT>
std::string format(const std::string& arg_str, ArgsT&&... Tail)
{
    std::vector<std::string> formats{to_string(std::forward<ArgsT>(Tail))...};
    return format_with_vector(arg_str, formats);
}
