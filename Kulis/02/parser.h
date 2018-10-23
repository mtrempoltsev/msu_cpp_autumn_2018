#pragma once
#include <cstdint>

enum LexType {
    NONE = 0,
    VALUE = 1,
    PLUS = '+',
    MINUS = '-',
    MUL = '*',
    DIV = '/'
};

struct Lex
{
    Lex(LexType type = NONE, int64_t value = 0);

    int64_t value;
    LexType type;
};

// Takes a string in constuctor or in setString method,
// and returns Lexems from the string by getNext method.
// returns lexem with type NULL, if we gained the end of string
// and throws exceptions, if the string is illegal
class Parser
{
public:
    Parser(const char * str = nullptr);

    Lex getNext();
    void setString(const char * str);
private:
    // read value from string
    int64_t readValue();

    const char * str;

    // index of current symbol in string
    int i;
};
