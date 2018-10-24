#include <stdexcept>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cctype>
#include <cstdlib>

using namespace std;

class Calculator
{
 public:
    enum SIGN
    {
        PLUS = '+',
        MIN = '-',
        DIV = '/',
        MUL = '*'
    };
    string buf = "";
    int64_t Do (const string& str)
    {
        normalize(str);
        pos = 0;
        value = Getnum(buf);
        cur = value;
        if (str[pos]) {
            AddSub(buf);
        }
        return value;
    }
 private:
    int64_t value, cur, pos;

    void normalize(const string& str)
    {
        for (int i = 0; i < str.size(); i++) {
            if (isdigit(str[i]) || str[i] == PLUS || str[i] == MIN || str[i] == MUL ||str[i] == DIV) {
                buf += str[i];
            } else if (!isspace(str[i])) {
                throw invalid_argument("");
            }
        }
        int signs = 0;
        int64_t bufsize = buf.size();
        if (bufsize > 2 && !isdigit(buf[0]) && !isdigit(buf[1])) {
            throw invalid_argument("");
        }
        for (int i = 0; i < bufsize; i++) {
            if (isdigit(buf[i])) {
                if (signs >= 3) throw invalid_argument("");
                signs = 0;
            } else {
                signs++;
            }
        }
        if (!isdigit(buf[bufsize - 1])) throw invalid_argument("");
    }

    void MulDiv(int64_t& val, const string& str) {
        while(str[pos] == MUL || str[pos] == DIV) {
            int op = str[pos++];
            int64_t next = Getnum(str);
            if (op == MUL) {
                val *= next;
            } else {
                if (!next) throw invalid_argument("");
                val /= next;
            }
        }
    }
    void AddSub(const string& str) {
        int64_t op = 1;
        bool first = true;
        do {
            if (first && (str[pos] == MUL || str[pos] == DIV)) {
                first = false;
                cur = value;
                while(str[pos] && (str[pos] == MUL || str[pos] == DIV)) {
                    MulDiv(cur, str);
                }
                value += op * cur;
            } else {
                if (str[pos++] == '-') {
                op = -1;
                } else op = 1;
                cur = Getnum(buf);
                while(str[pos] && (str[pos] == MUL || str[pos] == DIV)) {
                    MulDiv(cur, str);
                }
                value += op * cur;
            }
        } while(str[pos]);

    }
    int64_t Getnum(const string& str)
    {
        char *last;
        const char *fst = &str[pos];
        int64_t res = strtoll(fst, &last, 10);
        pos += last - fst;
        return res;
    }

};
int main(int argc, char **argv)
{
    if (argc != 2) {
        cout << "error"<< endl;
		return 1;
    }
    Calculator calc;
    try
    {
        cout << calc.Do(argv[1]) << endl;
    }
    catch(invalid_argument&)
    {
        cout << "error" << endl;
        return 1;
    }
    return 0;
}
