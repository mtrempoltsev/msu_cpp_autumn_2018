#include <iostream>
#include <vector>
#include <stack>
#include <cstdint>

void debug(int n)
{
    std::cout << "test" << n << std::endl;
}
//DEBUG

class Calc
{
public:
    Calc(std::string input): input(input) {}
    struct Lex {
        int64_t num;
        char op;
        Lex(char op = '\0', int64_t num = 0): num(num), op(op) {}
    };
    Lex cur_lex = Lex();
    std::string input;
    std::stack<Lex> st;
    size_t input_pos = 0;

    char getc()
    {
        if(input_pos < input.size()) {
            return input[input_pos++];
        } else {
            return '\0';
        }
    }

    void ungetc()
    {
        input_pos--;
    }

    void read_lex()
    {
        char c;
        do {
            c = getc();
        } while(c == ' ');
        // std::cout << "c=" << c << std::endl; //DEBUG
        if(isdigit(c)) {
            std::string buf;
            buf.push_back(c);
            while(c = getc()) {
                if(!isdigit(c)) {
                    break;
                }
                buf.push_back(c);
            }
            // std::cout << "(2)c=" << c << std::endl; //DEBUG
            if(c) {
                ungetc();
            }
            int64_t num = std::stoi(buf);
            cur_lex = Lex('n', num);
            return;
        } else if(c == '+' || c == '-'
        || c == '*' || c == '/'
        || c == '\0'
        ) {
            cur_lex = Lex(c);
            return;
        } else {
            std::cout << "error" << std::endl;
            cur_lex =  Lex();
            return;
        }
    }

    void parse()
    {
        read_lex();
        expr2();
    }

    void expr2()
    {
        // expr2 := expr1 {[+-] expr1}
        expr1();
        // debug(4); //DEBUG
        char op = cur_lex.op;
        while(op == '+' || op == '-') {
            // debug(5); //DEBUG
            read_lex();
            expr1();
            st.push(Lex(op));
            op = cur_lex.op;
        }
    }

    void expr1()
    {
        // expr1 := oper {[*/] oper}
        operand();
        char op = cur_lex.op;
        // std::cout << "saved op = " << op << std::endl; //DEBUG
        // debug(2); //DEBUG
        while(op == '*' || op == '/') {
            // debug(3); //DEBUG
            read_lex();
            operand();
            st.push(Lex(op));
            op = cur_lex.op;
        }
    }

    void operand()
    {
        // debug(1); //DEBUG
        // std::cout << "in operand: cur_lex.op == " << cur_lex.op << std::endl; //DEBUG
        if(cur_lex.op == '-') {
            read_lex();
            if(cur_lex.op = 'n') {
                st.push(Lex('n', -cur_lex.num));
            } else {
                std::cout << "error" << std::endl;
                return;
            }
        } else {
            if(cur_lex.op == 'n') {
                st.push(Lex('n', cur_lex.num));
            } else {
                std::cout << "error" << std::endl;
                return;
            }
        }
        read_lex();
        // std::cout << "operand read lex:" << cur_lex.op << cur_lex.num << std::endl; //DEBUG
    }

    // int64_t calc_one()
    // {
    //     Lex lex = st.top();
    //     st.pop();
    //     if(lex.op = 'n') {
    //         return lex.
    //     }
    //     int64_t op2 = calc_one();
    //     int64_t op1
    // }
};

int main(int argc, char* argv[]) {
    if(argc <= 1) {
        return -1;
    }
    Calc calc = Calc(std::string(argv[1]));
    // for(int i = 0; i < 20; ++i) {
    //     calc.read_lex();
    //     std::cout << calc.cur_lex.op << ' ' << calc.cur_lex.num << std::endl;
    // }

    calc.parse();
    while(!calc.st.empty()) {
        std::cout << calc.st.top().op << calc.st.top().num << std::endl;
        calc.st.pop();
    }
}
