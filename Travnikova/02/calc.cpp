#include <stdexcept>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cctype>
#include <cstdlib>
#include <sstream>

using namespace std;

class Calculator
{
public:
	 int64_t Res(const string &str) {
		string s = del_spaces(str);
		istringstream expr(s);
		return add_sub(expr);
	}
private:
	string del_spaces(const string &str) {
		string s;
		int i = 0;
		while (i < str.length()) {
			if (isdigit(str[i]) || (str[i] == '+') || (str[i] == '-') || (str[i] == '*') || (str[i] == '/')) {
				s += str[i];
				i++;
				continue;
			}
			if (isspace(str[i])) {
				i++;
				continue;
			}
			throw invalid_argument("");
		}
		return s;
	}

	 int64_t add_sub(istringstream &expr)
	{
		int64_t r_num, l_num = div_mul(expr);
		int64_t res = l_num;
		char op;
		while (expr >> op)
		{
			r_num = div_mul(expr);
			if (op == '+') {
				res += r_num;
			}
			if (op == '-') {
				res -= r_num;
			}
		}
		return res;
	}

	int64_t div_mul(istringstream &expr)
	{
		int64_t r_num, l_num = number(expr);
		int64_t res = l_num;
		char op;
		while (expr >> op && (op == '*' || op == '/'))
		{
			r_num = div_mul(expr);
			if (op == '*') {
				res *= r_num;
			}
			if (op == '/') {
				if (r_num == 0) {
					throw invalid_argument("");
				}
				res /= r_num;
			}
		}
		if (expr)
			expr.putback(op);
		return res;
	}

	int64_t number(istringstream &expr)
	{
		char sign;
		int64_t sign1 = 1;
		if (!(expr >> sign))
			throw invalid_argument("");
		if (sign == '-') {
			sign1 = -1;
		} else {
			expr.putback(sign);
		}
		int64_t num = 0;
		if (!(expr >> num))
			throw invalid_argument("");
		num *= sign1;
		return num;
	}
};


int main(int argc, char **argv)
{
	if (argc != 2)	{
		cout << "error"<< endl;
		return 1;
	}
	Calculator a;
	try {
		cout << a.Res(argv[1]) << endl;
	}
	catch (invalid_argument&) {
		cout << "error" << endl;
		return 1;
	}
	return 0;
}
