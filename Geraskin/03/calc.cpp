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
    explicit Parser(const char* s);
    int64_t GetResult() const;

private:
    std::istringstream Expr;
    const int64_t Result;

    int64_t Atom();
    int64_t DivMul();
    int64_t AddSub();
};

Parser::Parser(const char* s)
    : Expr(s)
    , Result(AddSub())
{
}

int64_t Parser::GetResult() const {
    return Result;
}

int64_t Parser::Atom() {
    char c;
    int64_t x;

    Expr >> c;

    if (c == Operations::Sub) {
        Expr >> x;
        x = -x;
    } else {
        Expr.putback(c);
        Expr >> x;
    }

    if (!Expr) {
        throw std::invalid_argument("Wrong Expression!");
    }

    return x;
}

int64_t Parser::DivMul() {
    char op;
    auto lhs = Atom();

    while (Expr >> op && (op == Operations::Mul || op == Operations::Div)) {
        auto rhs = Atom();
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

    if (Expr) {
        Expr.putback(op);
    }

    return lhs;
}

int64_t Parser::AddSub() {
    char op;
    auto lhs = DivMul();

    while (Expr >> op && (op == Operations::Add || op == Operations::Sub)) {
        auto rhs = DivMul();
        switch (op) {
            case Operations::Add: {
                lhs += rhs;
            } break;
            case Operations::Sub: {
                lhs -= rhs;
            } break;
        }
    }

    if (Expr) {
        throw std::invalid_argument("Wrong Expression!");
    }

    return lhs;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "error\n";
        return 1;
    }

    try {
        std::cout << Parser(argv[1]).GetResult() << '\n';
    } catch (std::exception& /*e*/) {
        std::cout << "error\n";
        return 1;
    }
}
