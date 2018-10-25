#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

class Calculator {
    int64_t res;
    std::istringstream din;
    
    void skip_spaces()
    {
        char ch = 0;
        while ((din >> ch) && ch == ' ') {}
        if (!isdigit(ch) && ch != '+' && ch != '-' && ch != '*' && ch != '/' && ch != ' ' && ch != 0) {
            throw std::invalid_argument("invalid_arg");
        } else {
            din.putback(ch);
        }
    }

    char read_op()
    {
        skip_spaces();
        char ch = 0;
        din >> ch;
        return ch;
    }

    char read_unary()
    {
        skip_spaces();
        char sign;
        din >> sign;
        if (sign != '-') {
            din.putback(sign);
            sign = '+';
        }
        return sign;
    }

    int64_t read_number()
    {
        skip_spaces();
        int64_t sign = (read_unary() == '+') ? 1 : -1;
        int64_t num;
        din >> num;
        if (!din) {
            throw std::invalid_argument("invalid_arg");
        }
        return num * sign;
    }

    int64_t parse_mul_div()
    {
        char ch = '*';
        int64_t l_num = read_number();
        while (ch == '*' || ch == '/') {
            ch = read_op();
            if (ch == '*' || ch == '/') {
                int64_t r_num = read_number();
                if (ch == '*') {
                    l_num *= r_num;
                } else {
                    if (!r_num) {
                        throw std::invalid_argument("invalid_arg");
                    }
                    l_num /= r_num;
                }
            } else {
                din.putback(ch);
            }
        }
        return l_num;
    }

    int64_t parse_add_sub()
    {
        char ch = '+';
        int64_t l_num = parse_mul_div();
        while (ch == '+' || ch == '-') {
            ch = read_op();
            if (ch == '+' || ch == '-') {
                int64_t r_num = parse_mul_div();
                if (ch == '+') {
                    l_num += r_num;
                } else {
                    l_num -= r_num;
                }
            }
        }
        skip_spaces();
        if (din) {
            throw std::invalid_argument("invalid_arg");
        }

        return l_num;
    }

public:
    Calculator(const std::string& in) : din(in)
    {
        res = parse_add_sub();
    }

    int64_t get_result() 
    {
        return res;
    }
};

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "error" << std::endl;
        return 1;
    }
   
    try {
        Calculator calc(argv[1]);
        std::cout << calc.get_result() << std::endl;
    } catch (const std::invalid_argument &e) {
        std::cout << "error" << std::endl;
        return 1;
    }
}
