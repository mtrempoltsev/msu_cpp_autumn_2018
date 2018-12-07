#include<iostream>
#include<string>
#include<sstream>
#include<exception>
using namespace std;

class Calculator
{
public:
	int64_t calculate(const char* str)
	{
		istringstream expr(str);
		return addition(expr);
	}

private:
	int64_t addition(istringstream& expr)
	{
		int64_t left = multiplication(expr);
		char o;
		while (expr >> o && (o == '+' || o == '-'))
		{
			int64_t right = multiplication(expr);
			if (o == '+')
				left += right;
			else
				left -= right;
		}
		return left;
	}

	int64_t multiplication(istringstream& expr)
	{
		int64_t left = numbers(expr);
		char o;
		while (expr >> o && (o == '*' || o == '/'))
		{
			int64_t right = numbers(expr);
			if (o == '*')
				left *= right;
			else
			{
				if (right == 0)
					throw invalid_argument("");
				left /= right;
			}
		}
		if (o != '*' || o != '/')
			expr.putback(o);
		return left;
	}

	int64_t numbers(istringstream& expr)
	{
		char num;
		int sign = 1;
		expr >> num;
		if (num == '-')
		{
			sign = -1;
			expr >> num;
		}
		if (num < '0' || num > '9')
			throw invalid_argument("");
		int64_t x = num - '0';
		return x * sign;
	}
};


int main(int argc, char* argv[])
{
	if (argc == 2)
	{
		Calculator calc;
		try
		{
			cout << calc.calculate(argv[1]) << endl;
		}
		catch (exception& err)
		{
			cout << "error" << endl;
			return 1;
		}
	}
	else
	{
		cout << "error" << endl;
		return 1;
	};
	return 0;
}