#include <iostream>
#include <string>
#include <stdint.h>
#include <stdexcept>
#include <algorithm>

using namespace std;

class Calculator {
    string s;

    int64_t do_mult(string str) const;
    int64_t do_add(string str) const;
    void del_spaces();
    bool check() const;

    public:
        Calculator(char* argv): s(string(argv)) {    
        }
        int64_t calculate();
};

void Calculator:: del_spaces() {
    s.erase (remove (s.begin(), s.end(), ' '), s.end());
} 

bool Calculator::check() const{
    for (int64_t i = 0; i < s.size(); ++i) {
        if (!((s[i] == '+') || (s[i] == '-') || (s[i] == '*') || (s[i] == '/') || ((s[i] >= '0') && (s[i] <= '9')))) {
            return false;
        }
    }
    for (int64_t i = 0; i < s.size(); ++i) {
        if ((s[i] == '*') || (s[i] == '/') || (s[i] == '+') || (s[i] == '-')) {
            if (i == s.size()-1) 
                return false;
                if ((s[i+1] != '-') && ((s[i+1] < '0') || (s[i+1] > '9')))
                    return false;
            if (s[i] != '-') {
                if (i == 0) 
                    return false;
                if ((s[i-1] > '9') || (s[i-1] < '0')) 
                    return false;
            }
        }
    }
    return true;
}

int64_t Calculator::do_mult(string str) const{
    bool op_exist = false;
    bool minus = false;
    char op;
    string s1 = "";
    int64_t number=0;
    int64_t i;
    for (i = str.size()-1; i >=0; --i) {
        if ((str[i] == '*') || (str[i] == '/')) {
            op_exist = true;
            op = str[i];
            break;
        }
    }
    if (op_exist == false) {
        if (str[0] == '-') minus = true;
        for (int64_t j = 0+minus; j < str.size(); ++j) {
            number = number*10+(str[j]-'0');
        }
        if (minus) 
            return (-1)*number;
        else 
            return number;
    }
    
    if (str[i+1] == '-') minus = true;
    for (int64_t j = i+1+minus; j < str.size(); ++j) {
        number = number*10+(str[j]-'0');
    }
    s1.append(str.begin(), str.begin()+i);
    if (op == '*') {
        if (minus) 
            return do_mult(s1)*number*(-1);
        else
            return do_mult(s1)*number;
    }
    else {
        if (number == 0) {
            throw invalid_argument("");
        }
        else {
            if (minus) 
                return (-1)*do_mult(s1)/number;
            else
                return do_mult(s1)/number;
        }
    }
}    

int64_t Calculator::do_add(string str) const{
    bool op_exist = false;
    char op;
    string s1 = "", s2 = "";
    int64_t i;
    for (i = str.size()-1; i >= 0; --i) {
        if ((str[i] == '+') || ((str[i] == '-') && (i != 0) && (str[i-1] >= '0') && (str[i-1] <= '9'))) {
            op_exist = true;
            op = str[i];
            break;
        }
    }
    if (op_exist == false) {
        return do_mult(str);
    }
    s1.append(str.begin(), str.begin()+i);
    s2.append(str.begin()+i+1, str.end());
    if (op == '+')
        return do_add(s1)+do_add(s2);
    else
        return do_add(s1)-do_add(s2);
}

int64_t Calculator::calculate() {
    del_spaces();
    if (check() == false) {
        throw invalid_argument("");
    }
    return do_add(s);
}

int main(int argc, char* argv[])
{
    if(argc != 2) {
        cout << "error" << endl;
        return 1;
    }
    Calculator calc = Calculator(argv[1]);
    try {
        int64_t res = calc.calculate();
        cout << res << endl;
    }
    catch(invalid_argument& ex) {
        cout << "error" << endl;
        return 1;
    }
    return 0;
}
