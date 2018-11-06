#include <iostream>
#include <string.h>
#include <malloc.h>
#include <algorithm>

class BigInt {
    char* operand;
    size_t operand_size;
    bool is_neg;
    bool cmp_as_positive(const BigInt& a, const BigInt& b) const {
        if (a.operand_size > b.operand_size)
            return true;
        if (a.operand_size < b.operand_size)
            return false;
        for (int i = a.operand_size - 1; i >= 0; i--) {
            if (a.operand[i] < b.operand[i])
                return false;
            if (a.operand[i] > b.operand[i])
                return true;
        }
        return false;
    }
    bool one_is_not_positive(const BigInt& a, const BigInt& b) const {
        if (a.is_neg)
            return false;
        else
            return true;
    }
public:
    BigInt(int64_t a = 0) {
        if (a < 0) {
            is_neg = true;
            a*=-1;
        } else
            is_neg = false;
        std::string s = std::to_string(a);
        std::reverse(s.begin(), s.end());
        operand_size = s.size();
        operand = (char*) malloc(operand_size*sizeof(char));
        memcpy(operand, s.c_str(), operand_size*sizeof(char));

    }
    BigInt(const BigInt& b) {
        operand_size = b.operand_size;
        is_neg = b.is_neg;
        operand = (char*) malloc(operand_size*sizeof(char));
        memcpy(operand, b.operand, operand_size);
    }
    BigInt(BigInt&& b) {
        operand = b.operand;
        is_neg = b.is_neg;
        operand_size = b.operand_size;
        b.operand = nullptr;
    }
    ~BigInt() {
        if (operand != nullptr)
            free(operand);
    }
    BigInt& operator=(const BigInt& b) {
        if (this == &b)
            return *this;
        operand_size = b.operand_size;
        is_neg = b.is_neg;
        char* operand1 = (char*) malloc(operand_size*sizeof(char));
        if (operand_size != 0)
            free(operand);
        operand = operand1;
        memcpy(operand, b.operand, operand_size);
        return *this;
    }
    BigInt& operator=(BigInt&& b) {
        if (this == &b)
            return *this;
        operand = b.operand;
        is_neg = b.is_neg;
        operand_size = b.operand_size;
        b.operand = nullptr;
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& out, const BigInt& value);
    BigInt operator+(const BigInt& b1)const {
        BigInt answ;
        if (is_neg == true && b1.is_neg == true) {
            answ.is_neg = true;
        } else {
            if (is_neg == true) {
                BigInt dontlookatthis = *this;
                dontlookatthis.is_neg = false;
                BigInt t = b1 - dontlookatthis;
                return t;
            }
            if (b1.is_neg == true) {
                BigInt dontlookatthis = b1;
                dontlookatthis.is_neg = false;
                BigInt t = *this - dontlookatthis;
                return t;
            }
            answ.is_neg = false;
        }
        const BigInt* a = this;
        const BigInt* b = &b1;
        if (a->operand_size < b->operand_size)
            std::swap(a, b);
        char* o_1 = (char*)malloc(sizeof(char)*(a->operand_size + 1));
        free(answ.operand);
        answ.operand = o_1;
        answ.operand_size = a->operand_size+1;
        int more = 0;
        for (auto i = 0; i < a->operand_size; ++i) {
            if (i >= b->operand_size)
                answ.operand[i] = a->operand[i] + more;
            else
                answ.operand[i] = a->operand[i] + b->operand[i] - '0' + more;
            more = 0;
            if (answ.operand[i] > '9') {
                answ.operand[i]-=10;
                more = 1;
            }
        }
        if(more == 1)
            answ.operand[answ.operand_size - 1] = '1';
        else
            answ.operand_size -= 1;
        return answ;
    }

    BigInt operator-(const BigInt& b1) const {
        BigInt answ;
        if (is_neg == true && b1.is_neg == true) {
            answ.is_neg = true;
        } else {
            if (is_neg == true) {
                BigInt dontlookatthis = *this;
                dontlookatthis.is_neg = false;
                BigInt t = b1 + dontlookatthis;
                t.is_neg = true;
                return t;
            } else if (b1.is_neg== true) {
                BigInt dontlookatthis = b1;
                dontlookatthis.is_neg = false;
                BigInt t = *this + dontlookatthis;
                t.is_neg = false;
                return t;
            }
            answ.is_neg = false;
        }
        const BigInt* a = this;
        const BigInt* b = &b1;
        if (cmp_as_positive(*b,*a)) {
            std::swap(a, b);
            answ.is_neg = !answ.is_neg;
        }
        answ.operand_size = a->operand_size;
        char* o_1 = (char*)malloc(sizeof(char)*(a->operand_size + 1));
        free(answ.operand);
        answ.operand = o_1;
        int more = 0;
        for (int i = 0; i < a->operand_size; ++i) {
            if (i >= b->operand_size)
                answ.operand[i] = a->operand[i] - more;
            else
                answ.operand[i] = a->operand[i] - (b->operand[i] - '0') - more;
            more = 0;
            if (answ.operand[i] < '0') {
                answ.operand[i]+=10;
                more = 1;
            }
        }
        for (int i = answ.operand_size - 1; i > 0; --i) {
            if (answ.operand[i] == '0')
                answ.operand_size--;
            else
                break;
        }
        if (answ.operand[0] == '0' && answ.operand_size == 1)
            answ.is_neg = false;
        return answ;
    }
    BigInt operator-() const
    {
        BigInt tmp(*this);
        if (!(operand_size == 1 && operand[0] == '0'))
        tmp.is_neg = !is_neg;
        return tmp;
    }

    bool operator<(const BigInt& b) const {
        return b > (*this);
    }
    bool operator>(const BigInt& b) const {
        if (!(is_neg || b.is_neg)) {
            return cmp_as_positive(*this, b);
        } else if (is_neg && b.is_neg) {
            return cmp_as_positive(b, *this);
        }
        return one_is_not_positive(*this, b);
    }
    bool operator==(const BigInt& x) const {
        if (x.is_neg != is_neg || x.operand_size != operand_size)
            return false;
        for (int i=0;i<x.operand_size;++i) {
            if (x.operand[i] != operand[i]) return false;
        }
        return true;
    }
    bool operator!=(const BigInt& x) const {
        return !(*this == x);
    }
    bool operator<=(const BigInt& x) const {
        return (*this == x) || (*this < x);
    }
    bool operator>=(const BigInt& x) const {
        return (*this == x) || (*this > x);
    }
};
std::ostream& operator<<(std::ostream& out, const BigInt& value)
{
    if (value.is_neg)
        out << '-';
    for (int i = value.operand_size - 1; i >= 0; --i) {
        out << value.operand[i];
    }
}
