#include <iostream>
#include <string>
#include <sstream>
#include <ctype.h>


class Calculator{
private:
    int64_t res;
    int64_t num(std::istringstream& in){
        char minus;
        if(!(in >> minus)) throw "error";
        int64_t buf;
        if(minus == '-'){
            if(!(in >> buf)) throw "error";
            return -buf;
        }else{
            if(!isdigit(minus)) throw "error";
            in.putback(minus);
            in >> buf;
            return buf;
        }
    }
    int64_t mul(std::istringstream& in){
        int64_t num1 = num(in);
        char buf;
        while((in >> buf) && (buf == '*' || buf == '/')){
            int64_t num2 = num(in);
            if(buf == '*') num1 *= num2;
            if(buf == '/'){
                if(num2 == 0) throw "error";
                num1 /= num2;
            }
        }
        in.putback(buf);
        return num1;
    }
    int64_t sum(std::istringstream& in){
        int64_t num1 = mul(in);
        char buf;
        while(in >> buf && (buf == '+' || buf == '-')){
            int64_t num2 = mul(in);
            if(buf == '+') num1 += num2;
            if(buf == '-') num1 -= num2;
        }
        return num1;
    }
public:
    Calculator(std::string const& str){
        std::istringstream in(str);
        res = sum(in);
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
    }catch(const char* str){
        std::cout << str;
        return 1;
    }
    return 0;
}
