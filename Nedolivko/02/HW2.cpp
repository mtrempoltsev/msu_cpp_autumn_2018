#include <cstdint>
#include <iostream>
#include <sstream>
#include <stack>

using namespace std;

class Calculator 
{
	stringstream str;
	stack <char> operations;
	stack <int64_t> numbers;
	int64_t result;
	bool minus_add;
	
	bool add();
	bool mult();
	bool num();
	
	bool parse();
	bool read_number(bool sign = false);
	char read_probels();
	
public:
	// calculator: add
	//        add: mult | add + mult | add - mult
        //       mult: num  | mult * num | mult / num 
	//        num: integer number|- integer number
	Calculator(const char *arg);
	bool calculate();
	void print();
};


int main(int argc, char *argv[])
{
	if (argc == 2) {
		Calculator calc(argv[1]);
		if (calc.calculate()){
			calc.print();
		} else {
			return 1;
		}
	} else {
		std::cout << "error" << std::endl;
		return 1;
	}
	return 0;
}

Calculator::Calculator(const char *arg): result(0), str(stringstream(arg)), numbers(), operations(){}

bool Calculator::parse()
{
	char cur_c;
	bool number_f;
	while (not str.eof()){
		// read number
		cur_c = read_probels();
		if (str.eof()) {
		    return false;
		}
		if (cur_c == '-') {
			number_f = read_number(true);
		} else if (cur_c >= '0' && cur_c <= '9') {
			str.unget();
			number_f = read_number();
		} else if (not str.eof()){
			number_f = false;
		}
		if (not number_f) 
			return false;
		// if not end of line, read operator
		if (not str.eof()) {
			cur_c = read_probels();
			if (cur_c == '+' or cur_c == '-' or cur_c == '/' or cur_c == '*') {
				operations.push(cur_c);
			} else if (not str.eof()){
				return false;
			}
		}
	}
	return true;
}

bool Calculator::read_number(bool sign) 
{
	int64_t number;
	str >> number;
	if (str.fail()) {
		return false;
	}
	if (sign) {
		number *= -1;
	}
	numbers.push(number);
	return true;
}

char Calculator::read_probels()
{
	char cur_c;
	do {
		str >> cur_c;
	} while ((cur_c == ' ')&&(not str.eof()));
	return cur_c;
}

bool Calculator::calculate()
{
	bool success = true;
	if (parse()){
		if (add()){
			result = numbers.top();
			numbers.pop();
		} else {
			success = false;
		}
	} else {
		success = false;
	}
	if (not success) 
		cout << "error" << endl;
	return success;
}

bool Calculator::add()
{
	char cur_c;
	bool success = mult();
	
	if (not operations.empty() && success){
		int64_t add_res = numbers.top();
		
		numbers.pop();
		cur_c = operations.top();
		operations.pop();
		if (cur_c == '+'){
			success = add();
			if (success) {
				add_res = numbers.top() + add_res;
				numbers.pop();
				numbers.push(add_res);
			}
		} else {
			success = add();
			if (success) {
				add_res = numbers.top() - add_res;
				numbers.pop();
				numbers.push(add_res);
			}
		}
	}
	return success;
}

bool Calculator::mult()
{
	char cur_c;
	bool success = true;
	
	if (not operations.empty()){
		int64_t mult_res = numbers.top();
		
		cur_c = operations.top();
		operations.pop();
		if (cur_c == '*'){
			
			numbers.pop();
			success = mult();
			if (success) {
				mult_res = numbers.top() * mult_res;
				numbers.pop();
				numbers.push(mult_res);
			}
		} else if (cur_c == '/') {
			
			numbers.pop();
			if (mult_res) {
				success = mult();
				mult_res = numbers.top() / mult_res;
				numbers.pop();
				numbers.push(mult_res);
			} else {
				success = false;
			}
		} else {
			operations.push(cur_c);
		}
	}
	return success;
}

void Calculator::print()
{
	cout << result << endl;
}
