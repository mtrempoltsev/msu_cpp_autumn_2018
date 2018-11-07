#ifndef MYBIGINT_H
#define MYBIGINT_H
#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <type_traits>

template<class T = int>
class Vector
{
        static_assert(std::is_integral<T>::value, "class T should be an integral type");
        T* ptr;
        ssize_t allocd;
        ssize_t used;
        static constexpr ssize_t baselen{10};
        static constexpr ssize_t multipl{2};
    public:
        explicit Vector(ssize_t len = baselen): ptr{new T[len]}, allocd{len}, used{0} {}
        Vector(const Vector& that): ptr{new T[that.allocd]}, allocd{that.allocd}, used{that.used}
        {
            std::copy(that.ptr, that.ptr + that.used, ptr);
        }
        Vector(Vector&& that) noexcept: ptr{that.ptr}, allocd{that.allocd}, used{that.used}
        {
            that.ptr = nullptr;
        }
        Vector& operator=(const Vector&);
        Vector& operator=(Vector&&) noexcept;
        ~Vector()
        {
            delete[] ptr;
        }
        T operator[](ssize_t) const;
        void reserve(ssize_t);
        void push_back(T);
        void pop_back();
        T give_back() const;
        T back() const;
        ssize_t size() const;
};

class BigInt
{
        using Int = int;
        Vector<Int> repr;
        static constexpr Int base = 1'000'000'000;
        static constexpr Int baselen = 9;
        enum {
            positive = 1,
            negative = -1,
        };
        int sign;
        void normalize();
        int le_eq_gr(const BigInt&) const;
    public:
        BigInt(long long = 0, bool = false);
        BigInt operator-() const;
        BigInt operator+(const BigInt&) const;
        BigInt operator-(const BigInt&) const;
        bool operator<(const BigInt&) const;
        bool operator>(const BigInt&) const;
        bool operator<=(const BigInt&) const;
        bool operator>=(const BigInt&) const;
        bool operator==(const BigInt&) const;
        bool operator!=(const BigInt&) const;
        ssize_t size() const;
        friend std::ostream& operator<<(std::ostream&, const BigInt&);
};

template<class T>
Vector<T>& Vector<T>::operator=(const Vector& that)
{
    if (this == &that) {
        return *this;
    }
    delete[] ptr;
    allocd = that.allocd;
    used = that.used;
    ptr = new T[allocd];
    std::copy(that.ptr, that.ptr + that.used, ptr);
    return *this;
};

template<class T>
Vector<T>& Vector<T>::operator=(Vector&& that) noexcept
{
    if (this == &that) {
        return *this;
    }
    delete[] ptr;
    allocd = that.allocd;
    used = that.used;
    ptr = that.ptr;
    that.ptr = nullptr;
    return *this;
}

template<class T>
T Vector<T>::operator[](ssize_t ind) const
{
    if (ind < used) {
        return ptr[ind];
    }
    throw std::out_of_range("Index out of range");
}

template<class T>
void Vector<T>::reserve(ssize_t len)
{
    if (len <= allocd) {
        return;
    }
    allocd = len;
    T* newptr{new T[allocd]};
    std::copy(ptr, ptr + used, newptr);
    delete[] ptr;
    ptr = newptr;
}

template<class T>
void Vector<T>::push_back(T value)
{
    if (used >= allocd) {
        reserve(allocd * multipl);
    }
    ptr[used++] = value;
}

template<class T>
void Vector<T>::pop_back()
{
    if (used > 0) {
        --used;
    } else {
        throw std::out_of_range("Attempt to decrement used to negative value");
    }
}

template<class T>
T Vector<T>::give_back() const
{
    if (used > 0) {
        return ptr[used - 1];
    }
    throw std::out_of_range("Attempt to get an element from an empty vector");
}

template<class T>
T Vector<T>::back() const
{
    if (used > 0) {
        return ptr[used - 1];
    }
    throw std::out_of_range("Attempt to get value from an empty vector");
}

template<class T>
ssize_t Vector<T>::size() const
{
    return used;
}

void BigInt::normalize()
{
    while (repr.size() > 1 && repr.give_back() == 0) {
        repr.pop_back();
    }
    if (repr.give_back() == 0) {
        sign = positive;
    }
}

