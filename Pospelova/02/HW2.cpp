#include <iostream>
#include <cstring>
#include <stdexcept>
//Calculator -> plus_minus '\0'
//Plus_minus -> Multiplication_division '+' Plus_minus |
//              Multiplication_division '-' Plus_minus |
//              Multiplication_division
//Multiplication_division -> Read_number '*' Multiplication_division |
//                           Read_number '/' Multiplication_division_no_zero |
//                           Read_number
//Read_number -> space Read_number | '-' Neg_number | Poz_number
//Neg_number -> space Neg_number | Poz_number
//Poz_number -> number Poz_number | End_number
//End_number -> space End_number | конец, для не space
//space -> ' ' | '\t' | '\n' | '\v' | '\f' | '\r'
//number -> '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'

class Calculator
{
private:
	int64_t answer;

	bool zero_division;
	bool empty_number;

	int64_t summand;
	int64_t sign_sum;
	int64_t multiplier;
	int64_t sign_multi;

	void plus_minus(const char *expression, int64_t &index);
	void multiplication_division(const char *expression, int64_t &index);
	void read_number(const char *expression, int64_t &index);
	void neg_number(const char *expression, int64_t &index);
	void poz_number(const char *expression, int64_t &index);
	void end_number(const char *expression, int64_t &index);
public:
	Calculator(const char *expression)
		: answer(0)
		, zero_division(false)
		, empty_number(true)
		, summand(1)
		, sign_sum(1)
		, multiplier(0)
		, sign_multi(1)
	{
		int64_t index = 0;
		plus_minus(expression, index);
		if ((index != strlen(expression)) || (zero_division) || (empty_number))
			throw std::invalid_argument("Invalid input expression");
	}
	int64_t get_answer() const {return answer;}
};

void Calculator::plus_minus(const char *expression, int64_t &index) {
	summand = sign_multi = 1;
	multiplication_division(expression, index);
	answer += sign_sum * summand;
	switch (expression[index])
	{
		case '+':
			sign_sum = 1;
			index++;
			plus_minus(expression, index);
			break;
		case '-':
			sign_sum = -1;
			index++;
			plus_minus(expression, index);
			break;
	}
}

void Calculator::multiplication_division(const char *expression, int64_t &index) {
	multiplier = 0;
	read_number(expression, index);
	if (sign_multi)
		summand *= multiplier;
	else {
		if (!multiplier) {
			zero_division = true;
		}
		else
			summand /= multiplier;
	}
	switch (expression[index])
	{
		case '*':
			sign_multi = 1;
			index++;
			multiplication_division(expression, index);
			break;
		case '/':
			sign_multi = 0;
			index++;
			multiplication_division(expression, index);
			break;
	}
}

void Calculator::read_number(const char *expression, int64_t &index) {
	if (isspace(expression[index])) {
		index++;
		read_number(expression, index);
		return;
	}
	if (expression[index] == '-') {
		index++;
		neg_number(expression, index);
		multiplier *= -1;
	}
	else
		poz_number(expression, index);
}

void Calculator::neg_number(const char *expression, int64_t &index) {
	if (isspace(expression[index])) {
		index++;
		neg_number(expression, index);
	}
	else
		poz_number(expression, index);
}

void Calculator::poz_number(const char *expression, int64_t &index) {
	if (isdigit(expression[index])) {
		empty_number = false;
		multiplier = multiplier * 10 + expression[index] - '0';
		index++;
		poz_number(expression, index);
	}
	end_number(expression, index);
}

void Calculator::end_number(const char *expression, int64_t &index) {
	if (isspace(expression[index])) {
		index++;
		end_number(expression, index);
	}
}

int main(int argc, char **argv) {
	try {
		if (argc != 2)
			throw std::invalid_argument("Wrong number of arguments");
		Calculator calculator_object(argv[1]);
		std::cout << calculator_object.get_answer() << std::endl;
	} catch(std::exception &e) {
		std::cerr << "error" << std::endl;
		return 1;
	}
	return 0;
}
