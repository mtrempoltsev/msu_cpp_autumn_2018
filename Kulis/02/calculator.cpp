#include <iostream>
#include <exception>
#include "calculator.h"

Lex Calculator::getNext()
{
    return cur = parser.getNext();
}

int64_t Calculator::plusminus()
{
    int64_t answer = muldiv();
    while (cur.type == PLUS || cur.type == MINUS) {
        if (cur.type == PLUS) {
            getNext();
            answer += muldiv();
        } else {
            getNext();
            answer -= muldiv();
        }
    }
    return answer;
}

int64_t Calculator::muldiv()
{
    int64_t answer = atom();
    while (cur.type == MUL || cur.type == DIV) {
        if (cur.type == MUL) {
            getNext();
            answer *= atom();
        } else {
            getNext();
            int64_t second = atom();
            if (second == 0)
                throw std::runtime_error("zero div");
            answer /= second;
        }
    }
    return answer;
}

int64_t Calculator::atom()
{
    int sign = 1;
    while (cur.type == MINUS) {
        sign = -sign;
        getNext();
    }

    if (cur.type == VALUE) {
        int64_t answer = cur.value;
        getNext();
        return answer * sign;
    } else {
        throw std::runtime_error("Unexpected lexem");
    }
}

int64_t Calculator::calculate(const char * str) {
    parser.setString(str);

    // take first lexem for analysis
    getNext();
    return plusminus();
}