int BigInt::le_eq_gr(const BigInt& that) const
{
    ssize_t n = this->size();
    ssize_t m = that.size();
    if (n == 1 && m == 1 && this->repr[0] == 0 && that.repr[0] == 0) {
        return 0;
    }
    if (this->sign != that.sign) {
        return this->sign;
    }
    if (n != m) {
        return (n - m) * this->sign;
    }
    for (ssize_t i = n - 1; i >= 0; --i) {
        if (this->repr[i] != that.repr[i]) {
            return (this->repr[i] - that.repr[i]) * this->sign;
        }
    }
    return 0;
}

BigInt::BigInt(long long val, bool empty): repr{}, sign{positive}
{
    if (empty) {
        return;
    }
    if (val < 0) {
        sign = negative;
        val = -val;
    }
    do {
        repr.push_back(val % base);
        val /= base;
    } while (val > 0);
}

BigInt BigInt::operator-() const
{
    BigInt result{*this};
    result.sign = -sign;
    result.normalize();
    return result;
}

BigInt BigInt::operator+(const BigInt& that) const
{
    if (this->sign == that.sign) {
        ssize_t n = this->size();
        ssize_t m = that.size();
        ssize_t mi = std::min(n, m);
        int carry = 0;
        BigInt result{0, true};
        result.sign = this->sign;
        ssize_t i = 0;
        for (; i < mi; ++i) {
            Int sum = this->repr[i] + that.repr[i] + carry;
            if (sum >= base) {
                carry = 1;
                sum -= base;
            } else {
                carry = 0;
            }
            result.repr.push_back(sum);
        }
        if (n != m) {
            const BigInt& longer = (n > m) ? *this : that;
            ssize_t ma = std::max(n, m);
            for (; i < ma; ++i) {
                Int sum = longer.repr[i] + carry;
                if (sum >= base) {
                    carry = 1;
                    sum -= base;
                } else {
                    carry = 0;
                }
                result.repr.push_back(sum);
            }
        }
        result.normalize();
        return result;
    }
    if (this->sign == positive) {
        return *this - -that;
    }
    return -(-*this - that);
}

BigInt BigInt::operator-(const BigInt& that) const
{
    if (that == *this) {
        return 0;
    }
    if (this->sign == that.sign) {
        if (this->sign == negative) {
            return (-that) - (-*this);
        }
        BigInt result{0, true};
        int cmpres = le_eq_gr(that);
        const BigInt& minuend = (cmpres > 0) ? *this : that;
        const BigInt& subtra = (cmpres > 0) ? that : *this;
        ssize_t n = minuend.size();
        ssize_t m = subtra.size();
        ssize_t mi = std::min(n, m);
        if (cmpres < 0) {
            result.sign = negative;
        }
        int carry = 0;
        ssize_t i = 0;
        for (; i < mi; ++i) {
            Int diff = minuend.repr[i] - subtra.repr[i] - carry;
            if (diff < 0) {
                carry = 1;
                diff += base;
            } else {
                carry = 0;
            }
            result.repr.push_back(diff);
        }
        if (n > m) {
            for (; i < n; ++i) {
                Int diff = minuend.repr[i] - carry;
                if (diff < 0) {
                    carry = 1;
                    diff += base;
                } else {
                    carry = 0;
                }
                result.repr.push_back(diff);
            }
        }
        result.normalize();
        return result;
    }
    return *this + -that;
}

bool BigInt::operator<(const BigInt& that) const
{
    return le_eq_gr(that) < 0;
}
bool BigInt::operator>(const BigInt& that) const
{
    return le_eq_gr(that) > 0;
}
bool BigInt::operator<=(const BigInt& that) const
{
    return le_eq_gr(that) <= 0;
}
bool BigInt::operator>=(const BigInt& that) const
{
    return le_eq_gr(that) >= 0;
}
bool BigInt::operator==(const BigInt& that) const
{
    return le_eq_gr(that) == 0;
}
bool BigInt::operator!=(const BigInt& that) const
{
    return le_eq_gr(that) != 0;
}

ssize_t BigInt::size() const
{
    return repr.size();
}

std::ostream& operator<<(std::ostream& out, const BigInt& num)
{
    if (num.size() == 0) {
        return out;
    }
    if (num.sign == BigInt::negative) {
        out << '-';
    }
    out << num.repr.back();
    if (num.size() >= 2) {
        for (ssize_t i = num.size() - 2; i >= 0; --i) {
            out << std::setfill('0') << std::setw(BigInt::baselen) << num.repr[i];
        }
    }

    return out;
}
#endif
