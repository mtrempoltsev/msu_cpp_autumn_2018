#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>


template<class T>
std::string to_string(T&& arg)
{
    std::ostringstream ostream;
    ostream << arg;
    return ostream.str();
}

template<class... T>
std::string format(const std::string& str, T&&... args)
{
    std::vector<std::string> str_args = {to_string(std::forward<T>(args))...};
    std::ostringstream res;

    for (size_t i = 0; i < str.length(); i++)
    {

        if (str[i] == '}')
            throw std::runtime_error("no left bound");

        if (str[i] == '{')
        {
            i++;
            size_t arg_num = 0;

            if (str[i] == '}')
                throw std::runtime_error("no arg");

            while ((str[i] != '}') && (i < str.length()))
            {
                arg_num = 10 * arg_num + str[i] - '0';
                i++;
            }

            if (i >= str.length())
                throw std::runtime_error("no right bound");

            if (arg_num > str_args.size() - 1)
                throw std::runtime_error("arg not exist");

            res << str_args[arg_num];
            continue;
        }

        res << str[i];
    }

    return res.str();
}
