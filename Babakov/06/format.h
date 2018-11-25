#include <string>
#include <iostream>
#include <sstream>
#include <vector>

template <class T>
std::string arg_to_string (T&& arg)
{
    std::stringstream stream;
    stream << arg;
    return stream.str();
}

template <class... ArgsT>
std::string format(const std::string& inp_str, ArgsT&& ... args)
{
    int cur_index = 0;
    size_t cur_num = 0;
    bool bracket_existence = false;
    if ((cur_index = inp_str.find("{", cur_index)) == std::string::npos)
        if (inp_str.find("}", 0) == std::string::npos)
            return inp_str;
        else
            throw std::runtime_error("Got wrong expression-0");
    while ((cur_index = inp_str.find("{", cur_index)) != std::string::npos) {
        bracket_existence = true;
        if (++cur_index == inp_str.length()) {
            throw std::runtime_error("Got wrong expression-1");
        } else {
            if (inp_str[cur_index] >= '0' &&  inp_str[cur_index] <= '9') {
                if (inp_str[cur_index] - '0' > cur_num)
                    cur_num = inp_str[cur_index] - '0';
            } else {
                throw std::runtime_error("Got wrong expression-2");
            }
            
            if (++cur_index == inp_str.length() || inp_str[cur_index] != '}')
                throw std::runtime_error("Got wrong expression-3");
        }
    }
    std::vector<std::string> arg_vector{ arg_to_string(std::forward<ArgsT>(args))... };
    if (cur_num + 1 > arg_vector.size())
        throw std::runtime_error("Not enough variables");
    std::string out_str;
    out_str = inp_str;
    int prev_index = -1;
    std::string desired_str;
    for (size_t i = 0; i < arg_vector.size(); i++) {
        cur_index = 0;
        desired_str = "{" + std::to_string(i) + "}";
        while ((cur_index = out_str.find(desired_str, cur_index))
                != prev_index) {
            prev_index = cur_index;
            if (cur_index != std::string::npos)
                out_str.replace(cur_index, desired_str.size(), arg_vector[i]);
        }
    }
    return out_str;
}
