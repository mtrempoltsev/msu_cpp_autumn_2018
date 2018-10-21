#include <iostream>
#include <algorithm>
#include <cstring>

enum LEX_TYPE {NUM, OP, EOL};
enum OP_TYPE {ADD, SUB, MUL, DIV};


struct Lex {
    LEX_TYPE t;   // t for type
    int64_t d;    // d for data
};

void error() {
    std::cout << "error" << std::endl;
    std::exit(1);
}


// S -> A
// A -> M A | M A | M             - plus and minus are handled in N state
// M -> N * M | N / M | N
// N -> -N | +N | [numeric constant]
class Expr {
private:
    char *s;
    Lex lex;

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
                        error();
                    }
                    cur /= divisor;
                }
            }
        }
        return cur;
    }

    int64_t parse_state_N() {
        if (lex.t == EOL) {
            error();
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
        error();
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
                    error();
            }
            ++s;
        }
    }

public:
    Expr(char *_s) {
        s = _s;
    }

    int64_t eval() {
        parse_lex();
        return parse_state_A();
    }
};


int main(int argc, char* argv[])
{
    if (argc != 2) {
        error();
    }
    
    Expr expr(argv[1]);

    std::cout << expr.eval() << std::endl;

    return 0;
}