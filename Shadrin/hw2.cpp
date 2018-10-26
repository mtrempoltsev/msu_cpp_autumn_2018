#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

class Parser {

    enum Tok: char {
      NUMBER,
      START,
      END,
      PLUS='+',
      MINUS='-',
      MUL='*',
      DIV='/'
    };

    Tok cur_tok;

    int64_t number_value;

    std::istringstream parse_stream;

    void get_tok();

    int64_t expr(bool);

    int64_t prim(bool);

    int64_t term(bool);

public:

    Parser(const std::string &);

    int64_t solve();

};

Parser::Parser(const std::string &s)
    : cur_tok(START)
    , number_value()
    , parse_stream(s)
{ }

int64_t Parser::solve() {
    get_tok();

    if (cur_tok == END) {
        return 0;
    }

    return expr(false);
}

int64_t Parser::expr(bool is_get) {
    int64_t left = term(is_get);

    for (;;) {
        if (cur_tok == PLUS) {
            left += term(true);
        } else if (cur_tok == MINUS) {
            left -= term(true);
        } else {
            return left;
        }
    }

}

int64_t Parser::term(bool is_get) {
    int64_t left = prim(is_get);

    for (;;) {
        if (cur_tok == MUL) {
            left *= prim(true);
        } else if (cur_tok == DIV) {
            int64_t r = prim(true);
            if (r == 0) {
                throw std::invalid_argument("Divide by 0");
            }
            left /= r;
        } else {
            return left;
        }
    }

}

int64_t Parser::prim(bool is_get) {
    if (is_get && cur_tok != END) {
        get_tok();
    }

    if (cur_tok == NUMBER) {
        int64_t t = number_value;
        get_tok();
        return t;
    } else if (cur_tok == MINUS) {
        return -prim(true);
    } else {
        throw std::invalid_argument("invalid_arg");
    }
}

void Parser::get_tok() {
    char ch;

    do {    
        if (!parse_stream.get(ch)) {
            cur_tok = END;
            return;
        }
    } while (ch != '\n' && isspace(ch));

    if (ch == 0 || ch == EOF) {
        cur_tok = END;
    } else if (ch == MUL ||
            ch == DIV ||
            ch == PLUS ||
            ch == MINUS) {
        cur_tok = Tok(ch);
    } else if (isdigit(ch)) {
        parse_stream.putback(ch);
        parse_stream >> number_value;
        cur_tok = NUMBER;
    } else {
        throw std::invalid_argument("invalid_arg");
    }
}

void error(const std::string &error_message = "error") {
    std::cout << error_message << std::endl;
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        error();
        return 1;
    }

    try {
        std::string in(argv[1]);
        Parser p(in);
        int64_t result = p.solve();
        std::cout << result << std::endl;
    } catch (const std::invalid_argument &e) {
        error();
        return 1;
    }

}
