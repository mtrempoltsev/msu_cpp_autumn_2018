#include <iostream>

class invalidExpression {};  // exception class

class Calculator {
private:
    const char *ar;
    int i;

    int64_t count();
    int64_t divide(int64_t);
    int64_t mul(int64_t);
    int64_t read_number();

    int count_sign();
    void exit_with_error();
    void skip_spaces();
public:
    Calculator(const char *exp)
        : ar(exp), i(0)
    {}

    int64_t calculate() {
            return count();
    };
};

int main(int argc, const char *argv[]) {
    if (argc != 2) {  // invalid command line parameters
        std::cout << "error" << std::endl;
        return 1;
    }

    int64_t result = 0;

    Calculator *calc = new Calculator(argv[1]);
    try {
        result = calc->calculate();
    } catch (invalidExpression &e) {
        return 1;
    }

//    int64_t  result = count(i, argv[1]);
    std::cout << result;
}

void Calculator::skip_spaces() {
    while (ar[i] == ' ') {
        i++;
    }
}

void Calculator::exit_with_error() {  // to shorten emergency stop code
    std::cout << "error" << std::endl;
    throw invalidExpression();
}

int Calculator::count_sign() {  // process sequence of '-' and ' '
    int sign = 1;
    while (ar[i] == '-') {
        sign *= -1;
        i++;
        skip_spaces();
    }
    return sign;
}

int64_t Calculator::read_number() {  // read number itself without previous '-'
    std::string number;
    skip_spaces();
    int sign  = count_sign();
    skip_spaces();
    while ((ar[i] > '0') && (ar[i] < '9')) {
        number += ar[i];
        i++;
    }
    if (!number.compare("")) {
        exit_with_error();  // empty number isn't good
    }
    skip_spaces();
    return std::stoi(number) * sign;
}

int64_t Calculator::mul (int64_t prev) {  // part of recursion chain for multiplication
    int64_t number = read_number();
    if (ar[i] == '\0') {  // last number of expression
        return prev * number;
    }
    switch(ar[i]) {
        case '+': {
            i++;
            return prev * number + count();
        }
        case '-': {
            return prev * number + count();  // crutch (- we encode as + (-...))
        }
        case '*': {
            i++;
            return mul(prev * number);
        }
        case '/': {
            i++;
            return divide(prev * number);
        }
        default: {  // invalid operation;
            exit_with_error();
        }
    }
    return 0;
}


int64_t Calculator::divide(int64_t prev) {  // part of recursion chain for division
    int64_t number = read_number();
    if (number == 0) {  // division by zero
        exit_with_error();
    }
    if (ar[i] == '\0') {  // last number of expression
        return prev / number;
    }
    switch(ar[i]) {
        case '+': {
            i++;
            return prev / number + count();
        }
        case '-': {
            return prev / number + count();  // crutch ('-' we encode as '+' (-...))
        }
        case '*': {
            i++;
            return mul(prev / number);
        }
        case '/': {
            i++;
            return divide(prev / number);
        }
        default: {  // invalid operation;
            exit_with_error();
        }
    }
    return 0;
}


int64_t Calculator::count() {  // "main" element of recursion
    if (ar[i] == '\0') {  //final step of recursion
        return 0;
    }
    int64_t number = read_number();
    if (ar[i] == '\0') {  // last number of expression
        return number;
    }
    switch (ar[i]) {
        case '+': {
            i++;
            return number + count();
        }
        case '-': {
            return number + count();  // crutch (- we encode as + (-...))
        }
        case '*': {
            i++;
            return mul(number) + count();
        }
        case '/': {
            i++;
            return divide(number) + count();
        }
        default:  // invalid operation
            exit_with_error();
    }
    return 0;
}