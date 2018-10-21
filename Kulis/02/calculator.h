#pragma once
#include <cstdint>
#include "lex.h"

class Calculator
{
public:
    int64_t calculate(const char * str);
private:
    // takes next lexem
    Lex getNext();

    // recursive descent
    int64_t plusminus();
    int64_t muldiv();
    int64_t atom();

    // string parser
    Parser parser;

    // contains current lexem
    Lex cur;
};
