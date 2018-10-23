#include <iostream>
#include <algorithm>
#include <cstring>
#include <exception>

enum LEX_TYPE {NUM, OP, EOL};   // EOF for End Of Line
enum OP_TYPE {ADD, SUB, MUL, DIV};


struct Lex {
    LEX_TYPE t;   // t for type
    int64_t d;    // d for data
};

class Error : public std::exception {};   // empty class for exceptions


// S -> A
// A -> M A | M A | M             - plus and minus are handled in N state
// M -> N * M | N / M | N
// N -> -N | +N | [numeric constant]
class Expr {
private:
    const char *s;
    Lex lex;
    bool correct;
    int64_t value;

    int64_t parse_state_A() {
        int64_t cur = parse_state_M();
        if (lex.t != EOL) {
            if (lex.t == OP) {
                if (lex.d == ADD) {
                    cur += parse_state_A();
                } else if (lex.d == SUB) {
                    cur += parse_state_A();
                }
            }
        }
        return cur;
    }

    int64_t parse_state_M() {
        int64_t cur = parse_state_N();
        if (lex.t != EOL) {
            if (lex.t == OP) {
                if (lex.d == MUL) {
                    parse_lex();
                    cur *= parse_state_M();
                } else if (lex.d == DIV) {
                    parse_lex();
                    int64_t divisor = parse_state_M();
                    if (divisor == 0) {
                        throw Error();
                    }
                    cur /= divisor;
                }
            }
        }
        return cur;
    }

    int64_t parse_state_N() {
        if (lex.t == EOL) {
            throw Error();
        }
        if (lex.t == NUM) {
            int64_t cur = lex.d;
            parse_lex();
            return cur;
        } else if (lex.t == OP) {
            if (lex.d == ADD) {
                parse_lex();
                return parse_state_N();
            } else if (lex.d == SUB) {
                parse_lex();
                return -parse_state_N();
            }
        }
        throw Error();
    }

    void parse_lex() {
        while (std::isspace(*s)) ++s;

        if (*s == '\0') {
            lex.t = EOL;
        } else if (std::isdigit(*s)) {
            lex.t = NUM;
            lex.d = 0;
            while (std::isdigit(*s)) {
                lex.d = lex.d * 10 + (*s - '0');
                ++s;
            }
        } else {
            lex.t = OP;
            switch (*s) {
                case '+':
                    lex.d = ADD;
                    break;
                case '-':
                    lex.d = SUB;
                    break;
                case '*':
                    lex.d = MUL;
                    break;
                case '/':
                    lex.d = DIV;
                    break;
                default:
                    throw Error();
            }
            ++s;
        }
    }

public:
    Expr(const char *_s) : s(_s) {
        try {
            parse_lex();
            value = parse_state_A();
            correct = true;
        } catch (Error&) {
            correct = false;
        }
    }

    bool is_correct() {
        return correct;
    }

    int64_t get_value() {
        return value;
    }
};


int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "error" << std::endl;
        return 1;
    }
    
    Expr expr(argv[1]);

    if (expr.is_correct()) {
        std::cout << expr.get_value() << std::endl;
    } else {
        std::cout << "error" << std::endl;
        return 1;
    }

    return 0;
}