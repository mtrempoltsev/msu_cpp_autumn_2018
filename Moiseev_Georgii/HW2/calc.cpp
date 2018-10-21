#include "calc.h"

inline bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

inline int64_t toDigit(char c)
{
    return c - '0';
}

bool Calculator::convert(const char *s)
{
    for (const char *p = s; *p != '\0';)
    {
        while ((*p) == ' ')
            p++;

        //operand section
        //unary munis subsection
        bool minusFlag = false;
        if ((*p) == '-')
        {
            minusFlag = true;
            p++;
        }
        //only digit symbols subsection
        int64_t num = 0;
        bool readNum = false;
        for (; isDigit(*p) || (*p) == ' '; p++)
            if (isDigit(*p))
            {
                num = num * 10 + toDigit(*p);
                readNum = true;
            }

        //only spaces till next non-digit case
        if (readNum == false)
            return false;

        if (minusFlag == true)
            num *= -1;

        operands.push_back(num);
        //operator section
        //expecting operator or end of string(after last operand) as non-space & non-digit
        switch (*p)
        {
            case '\0':
                break;

            case '+':
                operators.push_back(ADD);
                p++;
                break;

            case '-':
                operators.push_back(SUBS);
                p++;
                break;    

            case '*':
                operators.push_back(MULT);
                p++;
                break;

            case '/':
                operators.push_back(DIV);
                p++;
                break;

            default: 
                return false;
        }
    }

    //if last input unit is operator, operand expected
    if (operands.size() == operators.size())
        return false;

    return true;
}

std::pair<bool, int64_t> Calculator::compute(size_t leftInd, size_t rightInd)
{
    if (leftInd == rightInd)
        return {true, operands[leftInd]};

    //rightInd > 0
    for (size_t medianInd = rightInd; leftInd < medianInd; medianInd--)
    {
        if (operators[medianInd - 1] == ADD)
        {
            auto left = compute(leftInd, medianInd - 1);
            auto right = compute(medianInd, rightInd);
            return {left.first && right.first, left.second + right.second};
        }

        if (operators[medianInd - 1] == SUBS)
        {
            auto left = compute(leftInd, medianInd - 1);
            auto right = compute(medianInd, rightInd);
            return {left.first && right.first, left.second - right.second};
        }
    }

    //last operator is MULT or DIV
    if (operators[rightInd - 1] == MULT)
    {
        auto left = compute(leftInd, rightInd - 1);
            return {left.first, left.second * operands[rightInd]};
    }

    if (operators[rightInd - 1] == DIV)
    {   
        if (operands[rightInd] == 0)
            //division by zero error
            return {false, 1};

        auto left = compute(leftInd, rightInd - 1);
        return {left.first, left.second / operands[rightInd]};
    }
}

int64_t Calculator::getAns()
{
    auto res = compute(0, operators.size());

    if (res.first == false)
        throw std::runtime_error("Division by zero");

    return res.second;
}