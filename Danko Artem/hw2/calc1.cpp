#include <iostream>
#include <malloc.h>
#include <string.h>
//using recursion on grammar

class MyCalc {
    const char* s;
public:
    MyCalc(const char* s) {
        this->s = s;
    }
    int64_t get_result() {
        return parce_add_sub();
    }
private:
    int64_t parce_add_sub() {
        int64_t num, temp_num;
        char sim;
        num = parce_mul_div();
        while (true) {
            const char* ts = s;
            try {
                sim = get_sim();
            } catch (std::out_of_range&) {
                break;
            }
            if (sim != '-' && sim != '+') {
                s = ts;
                break;
            }
            try {
                temp_num = parce_mul_div();
            } catch (std::out_of_range&) {
                throw std::invalid_argument("last number lost");
            };
            if (sim == '+')
                num += temp_num;
            else
                num -= temp_num;
        }
        return num;
    }
    int64_t parce_mul_div() {
        int64_t num1 = get_num(), num2;
        char sim;
        while(true) {
            const char* ts = s;
            try {
                sim = get_sim();
            } catch (std::out_of_range&) {
                break;
            }
            if (sim != '*' && sim != '/') {
                s = ts;
                break;
            }
            num2 = get_num();
            if (sim == '*')
                num1 *= num2;
            else {
                if (num2 == 0)
                    throw std::invalid_argument("zero divide");
                num1 /= num2;
            }
        }
        return num1;
    }
    void skip_spaces() {
        while (s != '\0' && *s == ' '){ s++; }
        if (*s == '\0') throw std::out_of_range("end_of_string");
    }

    int64_t get_num() {
        int um_m = 1;
        int64_t answ = 0;
        //check unary minus
        skip_spaces();
        if (*s == '-') {
            um_m = -1;
            s++;
        } else if (*s > '9' || *s < '0' || *s == '\0')
            throw std::invalid_argument("strange symbol");
        //get number
        skip_spaces();
        while (*s != '\0' && isdigit(*s)) {
            answ = answ * 10 + *s - '0';
            s++;
        }
        return answ * um_m;
    }

    char get_sim(){
        skip_spaces();
        if (*s != '-' && *s != '+' && *s != '*' && *s != '/')
            throw std::invalid_argument("strange symbol");
        return *s++;
    }
};

int main(int argc, char* argv[])
{
    try{
        if (argc != 2)
            throw std::invalid_argument("???");
            MyCalc a = MyCalc(argv[1]);
        std::cout << a.get_result() << std::endl;
    } catch (std::exception&){
        std::cout << "error" << std::endl;
        return 1;
    }
    return 0;
}
