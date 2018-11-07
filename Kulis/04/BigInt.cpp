#include <cstring>
#include "BigInt.h"

const size_t BigInt::START_SIZE;

BigInt::BigInt(int64_t value)
    : capacity(START_SIZE)
    , sign((value > 0) - (value < 0))
{
    data = new char[capacity]{0};

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

BigInt & BigInt::operator=(const BigInt & src)
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

BigInt & BigInt::operator=(BigInt && src)
{
    capacity = src.capacity;
    size = src.size;
    sign = src.sign;
    delete[] data;
    data = src.data;
    src.data = nullptr;
    return *this;
}

BigInt BigInt::operator+(const BigInt & right) const
{
    if (this->sign == 0) {
        return right;
    } else if (right.sign == 0) {
        return *this;
    } else if (this->sign == right.sign) {
        return abssum(*this, right);
    } else {
        int cmp = abscmp(*this, right);
        if (cmp == 0) {
            return BigInt(0);
        } else if (cmp > 0) {
            return abssub(*this, right);
        } else {
            return abssub(right, *this);
        }
    }
}

BigInt BigInt::operator-(const BigInt & right) const
{
    if (this->sign == 0) {
        return BigInt(right.size, right.capacity,
            -right.sign, right.data);
    } else if (right.sign == 0) {
        return *this;
    } else if (sign != right.sign) {
        return abssum(*this, right);
    } else {
        int cmp = abscmp(*this, right);
        if (cmp == 0) {
            return BigInt(0);
        } else if (cmp > 0) {
            return abssub(*this, right);
        } else {
            BigInt result = abssub(right, *this);
            result.sign = -result.sign;
            return result;
        }
    }
}

BigInt BigInt::operator-() const
{
    return BigInt(size, capacity, -sign, data);
}

bool BigInt::operator<(const BigInt & right) const
{
    if (sign == right.sign){
        int cmp = abscmp(*this, right);
        return (cmp < 0) && (sign >= 0) || (cmp > 0) && (sign == -1);
    }
    return sign < right.sign;
}

bool BigInt::operator<=(const BigInt & right) const
{
    if (sign == right.sign) {
        int cmp = abscmp(*this, right);
        return (cmp <= 0) && (sign >= 0) || (cmp >= 0) && (sign == -1);
    }
    return sign < right.sign;
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

BigInt BigInt::abssum(const BigInt & left, const BigInt & right) const
{
    BigInt answer = left;
    char * ansdata = answer.data;
    const char * rdata = right.data;
    char additive = 0;
    int i;

    for (i = 0; i < right.size; ++i) {
        if (i >= answer.capacity)
            answer.moreMemory();
        char s = ansdata[i] + rdata[i] + additive;
        ansdata[i] = s % 10;
        additive = s / 10;
    }

    while (additive) {
        if (i == answer.capacity)
            answer.moreMemory();
        char s = ansdata[i] + additive;
        ansdata[i] = s % 10;
        additive = s / 10;
        i++;
    }
    answer.size = std::max((size_t)i, answer.size);
    return answer;
}

BigInt BigInt::abssub(const BigInt & left, const BigInt & right) const
{
    BigInt answer = left;
    char * ansdata = answer.data;
    const char * rdata = right.data;
    char additive = 0;
    int i;

    for (i = 0; i < right.size; ++i) {
        char s = ansdata[i] - rdata[i] + additive;
        ansdata[i] = (s + 10) % 10;
        additive = (s - 9) / 10;
    }

    while (additive) {
        char s = ansdata[i] + additive;
        ansdata[i] = (s + 10) % 10;
        additive = (s - 9) / 10;
        i++;
    }

    for (; ansdata[answer.size - 1] == 0; answer.size--);
    return answer;
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
