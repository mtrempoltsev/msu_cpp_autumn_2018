#include <iostream>
#include <cstdint>
#include <cstring>

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

    char getc();
    void ungetc();
    void read_lex();
    int64_t expr2();
    int64_t expr1();
    int64_t operand();
public:
    Calc(const char* input): input(input) {}
    int64_t calculate();
};

char Calc::getc()
{
    if(input_pos < strlen(input)) {
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
        char buf[32] = {0};
        buf[0] = c;
        size_t i = 1;
        while(c = getc()) {
            if(!isdigit(c)) {
                break;
            }
            buf[i++] = c;
        }
        if(c) {
            ungetc();
        }
        int64_t num = atoi(buf);
        cur_lex = Lex('n', num);
        return;
    } else if(c == '+' || c == '-'
    || c == '*' || c == '/'
    || c == '\0'
    ) {
        cur_lex = Lex(c);
        return;
    } else {
        throw false;
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
                throw false;
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
            throw false;
        }
    } else {
        if(cur_lex.op == 'n') {
            ret = cur_lex.num;
        } else {
            throw false;
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
    catch(bool) {
        std::cout << "error" << std::endl;
        return 1;
    }
    return 0;
}
