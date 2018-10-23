#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <stdexcept>

using namespace std;

#define __DBG__ 0

/*
S -> S + T | S - T |  T
T -> T * E | T / E | E
E -> int64_t
*/

class Calc
{
    public:
        int64_t run(const string& s)
        {
            return calc_S(s);
        }
    private:    
        bool is_number(const string& s)
        {
            auto it = s.begin();
            while (it != s.end() && isdigit(*it))
                ++it;
            return !s.empty() && it == s.end();
        }
        string trim(const string& str)
        {
            size_t first = str.find_first_not_of(' ');
            if (string::npos == first)
                return str;
            size_t last = str.find_last_not_of(' ');
            return str.substr(first, (last - first + 1));
        }

        int64_t check_if_zero(int64_t value){
            if (value == 0)
                 throw invalid_argument("division by zero");
            else 
                return value;
        }
    
        int64_t calc_S(const string& str)
        {
            if (__DBG__) cout << "calc_S: <<" << str << ">>" << endl;
            int i;
            if ((i = str.rfind('+')) != -1)
                return calc_S(str.substr(0, i)) + calc_S(str.substr(i+1, str.size()));
            if ((i = str.rfind('-')) != -1){
                int j = i-1;
                while ((j>0) && str[j]==' ') j--;
                if (__DBG__) cout <<str[j] << " " << str.substr(0, j) << endl;
                if (str[j] == '-')
                    return calc_S(str.substr(0, j)) - (-1)*calc_S(str.substr(i+1, str.size()));
                else if (str[j] == '+')
                    return calc_S(str.substr(0, j)) + (-1)*calc_S(str.substr(i+1, str.size()));
                else if (str[j] == '*')
                    return calc_S(str.substr(0, j)) * (-1)*calc_S(str.substr(i+1, str.size()));
                else if (str[j] == '/')
                    return (-1)*calc_S(str.substr(0, j)) / check_if_zero(calc_S(str.substr(i+1, str.size())));
                else
                    return calc_S(str.substr(0, i)) - calc_S(str.substr(i+1, str.size()));
            }
            return calc_T(str);
        }

        int64_t calc_T(const string& str)
        {
            if (__DBG__) cout << "calc_T: <<" << str << ">>" << endl;
            int i;
            if ((i = str.find('*')) != -1)
                return calc_T(str.substr(0, i)) * calc_T(str.substr(i+1, str.size()));
            if ((i = str.find('/')) != -1)
                return calc_T(str.substr(0, i)) / check_if_zero(calc_T(str.substr(i+1, str.size())));
            return calc_E(str.substr(i+1, str.size()));
        }

        int64_t calc_E(const string& str)
        {
            if (__DBG__) cout << "calc_E: <<" << str << ">>" << endl;
            if (str == "")
                return 0;
            if (is_number(trim(str)))
                return stoi(str);
            else
                throw invalid_argument("str is not a num");
        }
};


int main(int argc, char* argv[]){
    if (argc!=2){
        cout << "error" << endl;
        return 1;
    }
    auto s = string(argv[1]);
    Calc* calc = new Calc();
    try{
        cout << calc->run(s) << endl;
    }
    catch(exception& e){
        cout << "error" << endl;
        return 1;
    }
    return 0;
}