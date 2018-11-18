#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>

template<class T>
void parse_args(std::vector<std::string>& strs, T&& arg)
{
    std::ostringstream ostream;
    ostream << arg;
    strs.push_back(ostream.str());
}

template<class T, class... Args>
void parse_args(std::vector<std::string>& strs, T&& arg, Args&&... args)
{
    parse_args(strs, std::forward<T>(arg));
    parse_args(strs, std::forward<Args>(args)...);
}

std::string format_(const std::string& string_to_format, std::vector<std::string>& strings_to_insert){
    std::ostringstream ostream;
    for(size_t pos = 0; pos < string_to_format.length(); ++pos){
        if(string_to_format[pos] == '}')
            throw std::runtime_error("Wrong format: closing brace before opening");

        if(string_to_format[pos] == '{'){
            int pos_of_string = 0;
            ++pos;
            if(pos == string_to_format.length())
                throw std::runtime_error("Wrong format: closing brace not found");
            if(string_to_format[pos] == '}')
                throw std::runtime_error("Wrong format: empty braces");
            while(isdigit(string_to_format[pos])){
                pos_of_string = pos_of_string * 10 + string_to_format[pos] - '0';
                ++pos;
                if(pos == string_to_format.length())
                    throw std::runtime_error("Wrong format: closing brace not found");
            }
            if(pos_of_string > strings_to_insert.size() - 1)
                throw std::runtime_error("Wrong format: too big argument number");
            if(string_to_format[pos] != '}')
                throw std::runtime_error("Wrong format: closing brace not found");

            ostream << strings_to_insert[pos_of_string];
        }
        else
            ostream << string_to_format[pos];
    }
    return ostream.str();
}

std::string format(const std::string& string_to_format)
{
    return string_to_format;
}

template <class... Args>
std::string format(const std::string& string_to_format, Args&&... args)
{
    std::vector<std::string> strings_to_insert;
    parse_args(strings_to_insert, std::forward<Args>(args)...);
    return format_(string_to_format, strings_to_insert);
}