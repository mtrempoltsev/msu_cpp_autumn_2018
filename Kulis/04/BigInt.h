#pragma once
#include <algorithm>
#include <cstddef>
#include <iostream>

class BigInt
{
public:
    friend std::ostream& operator << (std::ostream &, const BigInt &);
    BigInt(int64_t value = 0);
    BigInt(const BigInt & bint);
    BigInt(BigInt && bint);
    ~BigInt();

    BigInt & operator= (const BigInt &);
    BigInt & operator= (BigInt &&);

    BigInt operator+   (const BigInt &) const;
    BigInt operator-   (const BigInt &) const;
    BigInt operator-   () const;

    bool operator<  (const BigInt &) const;
    bool operator<= (const BigInt &) const;
    bool operator>  (const BigInt &) const;
    bool operator>= (const BigInt &) const;
    bool operator== (const BigInt &) const;
    bool operator!= (const BigInt &) const;
private:
    static const size_t START_SIZE = 16;
    BigInt(size_t size, size_t capacity, char sign, char * data);

    BigInt abssum(const BigInt & left, const BigInt & right) const;
    BigInt abssub(const BigInt & left, const BigInt & right) const;
    int abscmp(const BigInt & left, const BigInt & right) const;

    void moreMemory();
    size_t capacity;
    size_t size;

    char * data;
    char sign;
};
