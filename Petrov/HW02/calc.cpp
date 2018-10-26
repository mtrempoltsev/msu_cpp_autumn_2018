#include<iostream>
#include<string>
#include<sstream>
#include<stdexcept>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::istringstream;
using std::invalid_argument;

class Calculator
{
public:

	Calculator(const string&);

	void calculation();
	int64_t result() const;

	~Calculator() {}

private:
	istringstream _ssexpr;
	int64_t _result;

	int64_t addition_subtraction();
	int64_t multiplication_division();
	int64_t number_parser();
	int64_t sign();
};

Calculator::Calculator(const string& expression) : _ssexpr(expression) {}

int64_t
Calculator::result() const
{
	return _result;
}

void
Calculator::calculation()
{
	_result = addition_subtraction();
}

int64_t
Calculator::addition_subtraction()
{
	int64_t left_argument = multiplication_division();

	char math_operator;
	while (_ssexpr >> math_operator && (math_operator == '+' || math_operator == '-')) {
		int64_t right_argument = multiplication_division();

		if (math_operator == '+') {
			left_argument += right_argument;
		}
		else {
			left_argument -= right_argument;
		}
	}

	if (_ssexpr) {
		throw invalid_argument("");
	}

	return left_argument;
}

int64_t
Calculator::multiplication_division()
{
	int64_t left_argument = number_parser();

	char math_operator;
	while (_ssexpr >> math_operator && (math_operator == '*' || math_operator == '/')) {
		int64_t right_argument = number_parser();

		if (math_operator == '*') {
			left_argument *= right_argument;
		}
		else {
			if (right_argument == 0) {
				throw invalid_argument("");
			}
			left_argument /= right_argument;
		}
	}

	if (_ssexpr) {
		_ssexpr.putback(math_operator);
	}

	return left_argument;
}

int64_t
Calculator::number_parser()
{
	int64_t s = sign();
	int64_t number;
	_ssexpr >> number;

	if (!_ssexpr) {
		throw invalid_argument("");
	}
	return s * number;
}

int64_t
Calculator::sign()
{
	char signum;
	_ssexpr >> signum;

	if (!_ssexpr) {
		throw invalid_argument("");
	}

	if (signum == '-')
	{
		return -1;
	}

	_ssexpr.putback(signum);
	return 1;
}


int main(int argc, char *argv[])
{
	if (argc != 2) {
		cerr << "error" << endl;
		return 1;
	}

	try {
		Calculator calc(argv[1]);
		calc.calculation();
		cout << calc.result() << endl;
	}
	catch (const invalid_argument&) {
		cerr << "error" << endl;
		return 1;
	}
	return 0;
}
