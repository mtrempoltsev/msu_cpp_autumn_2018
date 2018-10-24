#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

template <class T>
class Calculator
{
private:
    T unary_minus(std::istringstream &expr) const
    {
        char minus;
        expr >> minus;
        if (!expr) {
            throw std::invalid_argument("");
        }
        if (minus == '-') {
            return -1;
        }
        expr.putback(minus);
        return 1;
    }

    T read_number(std::istringstream &expr) const
    {
        T sign = unary_minus(expr);
        T number;
        expr >> number;

        if (!expr) {
            throw std::invalid_argument("");
        }
        return sign * number;
    }

    T multiplication_and_division(std::istringstream &expr) const
    {
        T arg1 = read_number(expr);

        char op;
        while (expr >> op && (op == '*' || op == '/')) {
            T arg2 = read_number(expr);

            if (op == '*') {
                arg1 *= arg2;
            } else {
                if (arg2 == 0) {
                    throw std::invalid_argument("");
                }
                arg1 /= arg2;
            }
        }

        if (expr) {
            expr.putback(op);
        }

        return arg1;
    }

    T summation_and_subtraction(std::istringstream &expr) const
    {
        T arg1 = multiplication_and_division(expr);

        char op;
        while (expr >> op && (op == '+' || op == '-')) {
            T arg2 = multiplication_and_division(expr);

            if (op == '+') {
                arg1 += arg2;
            } else {
                arg1 -= arg2;
            }
        }
        if (expr) {
            throw std::invalid_argument("");
        }

        return arg1;
    }

public:
    Calculator()
    {
    }

    T get_result(const std::string &expr) const
    {
        std::istringstream iss(expr);
        return summation_and_subtraction(iss);
    }
};

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "error" << std::endl;
        return 1;
    }

    try {
        std::cout << Calculator<int64_t>().get_result(argv[1]) << std::endl;
    } catch(const std::invalid_argument &) {
        std::cout << "error" << std::endl;
        return 1;
    }
}
