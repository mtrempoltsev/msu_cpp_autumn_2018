#include <iostream>
#include <sstream>
#include <vector>
#include <string>

template<class T>
std::string get_args(T&& arg)
{
    std::stringstream ss;
    ss << arg;
    return ss.str();
}

template<class... ArgsT>
std::string format(const std::string& str, ArgsT&&... args)
{
    std::vector<std::string> str_args({get_args(std::forward<ArgsT>(args))...});
    std::string ans{};

    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '}') {
            throw std::runtime_error("wrong branches");
        }

        if (str[i] == '{') {
            i++;
            int pos = 0;
            while (i < str.length() && str[i] != '}') {
                if (isdigit(str[i])) {
                    pos = pos * 10 + str[i] - '0';
                } else {
                    throw std::runtime_error("wrong position value");
                }
                i++;
            }

            if (i == str.length()) {
                throw std::runtime_error("wrong branches");
            }

            if (pos >= str_args.size()) {
                throw std::runtime_error("wrong number of arguments");
            }
            ans += str_args[pos];
        } else {
            ans += str[i];
        }
    }
    return ans;
}
