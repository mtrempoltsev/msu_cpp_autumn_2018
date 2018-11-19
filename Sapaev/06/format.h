#include <iostream>
#include <sstream>
#include <string>

template<class ArgT>
std::string to_string(const ArgT& arg)
{
    std::stringstream tmp;
    tmp << arg;
    return tmp.str();
}

template<class Arg>
std::string& format_index(std::string& str, int index, Arg&& Head)
{
    std::string str_to_replace = "{" + to_string(index) + "}";
    size_t pos = str.find(str_to_replace);
    size_t size = str_to_replace.size();
    while (pos != std::string::npos) {
        str.replace(pos, size, to_string(Head));
        pos = str.find(str_to_replace);
    }
    return str;
}

template<class Arg, class... ArgsT>
std::string& format_index(std::string& str, int index, Arg&& Head, ArgsT&&... Tail)
{
    std::string str_to_replace = "{" + to_string(index) + "}";
    size_t pos = str.find(str_to_replace);
    size_t size = str_to_replace.size();
    while (pos != std::string::npos) {
        str.replace(pos, size, to_string(Head));
        pos = str.find(str_to_replace);
    }
    return format_index(str, index + 1, std::forward<ArgsT>(Tail)...);
}

std::string& format(std::string&& str)
{
    if(str.find("{") != std::string::npos) {
        throw std::runtime_error("{ in wrong place");
    }
    if(str.find("}") != std::string::npos) {
        throw std::runtime_error("} in wrong place");
    }
    return str;
}

template<class... ArgsT>
std::string& format(std::string&& str, ArgsT&&... Tail)
{
    format_index(str, 0, std::forward<ArgsT>(Tail)...);
    if(str.find("{") != std::string::npos) {
        throw std::runtime_error("{ in wrong place");
    }
    if(str.find("}") != std::string::npos) {
        throw std::runtime_error("} in wrong place");
    }
    return str;
}
