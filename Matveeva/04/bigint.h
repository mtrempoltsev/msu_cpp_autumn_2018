#include<iostream>
#include<cstring>
#include<cstdlib>

using namespace std;

class BigInt{
    bool is_neg;
    char* value;
    size_t value_size;
public:
    BigInt(int64_t i = 0){
        if(i < 0){
            is_neg = true;
            i *= (-1);
        } else
            is_neg = false;
        string s = to_string(i);
        value_size = s.size();
        value = new char[value_size + 1];
        memcpy(value, s.c_str(), value_size + 1);
    }
    BigInt(const BigInt& other): value_size(other.value_size), is_neg(other.is_neg){
        value = new char [other.value_size + 1];
        memcpy(value, other.value, value_size + 1);
    }
    ~BigInt(){
        delete[] value;
    }
    BigInt& operator=(const BigInt& other){
        if (this == &other)
            return *this;
        char* ptr = new char[other.value_size];
        delete[] value;
        value = ptr;
        value_size = other.value_size;
        copy(other.value, other.value + value_size, value);
        return *this;
    }
    BigInt operator-() const{
        BigInt res(*this);
        if (is_neg)
            res.is_neg = false;
        else
            res.is_neg = true;
        return res;
    }
    BigInt operator+(const BigInt& other) const{
        if (is_neg != other.is_neg){
            return (*this - (-other));
        }
        BigInt res;
        BigInt a;
        BigInt b;
        int min_size;
        if (value_size > other.value_size){
            res.is_neg = is_neg;
            res.value_size = value_size;
            min_size = other.value_size;
            a = *this;
            b = other;
        } else {
            res.is_neg = other.is_neg;
            res.value_size = other.value_size;
            min_size = value_size;
            b = *this;
            a = other;
        }
        char* help = new char[res.value_size + 2];
        help[res.value_size+1] = '\0';
        int brain = 0;
        size_t i = 0;
        for (; i < res.value_size; i ++){
            if (i < min_size)
                help[res.value_size - i] = a.value[res.value_size - i - 1] + b.value[min_size - i - 1] - '0' + brain;
            else
                help[res.value_size - i] = a.value[res.value_size - i - 1]  + brain;
            brain = 0;
            if(help[res.value_size - i] > '9'){
                help[res.value_size - i] -= 10;
                brain = 1;
            }
        }
        char* help2;
        if (brain > 0){
            res.value_size++;
            help[0] = brain + '0';
            delete[] res.value;
            res.value = help;
        } else {
            delete[] res.value;
            help2 = new char [res.value_size + 1];
            memcpy(help2, help + 1, res.value_size + 1);
            res.value = help2;
            delete[] help;
        }
        i = 0;
        return res;
    }
    BigInt operator-(const BigInt& other) const{
        if (is_neg != other.is_neg)
            return ((*this) + (-other));
        BigInt a;
        BigInt b;
        BigInt res;
        int max_size = value_size;
        if ((value_size > other.value_size) || ((strcmp(value, other.value) > 0) && (value_size == other.value_size))){
            a = *this;
            b = other;
            res.is_neg = is_neg;
        } else if ((value_size < other.value_size) || ((strcmp(value, other.value) < 0) && (value_size == other.value_size))){
            b = *this;
            a = other;
            res.is_neg = !other.is_neg;
        } else {
            return res;
        }
        char* help = new char[a.value_size + 1];
        help[a.value_size] = '\0';
        int brain = 0;
        size_t i = 0;
        for (; i < a.value_size; i ++){
            if (i < b.value_size)
                help[a.value_size - i - 1] = a.value[a.value_size - i - 1] - (b.value[b.value_size - i -1] - '0') - brain;
            else
                help[a.value_size - i - 1] = a.value[a.value_size - i - 1] - brain;
            brain = 0;
            if (help[a.value_size - i - 1] < '0'){
                help[a.value_size - i - 1] += 10;
                brain = 1;
            }
        }
        for (i = 0; i < a.value_size; i ++){
            if (help[i] != '0')
                break;
        }
        res.value_size = a.value_size - i;
        delete[] res.value;
        char* help2 = new char [a.value_size + 1 - i];
        memcpy(help2, help + i, a.value_size + 1 - i);
        delete[] help;
        res.value = help2;
        return res;
    }
    bool operator<(const BigInt& other){
        if (is_neg != other.is_neg)
            return is_neg;
        if (value_size > other.value_size)
            return is_neg;
        if (value_size < other.value_size)
            return !is_neg;
        if ((!is_neg) && (strcmp(value, other.value) < 0))
            return true;
        if ((is_neg) && (strcmp(value, other.value) > 0))
            return true;
        return false;
    }
    bool operator==(const BigInt& other){
        if((value[0] == other.value[0]) && (value[0] == '0'))
            return true;
        if ((is_neg != other.is_neg) || (value_size != other.value_size))
            return false;
        if (strcmp(value, other.value) != 0)
            return false;
        return true;
    }
    bool operator>(const BigInt& other){
        return !((*this < other) || (*this == other));
    }
    bool operator<=(const BigInt& other){
        return !(*this > other);
    }
    bool operator>=(const BigInt& other){
        return !(*this < other);
    }
    bool operator!=(const BigInt& other){
        return !(*this == other);
    }
    friend ostream& operator<<(ostream& out, const BigInt& x);
};

ostream& operator<<(ostream& out, const BigInt& x){
    if (x.is_neg)
        out<<'-';
    for (int i = 0; i < x.value_size; i ++)
        out<<x.value[i];
    return out;
}
