#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>


template <class T>
void unroll(std::vector<std::string> &arguments, T&& x)
{
    std::stringstream buf;
    buf << x;

    arguments.push_back(buf.str());
}


template <class T, class... ArgsT>
void unroll(std::vector<std::string> &arguments, T&& x, ArgsT&&... args)
{
    std::stringstream buf;
    buf << x;

    arguments.push_back(buf.str());
    unroll(arguments, std::forward<ArgsT>(args)...);
}


inline bool isNumber(const char ch)
{
    return ch >= '0' && ch <= '9';
}


inline size_t toNumber(const char ch)
{
    return ch - '0';
}


std::string format(const char* str)
{
    for (auto iter = str; *iter != '\0'; ++iter)
        if (*iter == '{' || *iter == '}')
            throw std::runtime_error("Wrong format");
    return str;
}


template <class... ArgsT>
std::string format(const char* str, ArgsT&&... args)
{
    std::string format_str;

    std::vector<std::string> arguments;
    unroll(arguments, std::forward<ArgsT>(args)...);

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
                if (isNumber(*iter) == false)
                    throw std::runtime_error("Wrong input in {}");

                ind *= 10;
                ind += toNumber(*iter);
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