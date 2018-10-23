#include <iostream>
#include <string>
#include <sstream>
#include <ctype.h>
#include <stdexcept>


class Calculator{
private:
    int64_t res;
    bool num(std::istringstream& in, int64_t& arg){
        char minus;
        if(!(in >> minus)) return false;
        int64_t buf;
        if(minus == '-'){
            if(!(in >> buf)) return false;
            arg = -buf;
            return true;
        }else{
            if(!isdigit(minus)) return false;
            in.putback(minus);
            in >> buf;
            arg = buf;
            return true;
        }
    }
    bool mul(std::istringstream& in, int64_t& arg){
        int64_t num1;
        if(!num(in, num1)) return false;
        char buf;
        while((in >> buf) && (buf == '*' || buf == '/')){
            int64_t num2;
            if(!num(in, num2)) return false;
            if(buf == '*') num1 *= num2;
            if(buf == '/'){
                if(num2 == 0) return false;
                num1 /= num2;
            }
        }
        in.putback(buf);
        arg = num1;
        return true;
    }
    bool sum(std::istringstream& in, int64_t& arg){
        int64_t num1;
        if(!mul(in, num1)) return false;
        char buf;
        while(in >> buf && (buf == '+' || buf == '-')){
            int64_t num2;
            if(!mul(in, num2)) return false;
            if(buf == '+') num1 += num2;
            if(buf == '-') num1 -= num2;
        }
        arg = num1;
        return true;
    }
public:
    Calculator(std::string const& str) : res(0){
        std::istringstream in(str);
        if(!sum(in, res)) throw std::invalid_argument("error");
    }
    friend std::ostream & operator<<(std::ostream& out, const Calculator& calc);
};

std::ostream& operator<<(std::ostream& out, const Calculator& calc){
    return out << calc.res;
}

int main(int argc, char **argv){
    if(argc != 2){
        std::cout << "error";
        return 1;
    }
    try{
        Calculator calc(argv[1]);
        std::cout << calc;
    }catch(std::exception& e){
        std::cout << "error";
        return 1;
    }
    return 0;
}
