#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

class Calculator
{
private:

    int64_t summ_minus(std::istringstream &iss) const
    {
        int64_t arg1 = mult_div(iss);
        char oper;
        while (iss >> oper && (oper == '+' || oper == '-')) {
            int64_t arg2 = mult_div(iss);
            if (oper == '+') {
                arg1 += arg2;
            } else {
                arg1 -= arg2;
            }
        }
        if (iss) throw std::invalid_argument("");
        return arg1;
    }

    int64_t mult_div(std::istringstream &iss) const
    {
        int64_t arg1 = number(iss);
        char oper;
        while (iss >> oper && (oper == '*' || oper == '/')) {
            int64_t arg2 = number(iss);

            if (oper == '*') {
                arg1 *= arg2;
            } else {
                if (arg2 == 0) {
                    throw std::invalid_argument("");
                }
                arg1 /= arg2;
            }
        }
        if (iss) iss.putback(oper);
        return arg1;
    }

    int64_t number(std::istringstream &iss) const
    {
        int64_t sign = un_minus(iss);

        int64_t num;
        iss >> num;

        if (!iss) throw std::invalid_argument("");

        return sign * num;
    }

    int64_t un_minus(std::istringstream &iss) const
    {
        char sign;
        iss >> sign;
        if (!iss) {
            throw std::invalid_argument("");
        }
        if (sign == '-') {
            return -1;
        }
        iss.putback(sign);
        return 1;
    }


public:
    Calculator()
    {
    }

    int64_t calculate(const std::string &str) const
    {
        std::istringstream iss(str);
        return summ_minus(iss);
    }
};

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "error" << std::endl;
        return 1;
    }

    try {
        std::cout << Calculator().calculate(argv[1]) << std::endl;
    } catch(const std::invalid_argument &) {
        std::cout << "error" << std::endl;
        return 1;
    }
}
