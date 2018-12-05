#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <ctype.h>
#include <cstdlib>

template <class T>
std::string to_str(T&& x)
{
    std::stringstream buf;
    buf << x;

    return buf.str();
}


std::string format(const char* str)
{
    for (auto iter = str; *iter != '\0'; ++iter)
        if (*iter == '{' || *iter == '}')
            throw std::runtime_error("Wrong format");
    return str;
}

size_t to_num(char c)
{
    return c - '0';
}

template <class... ArgsT>
std::string format(const char* str, ArgsT&&... args)
{
    std::string format_str;

    std::vector<std::string> arguments{to_str(std::forward<ArgsT>(args))...};

    for (auto iter = str; *iter != '\0'; ++iter)
    {
        if (*iter != '{' && *iter != '}')
            format_str.push_back(*iter);

        else if (*iter == '{')
        {
            ++iter;

            if (*iter == '}')
                throw std::runtime_error("No number in {}");

            size_t ind = 0;
            for (; *iter != '}' && *iter != '\0'; ++iter)
            {
                if (isdigit(*iter) == 0)
                    throw std::runtime_error("Wrong input in {}");

                ind *= 10;
                ind += to_num(*iter);
            }

            if (*iter != '}')
                throw std::runtime_error("No }");

            if (ind >= arguments.size())
                throw std::runtime_error("Too less arguments");

            format_str.append(arguments[ind]);
        }

        else
            throw std::runtime_error("No opening bracket");
    }

    return format_str;
}