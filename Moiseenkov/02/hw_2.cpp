#include <iostream>
#include <cctype>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <sstream>

using namespace std;

class Calculator {
    using Int = int64_t;


public:

    Int Parse(const char* str) 
    {
        istringstream expression(str);
        return ParseAddSub(expression);
    }
private:

    struct Operations {
        static const char Add = '+';
        static const char Sub = '-';
        static const char Mult = '*';
        static const char Div = '/';
        static const char Min = '-';
    };

    Int ParseAddSub(istringstream& expression) 
    {
        Int left = ParseMulDiv(expression);
        char oper;

        while (expression >> oper && (oper == Operations::Add || oper == Operations::Sub)) {
            Int right = ParseMulDiv(expression);
            if (oper == Operations::Add)
                left += right;
            else
                left -= right;
        }
        if (expression)
            throw invalid_argument("Wrong expression_!");

        return left;
    }

    Int ParseMulDiv(istringstream& expression) {
        Int left = ParseNumber(expression);
        char oper;

        while (expression >> oper && (oper == Operations::Mult || oper == Operations::Div)) {
            Int right = ParseNumber(expression);
            if (oper == Operations::Mult)
                left *= right;
            else 
            {
                if (right == 0)
                    throw invalid_argument("Division by zero");
                left /= right;
            }
        }

        if (expression)
            expression.putback(oper);

        return left;
    }

    Int ParseNumber(istringstream& expression) 
    {
        Int sign = ParseNumberSign(expression);
        Int number;

        if (!(expression >> number))
            throw invalid_argument("Invalid input");

        return sign * number;
    }

    Int ParseNumberSign(istringstream& expression) 
    {
        char sign;

        if (!(expression >> sign))
            throw invalid_argument("Invalid input");

        if (sign == Operations::Min)
            return -1;

        expression.putback(sign);
        return 1;
    }




};

int main(int argc, char* argv[]) 
{
    const string error = "error";

    if (argc == 2) 
    {
        Calculator calc;

        try 
        {
            cout << calc.Parse(argv[1]) << '\n';
        } 
        catch (invalid_argument&) 
        {
            cerr << error;
            return 1;
        }
    } 
    else
    {
       cerr << error;
       return 1;
    }

    return 0;
};