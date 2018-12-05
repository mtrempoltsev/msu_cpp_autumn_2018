#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <cmath>


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

    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '{') {
            int next_bracket = str.substr(i).find('}') + i;
            if (next_bracket == std::string::npos)
                throw std::runtime_error("");

            int number = 0;
            for (int j = i+1; j < next_bracket; j++) {
                char number_chr = str[j];
                if (number_chr < '0' || number_chr > '9' || 
                    (number_chr - '0') >= arguments.size())
                    throw std::runtime_error("");

                number += (number_chr - '0')*std::pow(10, next_bracket-j-1);
            }

            if (number >= arguments.size())
                throw std::runtime_error("");

            out_stream << arguments[number];
            i += next_bracket - i;
        } else {
            if (str[i] == '}')
                throw std::runtime_error("");

            out_stream << str[i];
        }
    }
    return out_stream.str();
}