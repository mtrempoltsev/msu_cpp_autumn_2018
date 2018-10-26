#pragma once
#include <cstdint>
#include "parser.h"

class Calculator
{
public:
    int64_t calculate(const char * str);
private:
    // takes next lexem
    const Lex & getNext();

    // recursive descent
    int64_t plusminus();
    int64_t muldiv();
    int64_t atom();

    // string parser
    Parser parser;

    // contains current lexem
    Lex cur;
};
