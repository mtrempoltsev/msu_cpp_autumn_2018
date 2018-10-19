#include <iostream>
#include <stdlib.h>
#include <string>
#include <cctype>
#include <cstdlib>

using namespace std;

class Result
{
public:
	int64_t result;
	string rest;
	Result(int64_t v, string r)
	{
		result = v;
		rest = r;
	}
};
class Calculator
{
	using Int = int64_t;

public:
	Int calculate(string input)
	{
		input = edit(input);
		Result result = parse(input);
		return result.result;
	};

private:
	string edit(const string s)
	{
		string a;
		for (int i = 0; i<s.length(); i++)
		{
			if ((s[i] >= '(') && (s[i] <= '9'))
				a += s[i];
			else if (s[i] != ' ')
				throw invalid_argument("Invalid input");
		}
		return a;
	};

	Result parse(const string input)
	{
		Result current = multiplication(input);
		Int res = current.result;
		string next;
		while (current.rest.length() > 0)
		{
			char _operator = current.rest[0];
			next = current.rest.substr(1);
			current = multiplication(next);
			switch (_operator)
			{
				case '+':
				{
					res += current.result;
					break;
				}
				case '-':
				{
					res -= current.result;
					break;
				}
			}
			current.result = res;
		}
		return current;
	};

	Result get_digit(string s)
	{
		Int i = 0;
		Int sign = 1;
		Int digit;
		string a;
		if (s[0] == '-')
		{
			sign = -1;
			s = s.substr(1);
		}
		while (isdigit(s[i]))
		{
			a += s[i];
			i++;
		}
		if (i==0)
			throw invalid_argument("Invalid input");
		digit = sign * atoi(a.c_str());
		s = s.substr(i);
		return Result(digit, s);
	};

	Result multiplication(const string s)
	{
		Result current = get_digit(s);
		Int res = current.result;
		while (current.rest[0]=='*'|| current.rest[0] == '/')
		{
			char _operator = current.rest[0];
			string next = current.rest.substr(1);
			current = get_digit(next);
			switch (_operator)
			{
				case '*':
				{
					res *= current.result;
					break;
				}
				case '/':
				{
					if (current.result==0)
						throw invalid_argument("Division by zero");
					res /= current.result;
					break;
				}
			}
			current.result = res;
		}
		return current;
	};

};
int main(int argc, char *argv[])
{
	if (argc!=2)
	{
		cerr << "error";
		return 1;
	}
	string s = string(argv[1]);
	Calculator b;
	try {
		cout << b.calculate(s) << endl;	
	}
	catch (invalid_argument&) {
		cerr << "error";
		return 1;
	}
    return 0;
}
