#include <iostream>
#include <cstddef>
#include <cstdint>
#include <string>
#include <cstdlib>
#include <utility>

class Expression {
public:
	virtual ~Expression() {}
	virtual std::pair<int64_t, bool> get_result() = 0;
	virtual Expression *& get_left() = 0;
	virtual Expression *& get_right() = 0;
};

class Operation : public Expression {
public:
	Operation(char op) : _op(op), _left(NULL), _right(NULL) {}

	~Operation() {
		if (_left)
			delete _left;
		if (_right)
			delete _right;
	}

	Expression *& get_left() {
		return _left;
	}

	Expression *& get_right() {
		return _right;
	}

	std::pair<int64_t, bool> get_result() {
		std::pair<int64_t, bool> left_result = _left->get_result();
		std::pair<int64_t, bool> right_result = _right->get_result();
		
		switch (_op) {
			case '+':
				return std::make_pair(left_result.first + right_result.first, true);
			case '-':
				return std::make_pair(left_result.first - right_result.first, true);
			case '*':
				return std::make_pair(left_result.first * right_result.first, true);
			case '/':
				if (right_result.first == 0)
					return std::make_pair(0, false);
				else
					return std::make_pair(left_result.first / right_result.first, true);
		}
	}

private:
	char _op;
	Expression * _left;
	Expression * _right;
};

class Number : public Expression {
public:
	Number() : _value(0) {}
	
	Number(int64_t value) : _value(value) {}

	~Number() {}

	Expression *& get_left() {}
	Expression *& get_right() {}

	std::pair<int64_t, bool> get_result() {
		return std::make_pair(_value, true);
	}

private:
	int64_t _value;
};

bool parser(std::string const & s, Expression * & root) {
	if (s.size() == 0) {
		root = new Number();
		return true;
	}

	size_t pos = s.size() - 1;

	if (s[pos] == '+' || s[pos] == '-' || s[pos] == '*' || s[pos] == '/')
		return false;

	while (s[pos] != '+' && s[pos] != '-' && pos > 0)
		--pos;

	if (pos == 0) {
		pos = s.size() - 1;
		while (s[pos] != '*' && s[pos] != '/' && pos > 0)
			--pos;
	}

	if (pos == 0) {
		int64_t value = atoi(s.c_str());

		if (value == 0) {
			if (s.size() == 1 && s[0] != '0')
				return false;
			if (s.size() == 2 && s[0] != '-' && s[1] != '0')
				return false;
			return false;
		}
	
		root = new Number(value);
		return true;
	}

	switch (s[pos]) {
		case '+':
			root = new Operation('+');
			break;
		case '-':
			root = new Operation('-');
			break;
		case '*':
			root = new Operation('*');
			break;
		case '/':
			root = new Operation('/');
			break;
	}

	bool l = parser(s.substr(0, pos), root->get_left());
	bool r = parser(s.substr(pos + 1, s.size() - pos), root->get_right());

	return l && r;
}

std::string prepare(std::string const & s) {
	std::string new_string = "";
	size_t minus_counter = 0;

	for (size_t i = 0; i < s.size(); ++i) {
		if (s[i] == ' ')
			continue;
	
		if (s[i] == '-') {
			++minus_counter;
			continue;
		}
	
		if (minus_counter != 0) {
			if (minus_counter % 2 == 0)
				new_string.push_back('+');
			else
				new_string.push_back('-');

			minus_counter = 0;
		}
	
		new_string.push_back(s[i]);
	}

	std::string result = "";

	for (size_t i = 0; i < new_string.size() - 1; ++i) {
		if (new_string[i] == '+' && new_string[i + 1] == '-') {
			++i;
			result.push_back('-');
			continue;
		}

		result.push_back(new_string[i]);
	}

	result.push_back(new_string[new_string.size() - 1]);

	return result;
}

int main(int argc, char * argv[]) {
	if (argc != 2) {
		std::cout << "error" << std::endl;
		return 1;
	}

	Expression * root = NULL;

	bool ok = parser(prepare(std::string(argv[1])), root);

	if (ok) {
		std::pair<int16_t, bool> result = root->get_result();
		delete root;

		if (result.second) {
			std::cout << result.first << std::endl;
			return 0;
		} else {
			std::cout << "error" << std::endl;
			return 1;
		}
	} else {
		std::cout << "error" << std::endl;
		if (root)
			delete root;
		return 1;
	}

}