#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cmath>

using namespace std;

template <class T> string to_string(T&& arg)
{
    stringstream temp;
    temp << arg;
    
    return temp.str();
}




template<class... ArgsT> string format(const string& str, ArgsT&&... args) 
{
    vector<string> arg = {to_string(forward<ArgsT>(args))...};
    ostringstream out_stream;

    for (int i = 0; i < str.length(); i++) 
    {
        if (str[i] == '{') 
        {
           int next_bracket = str.substr(i).find('}') + i;
            
            if (next_bracket == string::npos)
                throw runtime_error("");

            int number = 0;
            
            int next_br_pow = pow(10, next_bracket-(i+2));
            
            for (int j = i+1; j < next_bracket; j++) 
            {
                char chr_number = str[j];
                
               // if (str[j]="}")
                   // break;
                    
                if (chr_number < '0' || chr_number > '9' || 
                    (chr_number - '0') >= arg.size())
                    throw runtime_error("");
                
               
                number += (chr_number - '0')*next_br_pow;
                next_br_pow /= 10;
            }

            if (number >= arg.size())
                throw runtime_error("");

            out_stream << arg[number];
            i += next_bracket - i;
            
        }
        else 
        {
            if (str[i] == '}')
                throw runtime_error("");

            out_stream << str[i];
        }
    }
    
    return out_stream.str();
}
