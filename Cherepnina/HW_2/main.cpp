#include <iostream>
#include <string>
#include <cstring>

#define INCORRECT_INPUT 1

using namespace std;

int64_t add(int64_t);

int64_t get_number(int *);

int pointer;
char *argv_gl;

int main(int argc, char *argv[]) {
    try {
        if (argc != 2)
            throw INCORRECT_INPUT;
        argv_gl = argv[1];
        pointer = 0;
        std::cout << add(get_number(&pointer)) << std::endl;
    }
    catch (int e) {
        cout << "error" << endl;
        return INCORRECT_INPUT;
    };
    return 0;
}

int64_t get_number(int *const pointer) {
    char digit = 0;
    char sign = 0;
    char symbol;
    int64_t number = 0;
    for (; (symbol = argv_gl[*pointer]) != '\0'; (*pointer)++)
        switch (symbol) {
            case '-':
            case '+':
                if (digit) return sign == '-' ? -number : number;
                else if (sign == symbol)
                    sign = '+';
                else sign = '-';
                break;
            case '*':
            case '/':
                if (digit) return sign == '-' ? -number : number;
                else throw INCORRECT_INPUT;
            case ' ':
                continue;
            case '0'...'9':
                number = number * 10 + (symbol - '0');
                digit = 1;
                break;
            default:
                throw INCORRECT_INPUT;
        }
    return sign == '-' ? -number : number;
}

char get_operation(int *const pointer) {
    if (*pointer >= strlen(argv_gl))
        return '\0';
    char op;
    while ((op = argv_gl[(*pointer)++]) != '\0')
        switch (op) {
            case '+':
            case '-':
            case '*':
            case '/':
                return op;
            case ' ':
                continue;
            default:
                return -1;
        }
    return '0';
}

int64_t add(int64_t argument) {
    char operation = get_operation(&pointer);
    switch (operation) {
        case '+':
            return argument + add(get_number(&pointer));
        case '-':
            return argument + add(-get_number(&pointer));
        case '*':
            return add(argument * get_number(&pointer));
        case '/': {
            int64_t divider = get_number(&pointer);
            if (divider == 0) throw 136;
            return add(argument / divider);
        }
        case '\0':
            return argument;
        default:
            throw INCORRECT_INPUT;
    }
}