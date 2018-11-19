#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>

void getPar(std::vector<std::string>& vec) {}

template <class T>
void getPar(std::vector<std::string>& vec, T&& val)
{
    std::ostringstream s;
    s << val;
    vec.push_back(s.str());
}

template <class T, class... Args>
void getPar(std::vector<std::string>& vec, T&& val, Args&&... args)
{
    std::ostringstream s;
    s << val;
    vec.push_back(s.str());
    getPar(vec, std::forward<Args>(args)...);
}

template <class ... Args>
std::string format(const std::string str, Args&&... args)
{
    std::vector<std::string> vec(0);
    getPar(vec, std::forward<Args>(args)...);
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
