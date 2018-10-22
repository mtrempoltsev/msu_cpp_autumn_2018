#include <iostream>
#include <string>
#include <stdexcept>
#include <stack>
#include <vector>

class Calculator {
    enum lex_type{PLUS, MINUS, STAR, SLASH, NUMBER, END};
    
    // POLIZ vector of the program
    std::vector< std::pair<lex_type, int64_t> > poliz;
    // Stack for counting
    std::stack<int64_t> stack;
    // Original string
    std::string expr;

    size_t expr_size;
    lex_type cur_lex;
    size_t cur_index;
    int64_t last_num_lex; // value of last NUMBER lex

    // Finds Lexes one by one from original string.
    // Places found lex in cur_lex and also returns it
    lex_type get_lex()
    {
        if (cur_index < expr_size) {
            int64_t val = 0;
            
            // read next symbol
            char c = expr[cur_index];
            ++cur_index;
            switch (c) {
                case ' ':
                    get_lex();
                    break;
                case '+':
                    cur_lex = PLUS;
                    break;
                case '-':
                    cur_lex = MINUS;
                    break;
                case '*':
                    cur_lex = STAR;
                    break;
                case '/':
                    cur_lex = SLASH;
                    break;
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    // count the number
                    while (isdigit(c)) {
                        val = 10 * val + c - '0';
                        c = expr[cur_index];
                        ++cur_index;
                    }
                    --cur_index; // in case we miss one symbol after number
                    
                    last_num_lex = val;
                    cur_lex = NUMBER;
                    break;
                default:
                    throw std::invalid_argument("Undefined symbol");
                    break;
            }
        } else {
            cur_lex = END;
        }
        return cur_lex;
    }

    // Next 5 functions implement recursive descent algorythm
    // Recursive descent starts from exp1()
    void number() // [-]NUMBER
    {
        int sgn = sign();
        if (cur_lex == NUMBER) {
            // add number to poliz
            poliz.push_back(std::pair<lex_type, int64_t>(NUMBER, sgn * last_num_lex));
            get_lex();
        } else {
            throw std::invalid_argument("Unexpected symbol (Number expected)");
        }
    }
    int sign() // [-]
    {
        if (cur_lex == MINUS) {
            get_lex();
            return -1;
        }
        return 1;
    }
    void exp1() // exp2() { +|- exp2() }
    {
        exp2();
        while (cur_lex == PLUS || cur_lex == MINUS) {
            lex_type tmp = cur_lex; // remember operation
            get_lex();
            exp2();
            // add remembered operation to poliz after exp2()
            poliz.push_back(std::pair<lex_type, int64_t>(tmp, 0));

        }
        end();
    }
    void exp2() // number() { *|/ number() }
    {
        number();
        while (cur_lex == STAR || cur_lex == SLASH) {
            lex_type tmp = cur_lex; // remember operation
            get_lex();
            number();
            // add remembered operation to poliz after number()
            poliz.push_back(std::pair<lex_type, int64_t>(tmp, 0));
        }
    }
    void end()
    {
        if (cur_lex != END) {
            throw std::invalid_argument("Unexpected symbol (end expected)");
        }
    }
    
    // Counts total result using poliz and stack
    int64_t count()
    {
        int64_t op1 = 0, op2 = 0;
        for (size_t i = 0; i < poliz.size(); ++i) {
            switch (poliz[i].first) {
                case NUMBER:
                    stack.push(poliz[i].second);
                    break;
                case PLUS:
                    op2 = stack.top();
                    stack.pop();
                    op1 = stack.top();
                    stack.pop();
                    stack.push(op1 + op2);
                    break;
                case MINUS:
                    op2 = stack.top();
                    stack.pop();
                    op1 = stack.top();
                    stack.pop();
                    stack.push(op1 - op2);
                    break;
                case STAR:
                    op2 = stack.top();
                    stack.pop();
                    op1 = stack.top();
                    stack.pop();
                    stack.push(op1 * op2);
                    break;
                case SLASH:
                    op2 = stack.top();
                    stack.pop();
                    op1 = stack.top();
                    stack.pop();
                    if (op2 == 0) {
                        throw std::invalid_argument("Zero devision");
                    }
                    stack.push(op1 / op2);
                    break;
                default:
                    throw std::invalid_argument("Undefined Lex");
                    break;
            }
        }
        return stack.top();
    }

public:
    Calculator(std::string str)
    {
        expr = str;
        expr_size = str.size();
        cur_index = 0;
        get_lex();
    }
    
    ~Calculator() {}
    
    void check()
    {
        exp1();
    }
    
    int64_t get_result()
    {
        int64_t result = count();
        return result;
    }
};


int main(int argc, char *argv[])
{
    try {
        if (argc != 2) {
            throw std::invalid_argument("");
        }
        
        Calculator calc(argv[1]);
        
        calc.check();
        std::cout << calc.get_result() << std::endl;
        return 0;
    }
    catch (std::invalid_argument&) {
        std::cout << "error" << std::endl;
        return 1;
    }
}
