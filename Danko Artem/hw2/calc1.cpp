#include <iostream>
#include <string>

//using recursion on grammar

class MyCalc {

public:
    static int64_t get_result(char* s) {
        return parce_add_sub(s);
    }

private:
    static int64_t parce_add_sub(char* &s) {
        int64_t num, temp_num;
        char sim;
        try {
            num = parce_mul_div(s);
        } catch (std::out_of_range&) {
            throw std::invalid_argument("empty or not correct input");
        }
        while (true) {
            char* ts = s;
            try {
                sim = get_sim(s);
            } catch (std::out_of_range&) {
                break;
            }
            if (sim != '-' && sim != '+') {
                s = ts;
                break;
            }
            try {
                temp_num = parce_mul_div(s);
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
    static int64_t parce_mul_div(char* &s) {
        int64_t num1 = get_num(s), num2;
        char sim;
        while(true) {
            char* ts = s;
            try {
                sim = get_sim(s);
            } catch (std::out_of_range&) {
                break;
            }
            if (sim != '*' && sim != '/') {
                s = ts;
                break;
            }
            num2 = get_num(s);
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
    static void skip_spaces(char* &s) {
        while (s != '\0' && *s == ' '){ s++; }
        if (*s == '\0') throw std::out_of_range("end_of_string");
    }

    static int64_t get_num(char* &s) {
        int um_m = 1;
        int64_t answ = 0;
        //check unary minus
        skip_spaces(s);
        if (*s == '-') {
            um_m = -1;
            s++;
        } else if (*s > '9' || *s < '0' || *s == '\0')
            throw std::invalid_argument("strange symbol");
        //get number
        skip_spaces(s);
        while (*s != '\0' && isdigit(*s)) {
            answ = answ * 10 + *s - '0';
            s++;
        }
        return answ * um_m;
    }

    static char get_sim(char* &s){
        skip_spaces(s);
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
        std::cout << MyCalc::get_result(argv[1]) << std::endl;
    } catch (std::exception&){
        std::cout << "error" << std::endl;
        return 1;
    }
    return 0;
}
