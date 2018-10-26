#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <stdexcept>

enum Lexem_type {
    PLUS,
    MINUS,
    DIVIDE,
    MULTIPLY,
    NUM,
    FIN
};
enum {
    MAGIC_NUMBER = 7 // Means absolutely nothing
};
typedef std::pair<int, Lexem_type> lex_vector_elem;

class Analyzer {
    std::string expr;
    size_t expr_len;
    size_t index = 0;
    Lexem_type lex_type;
    int lex_value;

    std::vector<lex_vector_elem> lex_vector;
    std::stack<int> value_stack;

    Lexem_type getlex() {
        if (index < expr_len) {
            char c = expr[index++];
            switch (c) 
            {
                case ' ':
                    getlex();
                    break;
                case '+':
                    lex_type = PLUS;
                    break;
                case '-':
                    lex_type = MINUS;
                    break;
                case '/':
                    lex_type = DIVIDE;
                    break;
                case '*':
                    lex_type = MULTIPLY;
                    break;
                case '0': case '1':
                case '2': case '3':
                case '4': case '5':
                case '6': case '7':
                case '8': case '9': 
                    lex_value = c - '0';
                    lex_type = NUM;
                    break;
                default:
                    throw std::invalid_argument("Wrong expression (in getlex())");
                    break;
            }
        } else {
            lex_type = FIN;
        }
        return lex_type;
    }

 // Start_condition -> Sum_condition Finale_condition
 // Sum_condition -> Mult_condition {+|- Sum_condition}
 // Mult_condition -> Number {*|/ Mult_condition}
 // Number -> - '0'..'9' | '0'..'9'

    void Finale_condition() {
        if (lex_type != FIN) {
            throw std::invalid_argument("Unexpected symbol (end expected)");
        }    
    }

    void Number() {
        int sign = 1;
        if (lex_type == MINUS){
            sign = -1;
            getlex();
        } 
        if (lex_type == NUM) {
            lex_vector.push_back(lex_vector_elem(sign * lex_value, NUM));
            getlex();
        } else {
            throw std::invalid_argument("Wrong something in C()");
        }
    }

    void Mult_condition() {
        Number();
        while (lex_type == DIVIDE || lex_type == MULTIPLY) {
            Lexem_type cur_lex = lex_type;
            getlex();
            Number();
            lex_vector.push_back(lex_vector_elem(MAGIC_NUMBER, cur_lex));
        } 
    }

    void Sum_condition() {
        Mult_condition();
        while (lex_type == PLUS || lex_type == MINUS) {
            Lexem_type cur_lex = lex_type;
            getlex();
            Mult_condition();
            lex_vector.push_back(lex_vector_elem(MAGIC_NUMBER, cur_lex));
        } 
    }

    void Start_condition() {
        Sum_condition();
        Finale_condition();
    }

public:
    Analyzer(const char* init) : expr(std::string(init)), 
                                 expr_len(expr.size()) {
        getlex();
    }

    void start() {
        Start_condition();
    }

    int result() {
        int op1 = 0, op2 = 0;
        for (size_t i = 0; i < lex_vector.size(); i++) {
            switch (lex_vector[i].second) {
                case NUM:
                    value_stack.push(lex_vector[i].first);
                    break;
                case PLUS:
                    op2 = value_stack.top();
                    value_stack.pop();
                    op1 = value_stack.top();
                    value_stack.pop();
                    value_stack.push(op1 + op2);
                    break;
                case MINUS:
                    op2 = value_stack.top();
                    value_stack.pop();
                    op1 = value_stack.top();
                    value_stack.pop();
                    value_stack.push(op1 - op2);
                    break;
                case DIVIDE:
                    op2 = value_stack.top();
                    value_stack.pop();
                    if (op2 == 0) {
                        throw std::invalid_argument("Trying to divide by 0");
                    }
                    op1 = value_stack.top();
                    value_stack.pop();
                    value_stack.push(op1 / op2);
                    break;
                case MULTIPLY:
                    op2 = value_stack.top();
                    value_stack.pop();
                    op1 = value_stack.top();
                    value_stack.pop();
                    value_stack.push(op1 * op2);
                    break;
                default:
                    throw std::invalid_argument("Wrong Lexem_type in result()");
                    break;
            }
        }
        return value_stack.top();
    }
    
};

int main(int argc, char* argv[])
{
    try{
        if (argc != 2) {
            throw std::invalid_argument("Wrong amount of arguments");
        }
        Analyzer analyze(argv[1]);
        analyze.start();
        std::cout << analyze.result() << std::endl;
        return 0;
    } catch (std::invalid_argument&) {
        std::cout << "error" << std::endl;
        return 1;
    }
}