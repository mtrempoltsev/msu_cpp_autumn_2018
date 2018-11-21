#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>

template <class T>
std::string to_str(T&& val){
    std::ostringstream s;
    s << val;
    return s.str();
}

template <class ... Args>
std::string format(const std::string& str, Args&&... args)
{
    std::vector<std::string> vec{to_str(std::forward<Args>(args))...};
    std::ostringstream newstr;
    bool isNumNow = false;
    int ind = 0;
    for(auto i : str)
        if(i =='{')
            if(isNumNow)
                throw std::runtime_error("");
            else
                isNumNow = true;
        else if(i == '}')
        {
            if(!isNumNow)
                throw std::runtime_error("");
            if(ind < 0)
                throw std::runtime_error("");
            isNumNow = false;
            if(ind > vec.size() - 1)
                throw std::runtime_error("");
            newstr << vec[ind];
            ind = -1;
        }
        else if(isNumNow)
            if((i < '9' + 1) && (i > '0' - 1))
                if(ind < 0)
                    ind = i - '0';
                else
                    ind = ind * 10 + i - '0';
            else
                throw std::runtime_error("");
        else
            newstr << i;
    if(isNumNow)
        throw std::runtime_error("");
    return newstr.str();
}
