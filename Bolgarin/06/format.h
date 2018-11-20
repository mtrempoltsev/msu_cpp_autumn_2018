#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>


template <class T>
std::string arg_to_str(T&& arg) {
    std::stringstream temp_stream;
    temp_stream << arg;
    return temp_stream.str();
}

template<class... ArgsT>
std::string format(const std::string& str, ArgsT&&... args) {
    std::vector<std::string> arguments = {arg_to_str(std::forward<ArgsT>(args))...};
    std::ostringstream out_stream;

    char number_chr = '0';
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '{') {
            if (i+2 >= str.length() || str[i+2] != '}')
                throw std::runtime_error("");

            number_chr = str[i+1];
            if (number_chr < '0' || number_chr > '9' || 
                (number_chr - '0') >= arguments.size())
                throw std::runtime_error("");

            out_stream << arguments[number_chr - '0'];
            i += 2;
        } else {
            if (str[i] == '}')
                throw std::runtime_error("");

            out_stream << str[i];
        }
    }
    return out_stream.str();
}