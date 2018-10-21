#include <cctype>
#include <iostream>
#include <exception>
#include "parser.h"

Lex::Lex(LexType type, int64_t value)
    : type(type)
    , value(value)
{   }

Parser::Parser(const char * str)
    : str(str)
    , i(0)
{   }

int64_t Parser::readValue()
{
    int64_t value = 0;
    do {
        value = value * 10 + str[i] - '0';
        ++i;
    } while (isdigit(str[i]));

    return value;
}

Lex Parser::getNext()
{
    if (str == nullptr)
        throw std::runtime_error("null ptr to string");

    // ignore spaces
    while (isspace(str[i]))
        ++i;

    if (str[i] == '\0') {
        return Lex(NONE);
    } else if (isdigit(str[i])) {
        return Lex(VALUE, readValue());
    } else if (str[i] == '+'
            || str[i] == '-'
            || str[i] == '*'
            || str[i] == '/') {
        return Lex(LexType(str[i++]));
    } else {
        throw std::runtime_error("Unrecognized symbol");
    }
}

void Parser::setString(const char * str)
{
    this->str = str;
    this->i = 0;
}
