#pragma once

#include <iostream>
#include <cstdint>
#include <vector>


class calculator
{
public:
    calculator(const char* s) : convertStatus(convert(s)), computeStatus(false) {}

    bool getStatus()
    {
        return convertStatus && computeStatus;
    }
    
    bool printAns();

private:
    enum mathOp{ADD, SUBS, MULT, DIV};

    std::vector<int64_t> operands;
    std::vector<mathOp> operators;
    bool convertStatus;
    bool computeStatus;

    bool convert(const char* s);
    std::pair<bool, int64_t> compute(size_t leftInd, size_t rightInd);
};