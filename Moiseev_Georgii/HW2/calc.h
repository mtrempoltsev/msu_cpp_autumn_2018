#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <exception>


class Calculator
{
public:
    Calculator(const char* s)
    {
        if (convert(s) == false)
            throw std::invalid_argument("Wrong input");
    }
    
    int64_t getAns();

private:
    enum mathOp{ADD, SUBS, MULT, DIV};

    std::vector<int64_t> operands;
    std::vector<mathOp> operators;

    bool convert(const char* s);
    std::pair<bool, int64_t> compute(size_t leftInd, size_t rightInd);
};