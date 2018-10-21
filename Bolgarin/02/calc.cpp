#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>

class Calculator {
    using Int = int64_t;

public:
    static Int Parse(const char* str) {
        std::istringstream expression(str);

        return ParsePlusMinus(expression);
    }

private:
    struct Operations {
        static constexpr auto kAdd = '+';
        static constexpr auto kSubstraction = '-';
        static constexpr auto kMultiplication = '*';
        static constexpr auto kDivision = '/';
        static constexpr auto kMinus = '-';
    };

    static Int ParsePlusMinus(std::istringstream& expression) {
        Int left = ParseMulDiv(expression);
        char operation;

        while (expression >> operation &&
                (operation == Operations::kAdd ||
                operation == Operations::kSubstraction)) {

            Int right = ParseMulDiv(expression);
            if (operation == Operations::kAdd)
                left += right;
            else
                left -= right;
        }

        if (expression)
            throw std::invalid_argument("Wrong expression!");

        return left;
    }

    static Int ParseMulDiv(std::istringstream& expression) {
        Int left = ParseNumber(expression);
        char operation;

        while (expression >> operation &&
                (operation == Operations::kMultiplication ||
                operation == Operations::kDivision)) {

            Int right = ParseNumber(expression);
            if (operation == Operations::kMultiplication) {
                left *= right;
            } else {
                if (right == 0)
                    throw std::invalid_argument("Division by zero");

                left /= right;
            }
        }

        if (expression)
            expression.putback(operation);

        return left;
    }

    static Int ParseNumber(std::istringstream& expression) {
        Int sign = ParseNumberSign(expression);
        Int number;

        if (!(expression >> number))
            throw std::invalid_argument("Invalid input");

        return sign * number;
    }

    static Int ParseNumberSign(std::istringstream& expression) {
        char sign;

        if (!(expression >> sign))
            throw std::invalid_argument("Invalid input");

        if (sign == Operations::kMinus)
            return -1;

        expression.putback(sign);
        return 1;
    }
};


int main(int argc, char* argv[]) {
    std::string error_message = "error";
    
    if (argc == 2) {
        try {
            std::cout << Calculator::Parse(argv[1]);
        } catch (std::invalid_argument&) {
            std::cerr << error_message;
            return 1;
        }

    } else {
        std::cerr << error_message;
        return 1;
    }

    return 0;
}
