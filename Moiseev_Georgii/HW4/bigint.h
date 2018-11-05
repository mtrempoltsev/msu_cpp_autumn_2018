#pragma once
#include <iostream>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <limits>

class BigInt
{
public:
    BigInt(int64_t x = 0);
    BigInt& operator=(const BigInt& copied);
    BigInt(const BigInt& copied);
    ~BigInt() { delete[] data;}

    BigInt operator+(const BigInt& b) const;
    BigInt operator-(const BigInt& b) const;
    BigInt operator-() const;
    bool operator==(const BigInt& b) const;
    bool operator!=(const BigInt& b) const {return !(*this == b);}
    bool operator<(const BigInt& b) const;
    bool operator>(const BigInt& b) const {return b < *this;}
    bool operator<=(const BigInt& b) const {return !(*this > b);}
    bool operator>=(const BigInt& b) const {return !(*this < b);}

    friend std::ostream& operator<<(std::ostream& stream, const BigInt& bigint);
private:
    enum sign_type{PLUS, MINUS};
    //create BigInt for operation result storage; inner usecase
    BigInt(size_t _data_size, sign_type _sign);

    BigInt sum(const BigInt& a, const BigInt& b, sign_type sign) const;
    BigInt diff(const BigInt& a, const BigInt& b, sign_type sign) const;
    bool absLessCompare(const BigInt& a, const BigInt& b) const;

    int64_t* data;
    size_t data_size;
    size_t size;
    //BASE must be 10^p, p >= 1
    constexpr static int64_t BASE = 1e18;
    static size_t start_size;

    sign_type sign;
};