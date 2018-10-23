#include <iostream>
#include <cstdint>
#include <cstring>
#include <stdexcept>

class Calc
{
    struct Lex {
        int64_t num;
        char op;
        Lex(char op = '\0', int64_t num = 0): num(num), op(op) {}
    };
    Lex cur_lex = Lex();
    const char* input;
    size_t input_pos = 0;
    size_t input_len;
    
    char getc();
    void ungetc();
    void read_lex();
    int64_t expr2();
    int64_t expr1();
    int64_t operand();
public:
    Calc(const char* input): input(input), input_len(strlen(input)) {}
    int64_t calculate();
};

char Calc::getc()
{
    if(input_pos < input_len) {
        return input[input_pos++];
    } else {
        return '\0';
    }
}

void Calc::ungetc()
{
    input_pos--;
}

void Calc::read_lex()
{
    char c;
    do {
        c = getc();
    } while(c == ' ');
    if(isdigit(c)) {
        int64_t num = c - '0';
        while(c = getc()) {
            if(!isdigit(c)) {
                break;
            }
            num = 10 * num + (c - '0');
        }
        if(c) {
            ungetc();
        }
        cur_lex = Lex('n', num);
        return;
    } else if(c == '+' || c == '-' || c == '*' || c == '/' || c == '\0') {
        cur_lex = Lex(c);
        return;
    } else {
        throw std::runtime_error("unexpected symbol");
    }
}

int64_t Calc::expr2()
{
    // expr2 := expr1 {[+-] expr1}
    int64_t res = expr1();
    int64_t opnd;
    while(true) {
        if(cur_lex.op == '+') {
            read_lex();
            opnd = expr1();
            res += opnd;
        } else if(cur_lex.op == '-') {
            read_lex();
            opnd = expr1();
            res -= opnd;
        } else {
            break;
        }
    }
    return res;
}

int64_t Calc::expr1()
{
    // expr1 := oper {[*/] oper}
    int64_t res = operand();
    int64_t opnd;
    while(true) {
        if(cur_lex.op == '*') {
            read_lex();
            opnd = operand();
            res *= opnd;
        } else if(cur_lex.op == '/') {
            read_lex();
            opnd = operand();
            if(opnd == 0) {
                throw std::runtime_error("division by zero");
            }
            res /= opnd;
        } else {
            break;
        }
    }
    return res;
}

int64_t Calc::operand()
{
    int64_t ret;
    if(cur_lex.op == '-') {
        read_lex();
        if(cur_lex.op == 'n') {
            ret = -cur_lex.num;
        } else {
            throw std::runtime_error("syntax error");
        }
    } else {
        if(cur_lex.op == 'n') {
            ret = cur_lex.num;
        } else {
            throw std::runtime_error("syntax error");
        }
    }
    read_lex();
    return ret;
}

int64_t Calc::calculate()
{
    read_lex();
    return expr2();
}

int main(int argc, char* argv[])
{
    if(argc != 2) {
        std::cout << "error" << std::endl;
        return 1;
    }
    Calc calc = Calc(argv[1]);
    try {
        int64_t res = calc.calculate();
        std::cout << res << std::endl;
    }
    catch(std::exception&) {
        std::cout << "error" << std::endl;
        return 1;
    }
    return 0;
}
