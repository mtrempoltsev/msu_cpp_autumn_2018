#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using std::cerr;
using std::endl;
using std::cout;
using std::string;
using std::invalid_argument;
using std::istringstream;

using Int = int64_t;

class Calc
{
    istringstream ssexpr;
    Int read_minus();
    Int read_num();
    Int mul_div();
    Int sum_sub();
public:
    explicit Calc(const string& expr): ssexpr{expr} {};
    Int get_result();
};

int main(int argc, char* argv[])
{
    const char* argerr{"error"};
    if (argc != 2) {
        cerr << argerr << endl;
        return 1;
    }
    const char* arithexpr{argv[1]};
    Calc mycalc{arithexpr};
    try {
        cout << mycalc.get_result() << endl;
        return 0;
    } catch (invalid_argument&) {
        cerr << argerr << endl;
        return 1;
    }
    return 1;
}

Int Calc::get_result()
{
    return sum_sub();
}

Int Calc::sum_sub()
{
    Int larg = mul_div();
    char op;
    while ((ssexpr >> op) && (op == '+' || op == '-')) {
        Int rarg = mul_div();
        if (op == '+') {
            larg += rarg;
        } else {
            larg -= rarg;
        }
    }
    if (ssexpr) {
        throw invalid_argument("");
    }
    return larg;
}

Int Calc::mul_div()
{
    Int larg = read_num();
    char op;
    while ((ssexpr >> op) && (op == '*' || op == '/')) {
        Int rarg = read_num();
        if (op == '*') {
            larg *= rarg;
        } else {
            if (rarg == 0) {
                throw invalid_argument("");
            }
            larg /= rarg;
        }
    }
    if (ssexpr) {
        ssexpr.putback(op);
    }
    return larg;
}

Int Calc::read_num()
{
    Int sign = read_minus();
    Int num;
    ssexpr >> num;
    if (!ssexpr) {
        throw invalid_argument("");
    }
    return sign * num;
}

Int Calc::read_minus()
{
    char minus;
    ssexpr >> minus;
    if (!ssexpr) {
        throw invalid_argument("");
    }
    if (minus == '-') {
        return -1;
    }
    ssexpr.putback(minus);
    return 1;
}
