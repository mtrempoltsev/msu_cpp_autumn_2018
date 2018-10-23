#include <iostream>
#include <string.h>
#include <cctype>
#include <cstdint>
#include <cstdlib>
/*
expr := addend [+-] ... [+-] addend

addend := factor [/*] ... [/*] factor

factor := number | -factor
*/

class Calculator {
private:
    char *s;
    char *start;
    int64_t expr();
    int64_t addend();
    int64_t factor();
    void err();
public:
    Calculator(const char *init);
    void calculate();
    ~Calculator();
};

Calculator::Calculator(const char *init)
{
    size_t len = strlen(init);
    char *str = (char*)malloc(sizeof(char)*(len+1));
    unsigned j = 0;


    for (unsigned i = 0; i < len; ++i) {
        if (!isspace(init[i])) {
            str[j++] = init[i];
        }
    }
    str[j] = 0;
    s = str;
    start = str;
}

Calculator::~Calculator()
{
    free(start);
}

int64_t Calculator::expr()
{
    int64_t ans = addend();
    while (*s == '+' || *s == '-') {
        if (*s == '+') {
            s += 1;
            ans += addend();
        }
        else if (*s == '-') {
            s += 1;
            ans -= addend();
        }
    }
    if (*s) err();
    return ans;
}

int64_t Calculator::addend()
{
    int64_t ans = factor();
    while (*s == '*' || *s == '/') {
        if (*s == '*')  {
            s += 1;
            ans *= factor();
        }
        else if (*s == '/') {
            s += 1;
            int64_t f = factor();
            if (!f) err();
            ans /= f;
        }
    }
    return ans;
}

int64_t Calculator::factor()
{
    if (*s != '-' && !isdigit(*s)) err();
    if (*s == '-') { s += 1; return -factor(); }
    int64_t ans = 0;
    while (isdigit(*s)) {
        ans = ans * 10 + *s - '0';
        s += 1;
    }
    return ans;
}

void Calculator::calculate() {
    std::cout << expr() << std::endl;
}

void Calculator::err() {
    throw std::invalid_argument("error");
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cout << "error" << std::endl;
        return 1;
    }

    Calculator calculator(argv[1]);
    try {
        calculator.calculate();
    } catch (std::exception &e) {
        std::cout << "error" << std::endl;
        return 1;
    }

    return 0;
}
