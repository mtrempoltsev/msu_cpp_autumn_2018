#include <iostream>
#include <cstring>
#include <cstdint>

class BigInt
{
private:
    int64_t positive;
    char *number;
    int size;
    char get_elem(int i) const 
    {
        if (i < size) {
            return number[i];
        }
        return 0;
    }
public:
    BigInt(int64_t init) : size(0),
                           number(NULL)
    {
        positive = init >= 0 ? 1 : -1;
        init = init >= 0 ? init : - init;
        number = new char[25];
        do {
            number[size++] = init % 10;
            init /= 10;
        } while (init);
    }
    
    BigInt(): BigInt(0) {}
    
    BigInt(const BigInt& n): number(NULL) 
    {
        *this = n;
    }
    
    ~BigInt() 
    {
        delete[] number;    
    }
    
    BigInt operator-() const {
        BigInt ans = *this;
        if (ans != BigInt(0)) {
            ans.positive *= -1;
        }
        return ans;
    }
    
    bool operator>(const BigInt &n) const 
    {
        if (positive != n.positive) {
            return positive > n.positive;
        }
        if (positive == -1) {
            return -n > -*this;   
        }
        if (size != n.size) {
            return size > n.size;
        }
        for (int i = size - 1; i >= 0; --i) {
            if (number[i] != n.number[i]) {
                return number[i] > n.number[i];
            }
        }
        return false;     
    }
    
    bool operator==(const BigInt &n) const 
    {
        if (*this > n || positive != n.positive || size != n.size) {
            return false;
        }
        for (int i = size - 1; i >= 0; --i) {
            if (number[i] != n.number[i]) {
                return false;
            }
        }
        return true;
    }
    
    BigInt& operator=(const BigInt &n)
    {
        if (this == &n) {
            return *this;
        }
        char *ptr = new char[n.size];
        delete[] number;
        number = ptr;
        positive = n.positive;
        size = n.size;
        memcpy(number, n.number, n.size);
        return *this;
    }
    
    BigInt& operator=(int64_t n)
    {
        BigInt tmp(n);
        *this = tmp;
        return *this;
    }
    
    bool operator!=(const BigInt &n) const 
    {
        return !(*this == n);   
    }
    
    bool operator<=(const BigInt &n) const 
    {
        return *this == n || *this < n;  
    }
    
    bool operator>=(const BigInt &n) const 
    {
        return *this == n || *this > n;  
    }
    
    bool operator<(const BigInt &n) const 
    {
        return !(*this >= n);   
    }
    
    BigInt operator-(const BigInt &n) const {
        if (positive * n.positive == -1) {
            if (positive == 1) {
                return *this + -n;   
            }
            else {
                return -(-*this + n);
            }
        }
        
        if (positive == -1) {
            return -n - -*this;    
        }
        
        if (n > *this) {
            return -(n - *this);   
        }
        
        BigInt ans(0);
        ans.number = new char[size];
        ans.size = 0;
        ans.positive = 1;
        int p = 0;
        while (ans.size < size) {
            ans.number[ans.size] = 10 + get_elem(ans.size) - n.get_elem(ans.size) - p;
            if (ans.number[ans.size] >= 10) {
                p = 0;
                ans.number[ans.size] -= 10;
            } else {
                p = 1;
            }
            ++ans.size;
        }
        
        for (int i = ans.size - 1; i >=1; i--) {
            if (ans.number[i] != 0)
                break;
            ans.size--;
        }
        
        return ans;
    }
    
    BigInt operator+(const BigInt &n) const {
        if (positive == -1 && n.positive == -1) {
            return -(-*this + -n);
        }
        if (positive * n.positive == -1) {
            if (*this > n) {
                return *this - -n;
            } else {
                return n - -*this;   
            }
        }
        int i = 0;
        BigInt ans(n);
        int max = n.size > size ? n.size : size;
        ans.number = new char[max + 1];
        ans.size = max;
        ans.positive = 1;
        int p = 0;
        while (i < max) {
            ans.number[i] = (get_elem(i) + n.get_elem(i) + p) % 10;
            p = (get_elem(i) + n.get_elem(i) + p) / 10;
            ++i;
        }
        if (p == 1) {
            ans.size++;
            ans.number[max] = 1;
        }
        return ans;
    }
    
    friend std::ostream& operator<<(std::ostream& out, const BigInt& n);
};

std::ostream& operator<<(std::ostream& out, const BigInt& n) 
{
     if (n.positive == -1) {
        out << '-';   
     }
    
     for (int i = 0; i < n.size; ++i) {
            char c = '0' + n.number[n.size-i-1];
            out << c;
     }

     return out;
}
