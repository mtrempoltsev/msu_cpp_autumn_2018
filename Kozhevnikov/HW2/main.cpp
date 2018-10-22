#include <iostream>

int64_t count(int &, const char *);
int64_t divide(int &, int64_t, const char *);
int64_t mul(int &, int64_t, const char *);

void skip_spaces(const char *ar, int &i) {
    while (ar[i] == ' ') {
        i++;
    }
}

void exit_with_error() {  // to shorten emergency stop code
    std::cout << "error" << std::endl;
    exit(1);
}

int count_sign(const char *ar, int &i) {  // process sequence of '-' and ' '
    int sign = 1;
    while (ar[i] == '-') {
        sign *= -1;
        i++;
        skip_spaces(ar, i);
    }
    return sign;
}

int64_t read_number(const char *ar, int &i) {  // read number itself without previous '-'
    std::string number;
    skip_spaces(ar, i);
    int sign  = count_sign(ar, i);
    skip_spaces(ar, i);
    while ((ar[i] > '0') && (ar[i] < '9')) {
        number += ar[i];
        i++;
    }
    if (!number.compare("")) {
        exit_with_error();  // empty number isn't good
    }
    skip_spaces(ar, i);
    return std::stoi(number) * sign;
}

int64_t mul (int &i, int64_t prev, const char *ar) {  // part of recursion chain for multiplication
    int64_t number = read_number(ar, i);
    if (ar[i] == '\0') {  // last number of expression
        return prev * number;
    }
    switch(ar[i]) {
        case '+': {
            return prev * number + count(++i,  ar);
        }
        case '-': {
            return prev * number + count(i, ar);  // crutch (- we encode as + (-...))
        }
        case '*': {
            return mul(++i, prev * number, ar);
        }
        case '/': {
            return divide(++i, prev * number, ar);
        }
        default: {  // invalid operation;
            exit_with_error();
        }
    }
}


int64_t divide(int &i, int64_t prev, const char *ar) {  // part of recursion chain for division
    int64_t number = read_number(ar, i);
    if (number == 0) {  // division by zero
        exit_with_error();
    }
    if (ar[i] == '\0') {  // last number of expression
        return prev / number;
    }
    switch(ar[i]) {
        case '+': {
            return prev / number + count(++i, ar);
        }
        case '-': {
            return prev / number + count(i, ar);  // crutch ('-' we encode as '+' (-...))
        }
        case '*': {

            return mul(++i, prev / number, ar);
        }
        case '/': {
            return divide(++i, prev / number, ar);
        }
        default: {  // invalid operation;
            exit_with_error();
        }
    }
}


int64_t count(int &i, const char *ar) {  // "main" element of recursion
    if (ar[i] == '\0') {  //final step of recursion
        return 0;
    }
    int64_t number = read_number(ar, i);
    if (ar[i] == '\0') {  // last number of expression
        return number;
    }
    switch (ar[i]) {
        case '+': {
            return number + count(++i, ar);
        }
        case '-': {
            return number + count(i, ar);  // crutch (- we encode as + (-...))
        }
        case '*': {
            return mul(++i, number, ar) + count(i, ar);
        }
        case '/': {
            return divide(++i, number, ar) + count(i, ar);
        }
        default:  // invalid operation
            exit_with_error();
    }
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {  // invalid command line parameters
        exit_with_error();
    }
    int i = 0;
    int64_t  result = count(i, argv[1]);
    std::cout << result;
}