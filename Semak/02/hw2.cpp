#include <iostream>
#include <sstream>
#include <exception>

using namespace std;

int64_t calc(stringstream&);
int64_t MulDiv(stringstream&);
int64_t AddSub(stringstream&);

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		cout << "error" << endl;
		return 1;
	}

	stringstream expr(argv[1]);
	int64_t result;

	try
	{
		result = calc(expr);
	}
	catch(exception& ex)
	{
		cout << "error" << endl;
		return 1;
	}

	cout << result << endl;

	return 0;
}

int64_t calc(stringstream& expr)
{
    return AddSub(expr);
}

int64_t MulDiv(stringstream& expr)
{
    int64_t result;

    expr >> result;
    if(expr.fail())
    {
        throw runtime_error("error");
    }

    char op;
    expr >> op;
    while(isspace(op))
        expr >> op;

    if(expr.eof())
        return result;

    switch (op)
    {
        case '*':
            return result * MulDiv(expr);
        case '/':
        {
            int64_t tmp = MulDiv(expr);

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

int64_t AddSub(stringstream& expr)
{
    int64_t result;

    result = MulDiv(expr);

    if(expr.eof())
        return result;

    char op;
    expr >> op;
    while(isspace(op))
        expr >> op;

    switch (op)
    {
        case '+':
            return result + AddSub(expr);
        case '-':
            return result - AddSub(expr);
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