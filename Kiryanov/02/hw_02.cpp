#include <iostream>
#include <stdlib.h>
#include <string>
#include <cctype>
#include <cstdlib>

using namespace std;

class Calculator
{
	using Int = int64_t;
	bool error;
	string expression;

public:
	Int result;
	Calculator(const string &input)
		:error(false),
		expression(edit(input)),
		result(0)
	{
		parse();
		if (error)
			throw invalid_argument("Invalid input");
	}

private:
	string edit(const string &input)
	{
		string a;
		for (int i = 0; i<input.length(); i++)
		{
			if ((input[i] >= '(') && (input[i] <= '9'))
				a += input[i];
			else if (input[i] != ' ')
			{	
				error = true;
				return "";
			}
		}
		return a;
	};

	void parse(void)
	{
		if (error)
			return;
		multiplication();
		Int res = result;
		while (expression.length() > 0)
		{
			char _operator = expression[0];
			expression.erase(0, 1);
			multiplication();
			switch (_operator)
			{
				case '+':
				{
					res += result;
					break;
				}
				case '-':
				{
					res -= result;
					break;
				}
			}
			result = res;
		}
	};

	void get_digit(void)
	{
		if (error)
			return;
		Int i = 0;
		Int sign = 1;
		string a;
		if (expression[0] == '-')
		{
			sign = -1;
			expression.erase(0,1);
		}
		while (isdigit(expression[i]))
		{
			a += expression[i];
			i++;
		}
		if (i == 0)
		{
			error = true;
			return;
		}
		result = sign * atoi(a.c_str());
		expression.erase(0, i);
	};

	void multiplication(void)
	{
		if (error)
			return;
		get_digit();
		Int res = result;
		while (expression[0] == '*' || expression[0] == '/')
		{
			char _operator = expression[0];
			expression.erase(0, 1);
			get_digit();
			switch (_operator)
			{
				case '*':
				{
					res *= result;
					break;
				}
				case '/':
				{
					if (result == 0)
					{
						error = true;
						return;
					}
					res /= result;
					break;
				}
			}
			result = res;
		}
	};

};
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cerr << "error";
		return 1;
	}
	const string s = string(argv[1]);
	try {
		Calculator b(s);
		cout << b.result << endl;
	}
	catch (invalid_argument&) {
		cerr << "error";
		return 1;
	}
	return 0;
}
