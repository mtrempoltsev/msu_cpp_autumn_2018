#include <iostream>
#include <string>
#include <cstring>

#define INCORRECT_INPUT invalid_argument("")

using namespace std;

class Calc {
    int pointer;
    const char *argv;

    int64_t get_number(int *const pointer) {
        char digit = 0;
        char sign = 0;
        char symbol;
        int64_t number = 0;
        for (; (symbol = argv[*pointer]) != '\0'; (*pointer)++)
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
        if (*pointer >= strlen(argv))
            return '\0';
        char op;
        while ((op = argv[(*pointer)++]) != '\0')
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
                if (divider == 0) throw INCORRECT_INPUT;
                return add(argument / divider);
            }
            case '\0':
                return argument;
            default:
                throw INCORRECT_INPUT;
        }
    }

public:
    Calc(const char *arg_string):argv{arg_string}{}

    int64_t get_result() {
        pointer = 0;
        return add(get_number(&pointer));
    }
};


int main(int argc, char *argv[]) {
    Calc *cl = new Calc(argv[1]);
    char return_result = 0;
    try {
        if (argc != 2)
            throw INCORRECT_INPUT;
        cout << cl->get_result() << endl;
    }
    catch (invalid_argument&) {
        cerr << "error" << endl;
        return_result = 1;
    };
    delete cl;
    return return_result;
}
