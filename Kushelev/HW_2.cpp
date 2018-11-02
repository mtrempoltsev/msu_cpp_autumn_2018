#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <stdexcept>

using namespace std;

class Calculator {
    vector <int64_t> numbers;
    vector <char> operations;

    int64_t calc(int l, int r) {
        if (l == r) {
            switch (operations[l]) {
            case '+':
                return numbers[l] + numbers[l + 1];
            case '*':
                return numbers[l] * numbers[l + 1];
            case '/':
                if (numbers[l + 1] == 0) {
                    throw runtime_error("");
                }
                return numbers[l] / numbers[l + 1];
            }
        }

        for (int i = l; i <= r; i++) {
            if (operations[i] == '+') {
                int64_t a, b;
                if (i == l)
                    a = numbers[i];
                else
                    a = calc(l, i - 1);
                if (i == r)
                    b = numbers[i + 1];
                else
                    b = calc(i + 1, r);
                return a + b;
            }
        }

        int64_t res = numbers[l];

        for (int i = l; i <= r; i++) {
            switch (operations[i]) {
            case '*':
                res *= numbers[i + 1];
                break;
            case '/':
                if (numbers[i + 1] == 0) {
                    throw runtime_error("");
                }
                res /= numbers[i + 1];
                break;
            }
        }

        return res;
    }

    void skip_space(const char* &pos) {
        while (*pos == ' ')
            pos++;
    }
    void skip_space(char* &pos) {
        while (*pos == ' ')
            pos++;
    }

public:
    int64_t calc_res;
    Calculator(const char* str) {
        int64_t num;
        char op;
        char *pos;
        const char *left_pos;
        left_pos = str;
        skip_space(left_pos);

        if (*left_pos == '-') {
            left_pos++;
            num = strtoll(left_pos, &pos, 10);
            if (left_pos == pos || num <= 0) {
                throw runtime_error("");
            }

            num = - num;
        }
        else
            num = strtoll(left_pos, &pos, 10);

        while (left_pos != pos) {
            numbers.push_back(num);
            skip_space(pos);
            op = *pos;

            if (op == '\0') {
                for (int i = 0; i < operations.size(); i++) {
                        if (operations[i] == '-') {
                            operations[i] = '+';
                            numbers[i + 1] = - numbers[i + 1];
                        }
                }
                calc_res = calc(0, operations.size() - 1);
                return;
            }

            if (op != '+' && op != '-' && op != '*' && op != '/'){
                throw runtime_error("");
            }

            operations.push_back(op);
            pos++;
            skip_space(pos);
            left_pos = pos;

            if (*left_pos == '-') {
                left_pos++;
                num = strtoll(left_pos, &pos, 10);
                if (left_pos == pos || num <= 0) {
                    throw runtime_error("");
                }

                num = - num;
            }
            else
                num = strtoll(left_pos, &pos, 10);
        }
        throw runtime_error("");
    }
};

int main(int argc, char* argv[])
{
    if (argc > 2 || argc == 1) {
        cout << "error" << endl;
        return 1;
    }

    try {

        Calculator Res(argv[1]);
        cout << Res.calc_res;
        return 0;
    } catch (exception &err) {
        cout << "error"<< endl;
        return 1;
    }
    return 0;
}
