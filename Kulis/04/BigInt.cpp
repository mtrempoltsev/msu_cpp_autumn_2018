#include <cstring>
#include "BigInt.h"

const size_t BigInt::START_SIZE;

BigInt::BigInt(int value)
    : capacity(START_SIZE)
{
    data = new char[capacity]{0};
    sign = (value >= 0) - (value < 0);
    if (value < 0) {
        value = -value;
    } else if (value == 0){
        data[0] = 0;
        size = 1;
        return;
    }

    for (size = 0; value; ++size, value /= 10) {
        if (size == capacity)
            moreMemory();
        data[size] = value % 10;
    }
}

BigInt::BigInt(const BigInt & src)
    : capacity(src.capacity)
    , size(src.size)
    , sign(src.sign)
{
    data = new char[capacity]{0};
    memcpy(data, src.data, size);
}

BigInt::BigInt(BigInt && src)
    : capacity(src.capacity)
    , size(src.size)
    , sign(src.sign)
    , data(src.data)
{
    src.data = nullptr;
}

BigInt::BigInt(size_t size, size_t capacity, char sign, char * data)
    : size(size)
    , capacity(capacity)
    , sign(sign)
{
    this->data = new char[capacity]{0};
    memcpy(this->data, data, size);
}

BigInt::~BigInt()
{
    delete[] data;
}

const BigInt & BigInt::operator=(const BigInt & src)
{
    if (this == &src) {
        return *this;
    }

    capacity = src.capacity;
    size = src.size;
    sign = src.sign;
    char * tmp = new char[capacity]{0};
    delete[] data;
    data = tmp;
    memcpy(data, src.data, size);
    return *this;
}

const BigInt & BigInt::operator=(BigInt && src)
{
    capacity = src.capacity;
    size = src.size;
    sign = src.sign;
    delete[] data;
    data = src.data;
    src.data = nullptr;
}

BigInt BigInt::operator+(const BigInt & right) const
{
    // std::cout << size << " " << right.size << std::endl;
    if (this->sign == right.sign) {
        BigInt result = *this;
        sum(result, right);
        return std::move(result);
    } else {
        int cmp = abscmp(*this, right);
        if (cmp >= 0 && this->sign == 1 || cmp < 0 && this->sign == -1) {
            BigInt result = *this;
            sub(result, right);
            return std::move(result);
        } else if (cmp == 0) {
            return BigInt(0);
        } else {
            BigInt result = right;
            sub(result, *this);
            return std::move(result);
        }
    }
}

BigInt BigInt::operator-(const BigInt & right) const
{
    if (sign != right.sign) {
        BigInt result = *this;
        sum(result, right);
        return std::move(result);
    } else {
        int cmp = abscmp(*this, right);
        if (cmp == 0) {
            return BigInt(0);
        } else if (cmp > 0 && this->sign == 1 || cmp < 0 && this->sign == -1) {
            BigInt result = *this;
            sub(result, right);
            return std::move(result);
        } else {
            BigInt result = right;
            result.sign = -result.sign;
            sub(result, *this);
            return std::move(result);
        }
    }
}

BigInt BigInt::operator-() const
{
    if (*this != 0)
        return BigInt(size, capacity, -sign, data);
    return *this;
}

bool BigInt::operator<(const BigInt & right) const
{
    if (sign == right.sign) {
        int cmp = abscmp(*this, right);
        return (cmp < 0) && (sign == 1) || (cmp > 0) && (sign == -1);
    }
    return sign == -1;
}

bool BigInt::operator<=(const BigInt & right) const
{
    if (sign == right.sign) {
        int cmp = abscmp(*this, right);
        return (cmp <= 0) && (sign == 1) || (cmp >= 0) && (sign == -1);
    }
    return sign == -1;
}

bool BigInt::operator==(const BigInt & right) const
{
    if (sign == right.sign) {
        return abscmp(*this, right) == 0;
    }
    return false;
}

bool BigInt::operator>(const BigInt & right) const
{
    return !(*this <= right);
}

bool BigInt::operator>=(const BigInt & right) const
{
    return !(*this < right);
}

bool BigInt::operator!=(const BigInt & right) const
{
    return !(*this == right);
}

void BigInt::sum(BigInt & left, const BigInt & right) const
{
    char * ldata = left.data;
    const char * rdata = right.data;
    char additive = 0;
    int i;

    for (i = 0; i < right.size; ++i) {
        if (i >= left.capacity)
            left.moreMemory();
        char s = ldata[i] + rdata[i] + additive;
        ldata[i] = s % 10;
        additive = s / 10;
    }

    while (additive) {
        if (i == left.capacity)
            left.moreMemory();
        char s = ldata[i] + additive;
        ldata[i] = s % 10;
        additive = s / 10;
        i++;
    }
    left.size = std::max((size_t)i, left.size);
}

void BigInt::sub(BigInt & left, const BigInt & right) const
{
    char * ldata = left.data;
    const char * rdata = right.data;
    char additive = 0;
    int i;

    for (i = 0; i < std::min(left.size, right.size); ++i) {
        char s = ldata[i] - rdata[i] + additive;
        ldata[i] = (s + 10) % 10;
        additive = (s - 9) / 10;
    }

    while (additive) {
        if (i == left.capacity)
            left.moreMemory();
        char s = ldata[i] + additive;
        ldata[i] = (s + 10) % 10;
        additive = (s - 9) / 10;
        i++;
    }
    for (i = left.size - 1; i >= 0 && ldata[i] == 0; --i);
    // while (--i >= 0 && ldata[i] == 0);
    if (i < 0) {
        left.size = 1;
    } else {
        left.size = i + 1;
    }
}

int BigInt::abscmp(const BigInt & left, const BigInt & right) const
{
    if (left.size - right.size)
        return left.size - right.size;
    for (int i = left.size - 1; i >= 0; --i)
        if (left.data[i] - right.data[i])
            return left.data[i] - right.data[i];
    return 0;
}

void BigInt::moreMemory()
{
    capacity *= 2;
    char * tmp = new char[capacity]{0};
    memcpy(tmp, data, size);
    delete[] data;
    data = tmp;
}

std::ostream& operator << (std::ostream & stream, const BigInt & bint)
{
    if (bint.sign == -1) {
        stream << '-';
    }
    for (int i = bint.size - 1; i >= 0; --i) {
        stream << static_cast<int>(bint.data[i]);
    }
    return stream;
}
