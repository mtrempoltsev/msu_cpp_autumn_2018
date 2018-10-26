#include <iostream>
#include <sstream>
#include <string>


class Calculator{
    using Int = int64_t;

    std::istringstream expression;
    Int answer;

    bool addition_subtraction(){
        if(!multiplication_division(expression, answer))
            return false;

        char operation;

        while(expression >> operation && ((operation == '+') || (operation == '-'))){
            Int next_argument;
            if(!multiplication_division(expression, next_argument))
                return false;

            if(operation == '+')
                answer += next_argument;
            else
                answer -= next_argument;
        }
        if(expression) // left unparsed symbols
            return false;
        
        return true;
    }

    bool multiplication_division(std::istringstream &expr, Int &result){
        if(!read_number_from_expr(expr, result))
            return false;

        char operation;

        while(expr >> operation && ((operation == '*') || (operation == '/'))){
            Int next_argument;
            if(!read_number_from_expr(expr, next_argument))
                return false;

            if(operation == '*')
                result *= next_argument;
            else{
                if(next_argument == 0)
                    return false;
                result /= next_argument;
            }
        }
        // return extra read symbol
        expr.putback(operation);

        return true;
    }

    bool read_number_from_expr(std::istringstream &expr, Int &result){
        Int number;
        char sign;

        if(!read_sign(expr, sign))
            return false;

        expr >> number;
        if (!expr)
            return false;

        result = number;
        if(sign == '-')
            result *= -1;

        return true;
    }

    bool read_sign(std::istringstream &expr, char &sign){
        expr >> sign;
        if(!expr)
            return false;

        if(sign != '-')
            expr.putback(sign);

        return true;
    }

public:
    Calculator(const std::string &expr)
        :answer(0),
        expression(std::istringstream(expr)){}

    bool compute_answer(){
        return addition_subtraction();
    }

    Int get_answer(){
        return answer;
    }
};

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "error" << std::endl;
        return 1;
    }

    Calculator calc(argv[1]);

    if(calc.compute_answer())
        std::cout << calc.get_answer() << std::endl;  
    else{
        std::cout << "error" << std::endl;
        return 1;
    }

    return 0;
}