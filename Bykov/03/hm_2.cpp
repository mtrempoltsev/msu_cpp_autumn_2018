#include<string.h>
#include<iostream>
enum types{
    NUM, SUM, MIN, MUL, DIV, SPC
};

class ERROR{};

class leksem{
    types type;
    int64_t value;
    bool isSign(char c);
    size_t isNum(const char* str);
    int normalizeFirst();
public:
    leksem *next;
    leksem(const char *expr);
    types gettype(){
        return type;
    }
    int64_t getval(){
        return value;
    }
    void print(){
        leksem *l = this;
        while(l != nullptr){
             std::cout<<l->type<<' ';
             if(l->type == NUM)
                 std::cout<<l->value<<' ';
             l = l->next;
        }
    }
    void normalize();
    ~leksem(){
        leksem *l = this->next;
        if(l != nullptr)
            delete l;
        this->next = nullptr;
    }
};

leksem* deleteLex(leksem *l){
    leksem *lnext = l->next;
    free(l);
    return lnext;
}

int leksem::normalizeFirst(){
    leksem *l = this;
    int sign = 1;
    while((l->type == SPC) || (l->type == MIN)){
        if(l->type == MIN)
            sign *= -1;
        if(l->next == nullptr){
            this->type = SPC;
            return -1;
        }
        l = l->next;
    }
    if(l->type == NUM){
        this->type = NUM;
        this->value = l->value * sign;
        if(this != l){
            leksem *l2 = this->next;
            while(l2 != l){
                l2 = deleteLex(l2);
            }
            this->next = l->next;
            if(l != nullptr)
                free(l);
        }
    }
    else{
        this->type = SPC;
        return -1;
    }
    return 0;
}

void leksem::normalize(){
    if(normalizeFirst() == -1)
        return;
    leksem *lastImpLex = this;
    leksem *iterLex = this->next;
    int sign = 1;
    while(iterLex != nullptr)
        if(iterLex->type == SPC)
            iterLex = deleteLex(iterLex);
        else
            if(iterLex->type == MIN)
                if(lastImpLex->type != NUM){
                    sign *= -1;
                    iterLex = deleteLex(iterLex);
                }
                else{
                    lastImpLex->next = iterLex;
                    lastImpLex = lastImpLex->next;
                    iterLex = iterLex->next;
                }
             else{
                 lastImpLex->next = iterLex;
                 lastImpLex = lastImpLex->next;
                 iterLex = iterLex->next;
                 if(lastImpLex->type == NUM){
                    lastImpLex->value *= sign;
                    sign = 1;
                 }
             }
    lastImpLex->next = nullptr;
}

bool leksem::isSign(char c){
    switch(c){
        case '+':
            this->type = SUM;
            return 1;
        case '-':
            this->type = MIN;
            return 1;
        case '*':
            this->type = MUL;
            return 1;
        case '/':
            this->type = DIV;
            return 1;
        case ' ':
            this->type = SPC;
            return 1;
        default:
            return 0;
    }
}

size_t leksem::isNum(const char* str){
    if((*str > '9') || (*str < '0'))
        return 0;
    const char *iter = str;
    this->type = NUM;
    int64_t res=0;
    char digit;
    while(!((*iter > '9') || (*iter < '0'))){
        digit = *iter++ - '0';
        res = res * 10 + digit;
    }
    this->value = res;
    size_t n = iter - str;
    return n;
}

leksem::leksem(const char *expr){
    this->value = 0;
    const char *oldexpr = expr;
    int mv = 0;
    if(*expr != '\0')
          mv += isNum(expr);
          if(mv == 0)
              mv += isSign(*expr);
    ERROR e;
    if(mv == 0)
        throw e;
    expr += mv;
    if(*expr != '\0')
        this->next = new leksem(expr);
    else
         this->next = nullptr;
}

class CalcTree{
    CalcTree *left;
    CalcTree *right;
    types type;
    int64_t value;
public:
    CalcTree(leksem *l);
    int calc();
    ~CalcTree(){
        CalcTree *l = this;
        if(l->left != nullptr)
            delete l->left;
        l->left = nullptr;
        if(l->right != nullptr)
            delete l->right;
        l->right = nullptr;
    }
};

int CalcTree::calc(){
    switch(type){
        case SUM:
            return left->calc() + right->calc();
        case MIN:
            return left->calc() - right->calc();
        case MUL:
            return left->calc() * right->calc();
        case DIV:{
            int r = right->calc();
            ERROR e;
            if(r == 0)
                throw e;
            return left->calc() / r;
        }
        case NUM:
            return value;
    }
}

int getPrior(leksem *l){
    switch(l->gettype()){
        case SUM:
        case MIN: return 3;
        case MUL:
        case DIV: return 2;
        case NUM: return 1;
        default:
            return 0;
    }
}

leksem* getMaxPriorLeksem(leksem *l){
    if(l == nullptr)
        return nullptr;
    leksem *maxl = l;
    int max = getPrior(maxl);
    while(l != nullptr){
        if(getPrior(l) >= max){
            maxl = l;
            max = getPrior(l);
        }
        l = l->next;
    }
    if(max==0)
        return nullptr;
    return maxl;
}

CalcTree::CalcTree(leksem *l){
    left = nullptr;
    right = nullptr;
    leksem *leftLex = l;
    leksem *rightLex = getMaxPriorLeksem(l);
    ERROR e;
    if(rightLex == nullptr)
        throw e;
    if(rightLex == leftLex){
        value = l->getval();
        type = l->gettype();
        if(type != NUM)
            throw e;
        return;
    }
    leksem *l2 = l;
    while(l2->next != rightLex)
        l2 = l2->next;
    l2->next = nullptr;
    type = rightLex->gettype();
    value = rightLex->getval();
    left = new CalcTree(leftLex);
    right = new CalcTree(rightLex->next);
    l2->next = rightLex;
}

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cout<<"error";
        return 1;
    }
    leksem *l = nullptr;
    CalcTree *tree = nullptr;
    try{
        l = new leksem(argv[1]);
        l->normalize();
        tree = new CalcTree(l);
        std::cout<<tree->calc();
    }
    catch(ERROR &e){
        std::cout<<"error";
        return 1;
    }

    if(l != nullptr){
        delete l;
        l = nullptr;
    }
    if(tree != nullptr){
        delete tree;
        tree = nullptr;
    }
    return 0;
}
