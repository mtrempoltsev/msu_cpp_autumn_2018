#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>

using namespace std;

class Result {
public:
    bool error = 0;
    int64_t res = 0;
};

class Calculator {
    Result Calc_res;
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
                    Calc_res.error = 1;
                    throw Calc_res;
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
                    Calc_res.error = 1;
                    throw Calc_res;
                }
                res /= numbers[i + 1];
                break;
            }
        }

        return res;
    }

    void skip_space(char* &pos) {
        while (*pos == ' ')
            pos++;
    }

public:
    Calculator(char* str) {
        int64_t num;
        char op;
        char *pos, *left_pos;
        left_pos = str;
        skip_space(left_pos);

        if (*left_pos == '-') {
            left_pos++;
            num = strtoll(left_pos, &pos, 10);
            if (left_pos == pos || num <= 0) {
                Calc_res.error = 1;
                throw Calc_res;
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
                Calc_res.error = 0;
                Calc_res.res = calc(0, operations.size() - 1);
                throw Calc_res;
            }

            if (op != '+' && op != '-' && op != '*' && op != '/'){
                Calc_res.error = 1;
                throw Calc_res;
            }

            operations.push_back(op);
            pos++;
            skip_space(pos);
            left_pos = pos;

            if (*left_pos == '-') {
                left_pos++;
                num = strtoll(left_pos, &pos, 10);
                if (left_pos == pos || num <= 0) {
                    Calc_res.error = 1;
                throw Calc_res;
                }

                num = - num;
            }
            else
                num = strtoll(left_pos, &pos, 10);
        }
        Calc_res.error = 1;
        throw Calc_res;
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
        //Calculator Res("+ 1");
    } catch (Result &Res) {
        if (Res.error) {
            cout << "error" << endl;
            return 1;
        } else {
            cout << Res.res << endl;
            return 0;
        }
    }
    return 0;
}
