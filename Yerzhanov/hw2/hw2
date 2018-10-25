#include <iostream>
#include <exception>


class LexicalEx: public std::exception {
} lexical;

class LogicalEx: public std::exception {
} logical;


class Calc {
private:
    int pos;
public:
    Calc(): pos(0) {return;}
    int64_t get_ans(int argc, const char* argv);
    int64_t get_expr(int sign, const char* argv);
    int64_t get_mul_div(char sign, int64_t current, const char* argv);
    int64_t get_num(const char* argv);
};


int main(int argc, char* argv[]) {
    Calc acm;
    try {
        std::cout << acm.get_ans(argc, argv[1]) << std::endl;
    } catch (std::exception& e) {
        std::cout << "error\n";
        return 1;
    }
    return 0;
}


int64_t Calc::get_ans(int argc, const char* argv) {
    if (argc != 2) {
        throw lexical;
    }
    return get_expr(1, argv);
}

//BNF:
//{expr} = {* | /} + {expr} or {* | /} - {expr} or {* | /} 
//{* | /} = {num} * {* | /} or {num} / {* | /} or {umin} {* | /} or {num}
//{num} = [0, 1, 2, ...]
//-1 -> lexical -2 -> logical
void skip_spaces(int& pos, const char* argv) {
    while (argv[pos] == ' ' || argv[pos] == '\t') { pos++; }
}

int64_t Calc::get_expr(int sign, const char* argv) {
    int64_t left_operand = get_mul_div('*', 1, argv);
    skip_spaces(pos, argv);
    switch (argv[pos]) {
        case 0: {
            return sign * left_operand;
        } case '+': {
            pos++;
            return sign * left_operand + get_expr(1, argv);
        } case '-': {
            pos++;
            return sign * left_operand + get_expr(-1, argv);
        } default: {
            throw lexical;
        }
    }
}

int64_t Calc::get_mul_div(char sign, int64_t current, const char* argv) {
    skip_spaces(pos, argv);
    if (argv[pos] == '-') {
        pos++;
        return -1 * get_mul_div(sign, current, argv);
    }
    int64_t right_operand = get_num(argv);
    if (sign == '*') {
        current *= right_operand;
    } else {
        if (right_operand) {
            current /= right_operand;
        } else {
            throw logical;
        }
    }
    skip_spaces(pos, argv);
    if (argv[pos] == 0 || argv[pos] == '+' || argv[pos] == '-') {
        return current;
    } else if (argv[pos] == '*') {
        pos++;
        return get_mul_div('*', current, argv);
    } else if (argv[pos] == '/') {
        pos++;
        return get_mul_div('/', current, argv);
    } else {
        throw lexical;
    }
}

int64_t Calc::get_num(const char* argv) {
    skip_spaces(pos, argv);
    int64_t tmp = 0;
    bool is_number = false;
    while (std::isdigit(argv[pos])) {
        is_number = true;
        tmp = tmp * 10 + argv[pos++] - '0';
    }
    if (is_number) {
        return tmp;
    } else {
        throw logical;
    }
}
