#include<iostream>
#include<sstream>
#include<exception>
#include<set>
using namespace std;

class Calculator {
private:
    static const set<char> dictionary;
    string body;
public:
    Calculator (const string& body_);
    int64_t calculate();
private:
    int64_t expr1(stringstream& body_stream) const;
    int64_t expr2(stringstream& body_stream) const;
    int64_t number(stringstream& body_stream) const;
};


int main(int argc, char const *argv[]) {
    if (argc != 2) {
        cout << "error" << endl;
        return 1;
    }
    try {
        Calculator calc = Calculator(string(argv[1]));
        cout << calc.calculate() << endl;
    } catch (const invalid_argument& ex) {
        cout << "error" << endl;
        return 1;
    } catch (const runtime_error& ex) {
        cout << "error" << endl;
        return 1;
    }
    return 0;
}

Calculator::Calculator (const string& body_) {
    for (char c : body_) {
        if (!isspace(c)) {
            if (dictionary.count(c) == 0)
                throw invalid_argument("invalid sign: " + to_string(c));
            body += c;
        }
    }
}

const set<char> Calculator::dictionary = {
    '1', '2', '3', '4', '5', '6', '7',
    '8', '9', '0', '+', '-', '*', '/'
};

int64_t Calculator::calculate() {
    stringstream body_stream(body);
    return expr1(body_stream);
}

int64_t Calculator::expr2(stringstream& body_stream) const {
    int64_t arg1 = number(body_stream);
    if (body_stream.eof()) {
        return arg1;
    }
    char op = body_stream.get();
    if (op == '+' || op == '-' ) {
        body_stream.unget();
        return arg1;
    }
    if (body_stream.eof())
        throw invalid_argument("incorrect sequence:" + body);
    int64_t arg2 =  expr2(body_stream);
    int64_t result;
    switch (op) {
        case '*':
            result = arg1 * arg2;
            break;
        case '/':
            if (arg2 == 0) {
                throw invalid_argument("division by zero");
            }
            result = arg1 / arg2;
            break;
        default:
            throw runtime_error("invalid operation: " + to_string(op));
    }
    return result;
}

int64_t Calculator::expr1(stringstream& body_stream) const {
    int64_t arg1 = expr2(body_stream);
    if (body_stream.eof()) {
        return arg1;
    }
    char op = body_stream.get();
    if (body_stream.eof())
        throw invalid_argument("incorrect sequence:" + body);
    int64_t result;
    switch (op) {
        case '-':
            //2 - 2 == 2 + -2
            body_stream.unget();
        case '+':
            result = arg1 + expr1(body_stream);
            break;
        default:
            throw runtime_error("invalid operation: " + to_string(op));
    }
    return result;
}

int64_t Calculator::number(stringstream& body_stream) const {
    string string_num;
    int sign = 1;
    while (body_stream.get() == '-') {
        sign *= -1;
    }
    body_stream.unget();
    char digit;
    while (!body_stream.eof() && body_stream.get(digit)) {
        if (isdigit(digit)) {
            string_num += digit;
        } else {
            body_stream.unget();
            break;
        }
    }
    return stoi(string_num) * sign;
}