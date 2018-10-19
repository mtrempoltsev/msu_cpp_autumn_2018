#include <iostream>
#include <sstream>
#include <exception>

using namespace std;

class Calculator
{
public:

    explicit Calculator(const char*);
    int64_t calc(); // вычисляет результат выражения expr

private:
    // Грамматика для метода рекурсивного спуска (addSub - аксиома грамматики):
    // addSub --> mulDiv + addSub | mulDiv - addSub | mullDiv
    // mulDiv --> num * mulDiv | num / mulDiv | num
    // num --> -?[0-9]+
    int64_t addSub(bool);
    int64_t mulDiv();
    int64_t num();

    stringstream expr;
};

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		cout << "error" << endl;
		return 1;
	}

	int64_t result;

	try
	{
	    result = Calculator(argv[1]).calc();
	}
	catch(exception& ex)
	{
		cout << "error" << endl;
		return 1;
	}

	cout << result << endl;

	return 0;
}

Calculator::Calculator(const char* expr)
{
    this->expr = stringstream(expr);
}

int64_t Calculator::calc()
{
    return addSub(false);
}

int64_t Calculator::mulDiv()
{
    int64_t result = num();

    if(expr.eof())
        return result;

    char op;
    expr >> op;
    while(isspace(op))
        expr >> op;

    if(expr.eof())
        return result;

    switch (op)
    {
        case '*':
            return result * mulDiv();
        case '/':
        {
            int64_t tmp = mulDiv();

            if(tmp == 0)
                throw runtime_error("error");

            return result / tmp;
        }
        case '+':
        case '-':
        {
            expr.unget();
            return result;
        }
        default:
            throw runtime_error("error");
    }
}

int64_t Calculator::addSub(bool minusFlag)
{
    int64_t result;

    result = mulDiv();

    if(expr.eof())
        return result;

    char op;
    expr >> op;
    while(isspace(op))
        expr >> op;

    switch (op)
    {
        case '+':
            if(!minusFlag)
                return result + addSub(minusFlag);
            else
                return result - addSub(~minusFlag);
        case '-':
            if(!minusFlag)
                return result - addSub(~minusFlag);
            else
                return result + addSub(minusFlag);
        case '*':
        case '/':
        {
            expr.unget();
            return result;
        }
        default:
            throw runtime_error("error");
    }
}

int64_t Calculator::num()
{
    char sym;
    int64_t result;
    int sign = 1;

    expr >> sym;
    if(sym == '-')
        sign = -1;
    else
        expr.unget();

    expr >> sym;
    while(isspace(sym)) {
        expr >> sym;
    }
    expr.unget();

    expr >> result;
    if(expr.fail())
    {
        throw runtime_error("error");
    }
    return sign * result;
}