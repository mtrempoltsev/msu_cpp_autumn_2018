#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <stdexcept>

std::string &deleteSpace(std::string &str)
{
    str.erase(remove_if(str.begin(), str.end(), isspace),str.end());
 
    return str;
}

enum Operation
{
    EMP, ADD, SUB, MUL, DIV,
};

struct Tree {
    int64_t value = 0;
    Operation op = EMP;
    Tree *leftBranch = nullptr;
    Tree *rightBranch = nullptr;
};

class calculator {
    
    int64_t resultExpr;
    std::string numExpr;

public:
    calculator(const char *expression): resultExpr(0), numExpr(std::string(expression)) 
    {

            deleteSpace(numExpr);
            Tree *treeExpr = Parse(numExpr);
            resultExpr = CalculateExp(treeExpr);
            DeleteExpr(treeExpr);
    }

    int64_t result() { return resultExpr; }

    bool DeleteExpr(Tree *expr)
    {
        if (expr) { 
            if (expr->leftBranch)
                DeleteExpr(expr->leftBranch);
            
            if (expr->rightBranch)
                DeleteExpr(expr->rightBranch);
            
            delete expr;
            
            return true;
        }
        return false;
    }

    int64_t CalculateExp(Tree *expr)
    {

        if (expr->op == EMP)
            return expr->value;

        CalculateExp(expr->leftBranch);
        CalculateExp(expr->rightBranch);

        switch (expr->op)
        {
        case ADD:
            expr->value = expr->leftBranch->value + expr->rightBranch->value;
            break;
        
        case SUB:
            expr->value = expr->leftBranch->value - expr->rightBranch->value;
            break;
        
        case MUL:
            expr->value = expr->leftBranch->value * expr->rightBranch->value;
            break;
        
        case DIV:
            if (expr->rightBranch->value)
                expr->value = expr->leftBranch->value / expr->rightBranch->value;
            else
                throw std::invalid_argument("error");
            break;

        case EMP:
            throw std::invalid_argument("error");
            break;
        }

        return expr->value;
    }
    
    Tree *Parse(std::string &str)
    {
        deleteSpace(str);
        
        Tree *full_expr = AddSub(str);
        
        return full_expr;
    }

    Tree *AddSub(std::string &str)
    {

        Tree *left = MulDiv(str);
        
        while (true)
        {
            Operation op = EMP;

            std::string tmp = str;

            if (!Operator(tmp, op) || (op != ADD && op != SUB))
                return left;

            str = tmp;

            Tree *right = nullptr;
            
            try {
                right = MulDiv(str);
            }
            
            catch (const std::invalid_argument& ia)
            {
                DeleteExpr(left);
                throw std::invalid_argument("error");
            }

            Tree *expr = new Tree;
            expr->leftBranch = left;
            expr->rightBranch = right;
            expr->op = op;
            left = expr;
        }

        return left;
    }

    Tree *MulDiv(std::string &str)
    {
        Tree *left = elemExpr(str);
        
        while (true)
        {
            Operation op = EMP;

            std::string tmp = str;

            if (!Operator(tmp, op) || (op != MUL && op != DIV))
                return left;
            
            str = tmp;
            
            Tree *right = nullptr;
            
            try {
                right = elemExpr(str);
            }

            catch (const std::invalid_argument& ia)
            {
                DeleteExpr(left);
                throw std::invalid_argument("error");
            }            
            
            Tree *expr = new Tree;
            expr->leftBranch = left;
            expr->rightBranch = right;
            expr->op = op;
            left = expr;
        }

        return left;
    }

    bool Operator(std::string &expression, Operation &op)
    {
        std::string tmp = expression;

        if (tmp.empty())
        {
            op = EMP;
            return false;
        }

        switch (tmp[0]) 
        {
        case '+':
            op = ADD; break;
        
        case '-':
            op = SUB; break;
        
        case '*':
            op = MUL; break;
        
        case '/':
            op = DIV; break;
        
        default:
            op = EMP; break;
        }

        if (op != EMP)
        {
            expression = tmp.erase(0, 1);
        }

        return (op != EMP);
    }

    Tree *elemExpr(std::string &str)
    {

        Tree *expr = new Tree;
        
        if (!Number(str, expr->value))
        {
            DeleteExpr(expr);
            throw std::invalid_argument("error");
        }

        return expr;
    }

    bool Number(std::string& expression, int64_t& value)
    {
        bool negative = false;
        size_t index = 0;

        if (expression[0] == '-')
        {
            negative = true;
            expression.erase(0, 1);
        }
            
        if (expression.size() > 0 && isdigit(expression[0]))
        {   
            while ( index < expression.size() && isdigit(expression[index]) ) {
                index++;
            }

            int64_t dec = 1;
            value = 0;

            for (int i = index - 1; i > -1; i--) {
                value += (expression.at(i) - '0') * dec;
                dec *= 10;
            }

            if ( negative )
                value = (-1) * value;

            expression.erase(0, index);
            
            return true;
        }
        
        return false;
    }

};

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cout << "error" << std::endl;
        return 1;
    }

    try {
        calculator myCalculator(argv[1]);
        std::cout << myCalculator.result() << std::endl;
    }

    catch (const std::invalid_argument& ia) {
        std::cerr << ia.what() << std::endl;
        return 1;
    }

    return 0;
}