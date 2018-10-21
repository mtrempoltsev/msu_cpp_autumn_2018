#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>

class Calculator {
    using Int = int64_t;

public:
    Calculator(const char* str) : expression_(str) { }

    Int Parse() {
        return ParsePlusMinus();
    }

private:
    std::istringstream expression_;
    struct Operations {
        static constexpr auto kAdd = '+';
        static constexpr auto kSubstraction = '-';
        static constexpr auto kMultiplication = '*';
        static constexpr auto kDivision = '/';
        static constexpr auto kMinus = '-';
    };

    Int ParsePlusMinus() {
        Int left = ParseMulDiv();
        char operation;

        while (expression_ >> operation &&
                (operation == Operations::kAdd ||
                operation == Operations::kSubstraction)) {

            Int right = ParseMulDiv();
            if (operation == Operations::kAdd)
                left += right;
            else
                left -= right;
        }

        if (expression_)
            throw std::invalid_argument("Wrong expression_!");

        return left;
    }

    Int ParseMulDiv() {
        Int left = ParseNumber();
        char operation;

        while (expression_ >> operation &&
                (operation == Operations::kMultiplication ||
                operation == Operations::kDivision)) {

            Int right = ParseNumber();
            if (operation == Operations::kMultiplication) {
                left *= right;
            } else {
                if (right == 0)
                    throw std::invalid_argument("Division by zero");

                left /= right;
            }
        }

        if (expression_)
            expression_.putback(operation);

        return left;
    }

    Int ParseNumber() {
        Int sign = ParseNumberSign();
        Int number;

        if (!(expression_ >> number))
            throw std::invalid_argument("Invalid input");

        return sign * number;
    }

    Int ParseNumberSign() {
        char sign;

        if (!(expression_ >> sign))
            throw std::invalid_argument("Invalid input");

        if (sign == Operations::kMinus)
            return -1;

        expression_.putback(sign);
        return 1;
    }
};


int main(int argc, char* argv[]) {
    const std::string error_message = "error";

    if (argc == 2) {
        Calculator calc(argv[1]);

        try {
            std::cout << calc.Parse();
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