#include <iostream>
#include <exception>
#include <string>
#include <assert.h>

enum class Operations : char {
    NUMBER, PLUS='+', SUB='-', MUL='*', DIV='/', MINUS = '-'
};


class Calc {
    using Int = int64_t;
    using Size = size_t;
    
    class Expression {
    public:
        Int value = 0;
        Operations operation = Operations::NUMBER;
        Expression *left = nullptr;
        Expression *right = nullptr;
    };
    
public:
    
    Int ans(const std::string &s) {
        Expression *pExpression = CreateExpression(s);
        const Int result = CalculateExpression(pExpression);
        DisposeExpression(pExpression);
        return result;
    }
        
    
private:
    
    Int CalculateExpression(Expression *expr) {
        if (expr->operation == Operations::NUMBER) return expr->value;
        
        assert(expr->left);
        assert(expr->right);
        CalculateExpression(expr->left);
        CalculateExpression(expr->right);
    
        switch (expr->operation) {
            case Operations::PLUS:
                expr->value = expr->left->value + expr->right->value;
                break;
                
            case Operations::MINUS:
                expr->value = expr->left->value - expr->right->value;
                break;
                
            case Operations::MUL:
                expr->value = expr->left->value * expr->right->value;
                break;
                
            case Operations::DIV:
                if (expr->right->value == 0) {
                    const auto message = "Division by zero";
                    throw std::logic_error(message);
                }
                expr->value = expr->left->value / expr->right->value;
                break;
                
            case Operations::NUMBER:
                assert(false);
                break;
        }
        return expr->value;
    }
    
    void SkipSpaces(std::string &expression) {
        Size size = 0;
        
        while (size < expression.size() 
            && (expression[size] == ' ')) ++size;
        
        expression = expression.substr(size);
    }
    
    bool parseNumber(std::string &expression, Int &result) {
        
        std::string remainingStr = expression;
        
        SkipSpaces(remainingStr);
        
        int sign = 1;
        
        if(remainingStr[0] == '-') {
            sign = -1;
            remainingStr.erase(0,1);
            SkipSpaces(remainingStr);

        }
        
        Size size = 0;
        
        if (remainingStr.size() > 0 && isdigit(remainingStr[0])) {
        
            while(size < remainingStr.size() && isdigit(remainingStr[size])) ++size;
            
            result = std::stod(remainingStr.substr(0, size)) * sign;
            expression = remainingStr.substr(size);
            return true;
        }
        return false;
    }
    
    bool ParseOperator(std::string &expression, Operations &op)  {
        
        std::string remainingStr = expression;
        SkipSpaces(remainingStr);
        
        if (remainingStr.empty()) {
            op = Operations::NUMBER;
            return false;
        }

        switch (remainingStr[0]) {
        case '+':
            op = Operations::PLUS; 
            break;
        case '-':
            op = Operations::MINUS; 
            break;
        case '*':
            op = Operations::MUL; 
            break;
        case '/':
            op = Operations::DIV; 
            break;
        default:
            op = Operations::NUMBER; 
            break;
        }

        const bool succeed = (op != Operations::NUMBER);
        
        if (succeed) expression = remainingStr.substr(1);
        
        return succeed;
    }
       
    Expression *ParsePlusSub(std::string &str) {
        Expression *left = ParseMulDiv(str);
        while (true) {
            Operations op = Operations::NUMBER;

            std::string remainingStr = str;
            if (!ParseOperator(remainingStr, op)) return left;
            
            switch (op) {
            case Operations::PLUS:
            case Operations::SUB:
                break;
            default:
                return left;
            }
            
            str = remainingStr;

            Expression *right = nullptr;
            
            try {
                right = ParseMulDiv(str);
            } catch (...) {
                DisposeExpression(left);
                throw;
            }

            try {
                Expression *expr = new Expression;
                expr->left = left;
                expr->right = right;
                expr->operation = op;
                left = expr;
            } catch (...) {
                DisposeExpression(left);
                DisposeExpression(right);
                throw;
            }
        }
        return left;
    }

    Expression *ParseMulDiv(std::string &str){
        Expression *left = ParseAtom(str);
        while (true) {
            Operations op = Operations::NUMBER;

            std::string remainingStr = str;
            if (!ParseOperator(remainingStr, op)) return left;
            switch (op){
            case Operations::MUL:
            case Operations::DIV:
                break;
            default:
                return left;
            }
            str = remainingStr;

            Expression *right = nullptr;
            try {
                right = ParseAtom(str);
            } catch (...) {
                DisposeExpression(left);
                throw;
            }

            try {
                Expression *expr = new Expression;
                expr->left = left;
                expr->right = right;
                expr->operation = op;
                left = expr;
            } catch (...) {
                DisposeExpression(left);
                DisposeExpression(right);
                throw;
            }
        }
        return left;
    }

    Expression *ParseAtom(std::string &str) {
        Expression *expr = new Expression;
        
        if (!parseNumber(str, expr->value)) {
            DisposeExpression(expr);
            throw std::invalid_argument("Expected number at: " + str);
        }
        return expr;
    }

    Expression *CreateExpression(const std::string &expression) {
        std::string remainingStr = expression;
        Expression *pExpr = ParsePlusSub(remainingStr);

        SkipSpaces(remainingStr);
        
        if (!remainingStr.empty()) {
            const auto message = "Unexpected symbol at: " + remainingStr;
            throw std::runtime_error(message);
        }

        return pExpr;
    }

    void DisposeExpression(Expression *pExpression) {
        if (pExpression) {
            
            if (pExpression->left) {
                DisposeExpression(pExpression->left);
                pExpression->left = nullptr;
            }
            
            if (pExpression->right) {
                DisposeExpression(pExpression->right);
                pExpression->right = nullptr;
            }
            
            delete pExpression;
        }
    }

};


int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        std::cout << "error" << std::endl;
        return 1;
    }
    
    try {
        Calc calc;
        std::cout << calc.ans(argv[1]) << std::endl;
        return 0;
    } catch (...) {
        std::cout << "error" << std::endl;
        return 1;
    }
}
