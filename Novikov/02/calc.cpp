#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cstring>
#include <inttypes.h>

using namespace std;

class Recurs_descent_calc {
public:
	bool there_was_error;
	int64_t count_up(char* str_expr) {
		expr = str_expr;
		return add();
	}
	Recurs_descent_calc(): there_was_error(false) {}
private:
	char* expr;
	void eat_up_next_spaces() { for(; expr[0] == ' '; expr++); }
	int64_t add() {
		eat_up_next_spaces();
		int64_t counted = sub();
		eat_up_next_spaces();
		if(*expr == '+') { expr++; return counted + add(); }
		return counted;
	}
	int64_t sub() {
		eat_up_next_spaces();
		int64_t counted = mul_div();
		eat_up_next_spaces();
		if(*expr == '-') { expr++; return counted - sub(); }
		return counted;
	}
	int64_t mul_div() {
		eat_up_next_spaces();
		int64_t counted = minus();
		eat_up_next_spaces();
		if(*expr == '*') { expr++; return counted * mul_div(); }
		if(*expr == '/') {
			expr++;
			int64_t divider = mul_div();
			if(divider != 0) return counted / divider;
			there_was_error = true;
		}
		return counted;
	}
	int64_t minus() {
		eat_up_next_spaces();
		if(*expr == '-') {
			expr++;
			return -1 * minus();
		}
		return num();
	}
	int64_t number_of_digits(int64_t num) {
		int64_t q;
		for(q = 0; num != 0; q++) num = num / 10;
		return q;
	}
	int64_t num() {
		eat_up_next_spaces();
		int64_t number;
		if(*expr != '\0' && sscanf(expr, "%" SCNd64, &number)) {
			expr += number_of_digits(number);
			eat_up_next_spaces();
			return number;
		}
		there_was_error = true;
	}
};

int main(int argc, char* argv[]) {
	int64_t result;
	Recurs_descent_calc calc;			//Create a calculator instance
	if(argc != 2) calc.there_was_error = true;	//Сheck the correctness of input
	else result = calc.count_up(argv[1]);		//Feed him the entered expr to calculate
	if(calc.there_was_error) {			//Сheck the correctness of calculation
		cout << "error" << '\n';
		return 1;
	}
	cout << result << '\n';
	return 0;
}
