#include <iostream>
#include <sstream>

struct Operations {
    static constexpr auto Add = '+';
    static constexpr auto Sub = '-';
    static constexpr auto Mul = '*';
    static constexpr auto Div = '/';
};

class Parser {
public:
    static int64_t Parse(const char* s);
private:
    static int64_t Atom(std::istringstream& expr);
    static int64_t DivMul(std::istringstream& expr);
    static int64_t AddSub(std::istringstream& expr);
};

int64_t Parser::Atom(std::istringstream& expr) {
    char c;
    int64_t x;

    expr >> c;

    if (c == Operations::Sub) {
        expr >> x;
        x = -x;
    } else {
        expr.putback(c);
        expr >> x;
    }

    if (!expr) {
        throw std::invalid_argument("Wrong expression!");
    }

    return x;
}

int64_t Parser::DivMul(std::istringstream& expr) {
    char op;
    auto lhs = Atom(expr);

    while (expr >> op && (op == Operations::Mul || op == Operations::Div)) {
        auto rhs = Atom(expr);
        switch (op) {
            case Operations::Mul: {
                lhs *= rhs;
            } break;
            case Operations::Div: {
                if (rhs == 0) {
                    throw std::invalid_argument("Zero division!");
                }
                lhs /= rhs;
            } break;
        }
    }

    if (expr) {
        expr.putback(op);
    }

    return lhs;
}

int64_t Parser::AddSub(std::istringstream& expr) {
    char op;
    auto lhs = DivMul(expr);

    while (expr >> op && (op == Operations::Add || op == Operations::Sub)) {
        auto rhs = DivMul(expr);
        switch (op) {
            case Operations::Add: {
                lhs += rhs;
            } break;
            case Operations::Sub: {
                lhs -= rhs;
            } break;
        }
    }

    if (expr) {
        throw std::invalid_argument("Wrong expression!");
    }

    return lhs;
}

int64_t Parser::Parse(const char* s) {
    std::istringstream expr(s);
    return AddSub(expr);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: ./calc 'expr'\n";
        return 1;
    }

    try {
        std::cout << Parser::Parse(argv[1]) << '\n';
    } catch (...) {
        std::cout << "error\n";
        return 1;
    }
}
