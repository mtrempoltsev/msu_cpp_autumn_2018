#pragma once

#include <iostream>
#include <cstring>

class BigInt
{
public:
    BigInt(long long n = 0);
    BigInt(const BigInt&);
    BigInt& operator=(const BigInt&);
    BigInt operator-() const;
    BigInt operator+(const BigInt&) const;
    BigInt operator-(const BigInt&) const;
    bool operator==(const BigInt&) const;
    bool operator!=(const BigInt&) const;
    bool operator<(const BigInt&) const;
    bool operator<=(const BigInt&) const;
    bool operator>(const BigInt&) const;
    bool operator>=(const BigInt&) const;
    friend std::ostream& operator<<(std::ostream&, const BigInt&);
    ~BigInt();
private:
    void shrink();
    char* buf = nullptr;
    size_t size = 0;
    size_t capacity = min_capacity;
    bool sign = false;
    static const size_t min_capacity = 8;
};

BigInt::BigInt(long long n) : buf(new char[min_capacity]), sign(n < 0)
{
    std::memset(buf, 0, capacity * sizeof(char));
    if(n == 0)
        size = 1;
    n *= sign? -1 : 1;
    while(n)
    {
        buf[size++] = n % 10;
        n /= 10;
        if(size == capacity)
        {
            char* tmp = buf;
            buf = new char[capacity * 2];
            std::memcpy(buf, tmp, capacity * sizeof(char));
            capacity *= 2;
            delete[] tmp;
        }
    }
}

BigInt::BigInt(const BigInt& n) : size(n.size), capacity(n.capacity), sign(n.sign)
{
    if(buf != nullptr)
        delete[] buf;

    buf = new char[capacity];
    std::memcpy(buf, n.buf, capacity * sizeof(char));
}

BigInt& BigInt::operator=(const BigInt& n)
{
    if(buf != nullptr)
        delete[] buf;

    size = n.size;
    capacity = n.capacity;
    sign = n.sign;

    buf = new char[capacity];
    std::memcpy(buf, n.buf, capacity * sizeof(char));
}

BigInt BigInt::operator-() const
{
    BigInt n(*this);
    if(n != 0)
        n.sign ^= true;

    return n;
}

bool BigInt::operator==(const BigInt& n) const
{
    return sign == n.sign && size == n.size && !std::memcmp(buf, n.buf, size * sizeof(char));
}

bool BigInt::operator!=(const BigInt& n) const
{
    return !(*this == n);
}

bool BigInt::operator<(const BigInt& n) const
{
    if(sign == n.sign)
    {
        if(size == n.size)
        {
            int cmp = 0, i = size - 1;
            while(cmp == 0 && i >= 0)
            {
                cmp = buf[i] - n.buf[i];
                i--;
            }
            return (cmp != 0) && (sign ^ (cmp < 0));
        }
        else
            return (size < n.size) ^ sign;
    }
    else
        return sign;
}

bool BigInt::operator<=(const BigInt& n) const
{
    return (*this < n) || (*this == n);
}

bool BigInt::operator>(const BigInt& n) const
{
    return !(*this <= n);
}

bool BigInt::operator>=(const BigInt& n) const
{
    return !(*this < n);
}

void BigInt::shrink()
{
    while(buf[size - 1] == 0 && size > 1)
        size--;
    if(size == 1 && buf[0] == 0)
        sign = false;
}

BigInt BigInt::operator+(const BigInt& n) const
{
    BigInt result;
    result.capacity = std::max(capacity, n.capacity) + 1;
    delete[] result.buf;
    result.buf = new char[result.capacity];
    std::memset(result.buf, 0, result.capacity * sizeof(char));
    result.size = 0;
    char more = 0;
    if(sign == n.sign)
    {
        result.sign = sign;
        for(size_t i = 0; i < result.capacity; i++)
        {
            if(i >= size && i >= n.size && more == 0)
                break;
            char a = (i < size)? buf[i] : 0;
            char b = (i < n.size)? n.buf[i]: 0;
            result.buf[result.size++] = (more + a + b) % 10;
            more = (more + a + b) / 10;
        }
    }
    else if(sign)
    {
        if((-*this) < n)
        {
            result.sign = false;
            for(size_t i = 0; i < result.capacity; i++)
            {
                if(i >= size && i >= n.size && more == 0)
                    break;
                char a = (i < size)? buf[i] : 0;
                char b = (i < n.size)? n.buf[i] - more / 10: 0;
                more = (b >= a)? 0 : 10;
                result.buf[result.size++] = (more + b - a) % 10;
            }
        }
        else
        {
            result.sign = true;
            for(size_t i = 0; i < result.capacity; i++)
            {
                if(i >= size && i >= n.size && more == 0)
                    break;
                char a = (i < size)? buf[i] - more / 10: 0;
                char b = (i < n.size)? n.buf[i]: 0;
                more = (a >= b)? 0 : 10;
                result.buf[result.size++] = (more + a - b) % 10;
            }
        }
    }
    else
    {
        if((-n) >= *this)
        {
            result.sign = true;
            for(size_t i = 0; i < result.capacity; i++)
            {
                if(i >= size && i >= n.size && more == 0)
                    break;
                char a = (i < size)? buf[i] : 0;
                char b = (i < n.size)? n.buf[i] - more / 10: 0;
                more = (b >= a)? 0 : 10;
                result.buf[result.size++] = (more + b - a) % 10;
            }
        }
        else
        {
            result.sign = false;
            for(size_t i = 0; i < result.capacity; i++)
            {
                if(i >= size && i >= n.size && more == 0)
                    break;
                char a = (i < size)? buf[i] - more / 10: 0;
                char b = (i < n.size)? n.buf[i]: 0;
                more = (a >= b)? 0 : 10;
                result.buf[result.size++] = (more + a - b) % 10;
            }
        }
    }
    result.shrink();
    return result;
}

BigInt BigInt::operator-(const BigInt& n) const
{
    return *this + (-n);
}

std::ostream& operator<<(std::ostream& out, const BigInt& n)
{
    if(n.sign)
        out<<'-';

    for(size_t i = 0; i < n.size; i++)
        out << (int)n.buf[n.size - 1 - i];

    return out;
}

BigInt::~BigInt()
{
    delete[] buf;
}