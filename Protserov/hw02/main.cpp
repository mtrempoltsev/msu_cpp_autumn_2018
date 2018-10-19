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

namespace Calc
{
    Int read_minus(istringstream& ssexpr);
    Int read_num(istringstream& ssexpr);
    Int mul_div(istringstream& ssexpr);
    Int sum_sub(istringstream& ssexpr);
    Int calculate(const string& expr);
}

int main(int argc, char* argv[])
{
    const char* argerr = "error";
    if (argc != 2) {
        cerr << argerr << endl;
        return 1;
    }
    const char* arithexpr = argv[1];
    try {
        cout << Calc::calculate(arithexpr) << endl;
        return 0;
    } catch (invalid_argument&) {
        cerr << argerr << endl;
        return 1;
    }
    return 1;
}

Int Calc::calculate(const string& expr)
{
    istringstream ssexpr{expr};
    return sum_sub(ssexpr);
}

Int Calc::sum_sub(istringstream& ssexpr)
{
    Int larg = mul_div(ssexpr);
    char op;
    while ((ssexpr >> op) && (op == '+' || op == '-')) {
        Int rarg = mul_div(ssexpr);
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

Int Calc::mul_div(istringstream& ssexpr)
{
    Int larg = read_num(ssexpr);
    char op;
    while ((ssexpr >> op) && (op == '*' || op == '/')) {
        Int rarg = read_num(ssexpr);
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

Int Calc::read_num(istringstream& ssexpr)
{
    Int sign = read_minus(ssexpr);
    Int num;
    ssexpr >> num;
    if (!ssexpr) {
        throw invalid_argument("");
    }
    return sign * num;
}

Int Calc::read_minus(istringstream& ssexpr)
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
