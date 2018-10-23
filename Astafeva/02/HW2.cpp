#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>

using namespace std;

class Answer {
public:
    bool is_error = 0;
    int64_t res = 0;
};

class Calculator{
    Answer A;
    vector <int64_t> num;
    vector <char> op;
    void skip_sp(char* &pos) {
        while (*pos == ' ')
            ++pos;
    }
    int64_t calc(int l, int r) {
        if (l == r) {
            switch (op[l]) {
            case '+':
                return num[l] + num[l + 1];
            case '*':
                return num[l] * num[l + 1];
            case '/':
                if (num[l + 1] == 0) {
                    A.is_error = 1;
                    throw A;
                }
                return num[l] / num[l + 1];
            }
        }
        for (int i = l; i <= r; ++i) {
            if (op[i] == '+') {
                int64_t a, b;
                if (i == l)
                    a = num[i];
                else
                    a = calc(l, i - 1);
                if (i == r)
                    b = num[i + 1];
                else
                    b = calc(i + 1, r);
                return a + b;
            }
        }
        int64_t res = num[l];
        for (int i = l; i <= r; ++i) {
            switch (op[i]) {
            case '*':
                res *= num[i + 1];
                break;
            case '/':
                if (num[i + 1] == 0) {
                    A.is_error = 1;
                    throw A;
                }
                res /= num[i + 1];
                break;
            }
        }
        return res;
    }

public:
    Calculator(char *str) {
        num.resize(0);
        op.resize(0);
        int64_t c_num;
        char c_op;
        char *pos, *p_pos;
        p_pos = str;
        this->skip_sp(p_pos);
        if (*p_pos == '-') {
            ++p_pos;
            c_num = strtoll(p_pos, &pos, 10);
            if (p_pos == pos || c_num <= 0) {
                A.is_error = 1;
                throw A;
            }
            c_num = -c_num;
        } else
            c_num = strtoll(p_pos, &pos, 10);
        while (p_pos != pos) {
            num.push_back(c_num);
            skip_sp(pos);
            c_op = *pos;
            if (c_op == '\0') {
                for (int i = 0; i < op.size(); ++i)
                if (op[i] == '-') {
                    op[i] = '+';
                    num[i + 1] = -num[i + 1];
                }
                A.is_error = 0;
                A.res = this->calc(0, op.size() - 1);
                throw A;
            }
            if (c_op != '+' && c_op != '-' && c_op != '*' && c_op != '/'){
                A.is_error = 1;
                throw A;
            }
            ++pos;
            skip_sp(pos);
            op.push_back(c_op);
            p_pos = pos;
            if (*p_pos == '-') {
                ++p_pos;
                c_num = strtoll(p_pos, &pos, 10);
                if (p_pos == pos || c_num <= 0) {
                    A.is_error = 1;
                    throw A;
                }
                c_num = -c_num;
            } else
                c_num = strtoll(p_pos, &pos, 10);
        }
        A.is_error = 1;
        throw A;
    }
};

int main(int argc, char* argv[])
{
    if (argc > 2 || argc == 1) {
        cout << "error" << endl;
        return 1;
    }
    try {
        Calculator Expression(argv[1]);
    } catch (Answer &A) {
        if (A.is_error) {
            cout << "error" << endl;
            return 1;
        } else {
            cout << A.res << endl;
            return 0;
        }
    }
    return 0;
}
