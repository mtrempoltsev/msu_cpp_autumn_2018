#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>


class Calc {
private:
	bool ISERROR = false;
	int64_t result;
	std::string s;

public:
	Calc(std::string &str)
		: s(str)
	{}

	bool count_result() {
		s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
		std::istringstream expr(s);
		result = add_sub(expr);
		return ISERROR;
	}

	int64_t get_result()
	{
		return result;
	}

	int64_t add_sub(std::istringstream &expr)
	{
		int64_t res = div_mul(expr);
		int64_t next_num;
		char op;

		while (expr >> op) {
			next_num = div_mul(expr);
			switch(op) {
				case '+':
					res += next_num;
					break;
				case '-':
					res -= next_num;
					break;
				default:
					ISERROR = true;
					return -1;
			}
		}
		return res;
	}

	int64_t div_mul(std::istringstream &expr)
	{
		int64_t next_num;
		int64_t res = get_number(expr);
		char op;

		while (expr >> op && (op == '*' || op == '/')) {
			next_num = div_mul(expr);

			switch (op) {
				case '*':
					res *= next_num;
					break;
				case '/':
					if (next_num == 0) {
						ISERROR = true;
						return -1;
					}

					res /= next_num;
					break;
			}
		}

		if (expr)
			expr.putback(op);

		return res;
	}


	int64_t get_number(std::istringstream &expr)
	{
		char sym_sign;
		int64_t sign = 1;
		int64_t number = 0;

		if (!(expr >> sym_sign)) {
			ISERROR = true;
			return -1;
		}
			
		if (sym_sign == '-') {
			sign = -1;
		} else {
			expr.putback(sym_sign);
		}

		if (!(expr >> number)) {
			ISERROR = true;
			return -1;
		}

		number *= sign;
		return number;
	}
};


int main(int argc, char* argv[])
{
	if (argc != 2) {
		std::cout << "error" << std::endl;
		return 1;
	}

	std::string s = argv[1];
	Calc c(s);

	if (c.count_result()) {
		std::cout << "error" << std::endl;
		return 1;
	} else {
		std::cout << c.get_result() << std::endl;
	}

	return 0;
}
