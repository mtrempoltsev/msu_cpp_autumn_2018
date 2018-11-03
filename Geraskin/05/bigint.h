#pragma once

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <iomanip>

class Vector {
private:
    size_t Allocated;
    size_t Used;
    int* Data;
    static constexpr size_t DefaultAllocated = 8;

public:
    Vector()
        : Allocated(DefaultAllocated)
        , Used(0)
        , Data(new int[Allocated])
    {}

    Vector(const Vector& other)
        : Allocated(other.Allocated)
        , Used(other.Used)
        , Data(new int[Allocated])
    {
        std::copy(other.cbegin(), other.cend(), begin());
    }

    ~Vector() {
        delete[] Data;
    }

    void reserve(size_t size) {
        if (Allocated < size) {
            Allocated = size;
            auto ptr = new int[Allocated];
            std::copy(begin(), end(), ptr);
            delete[] Data;
            Data = ptr;
        }
    }

    void push_back(int value) {
        if (Used == Allocated) {
            Allocated *= 2;
            auto ptr = new int[Allocated];
            std::copy(begin(), end(), ptr);
            delete[] Data;
            Data = ptr;
        }
        Data[Used++] = value;
    }

    int operator[](size_t idx) const {
        if (idx < Used) {
            return Data[idx];
        } else {
            throw std::out_of_range("Bad vector index!");
        }
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            reserve(other.Allocated);
            Used = other.Used;
            std::copy(other.cbegin(), other.cend(), begin());
        }
        return *this;
    }

    bool operator<(const Vector& other) const {
        if (Used > other.Used) {
            return false;
        }
        if (Used < other.Used) {
            return true;
        }

        for (size_t i = Used; i > 0; --i) {
            if (Data[i - 1] < other.Data[i - 1]) {
                return true;
            }
            if (Data[i - 1] > other.Data[i - 1]) {
                return false;
            }
        }
        return false;
    }

    bool operator==(const Vector& other) const {
        if (Used != other.Used) {
            return false;
        }
        return std::equal(cbegin(), cend(), other.cbegin());
    }

    void pop_back() {
        if (Used > 0) {
            --Used;
        }
    }

    size_t size() const {
        return Used;
    }

    int* begin() {
        return Data;
    }

    int* end() {
        return Data + Used;
    }

    const int* cbegin() const {
        return Data;
    }

    const int* cend() const {
        return Data + Used;
    }

    int back() const {
        return Data[Used - 1];
    }
};

class BigInt {
private:
    enum ESign {
        Positive,
        Negative
    };

    static constexpr int Base = 1e9;
    Vector Data;
    ESign Sign;

    int GetDigit(int idx) const {
        return idx < Data.size() ? Data[idx] : 0;
    }

public:
    BigInt(int64_t number = 0)
        : Sign(number >= 0 ? Positive : Negative)
    {
        if (number < 0) {
            number = -number;
        }
        while(number) {
            Data.push_back(number % Base);
            number /= Base;
        }
    }

    BigInt(const BigInt& other)
        : Data(other.Data)
        , Sign(other.Sign)
    {}

    BigInt& operator=(const BigInt& other) {
        if (this != &other) {
            Data = other.Data;
            Sign = other.Sign;
        }
        return *this;
    }

    bool operator<(const BigInt& other) const {
        bool negative = Sign == Negative;
        if (Sign != other.Sign) {
            return negative;
        }
        return negative ? other.Data < Data : Data < other.Data;
    }

    bool operator==(const BigInt& other) const {
        if (Sign != other.Sign) {
            return false;
        }
        return Data == other.Data;
    }

    bool operator!=(const BigInt& other) const {
        return !(*this == other);
    }

    bool operator<=(const BigInt& other) const {
        return !(*this > other);
    }

    bool operator>=(const BigInt& other) const {
        return !(*this < other);
    }

    bool operator>(const BigInt& other) const {
        return !(*this < other) && !(*this == other);
    }

    BigInt abs() const {
        auto result = *this;
        result.Sign = Positive;
        return result;
    }

    BigInt operator+(const BigInt& other) const {
        BigInt result;
        if (Sign == other.Sign) {
            result.Sign = Sign;
            auto longest = std::max(Data.size(), other.Data.size());
            int carry = 0;
            for (size_t i = 0; i < longest || carry; ++i) {
                auto cur = carry + GetDigit(i) + other.GetDigit(i);
                result.Data.push_back(cur % Base);
                carry = cur / Base;
            }
        } else if (this->abs() >= other.abs()) {
            result.Sign = Sign;
            auto longest = std::max(Data.size(), other.Data.size());
            int take = 0;
            for (size_t i = 0; i < longest; ++i) {
                auto cur = GetDigit(i) - other.GetDigit(i) - take;
                if (cur < 0) {
                    cur += Base;
                    take = 1;
                } else {
                    take = 0;
                }
                result.Data.push_back(cur);
            }
        } else {
            return other + *this;
        }
        while (result.Data.size() > 0 && result.Data.back() == 0) {
            result.Data.pop_back();
        }
        if (result.Data.size() == 0) {
            result.Sign = Positive;
        }
        return result;
    }

    BigInt operator-() const {
        if (*this == 0) {
            return *this;
        }
        auto result = *this;
        result.Sign = Sign == Positive ? Negative : Positive;
        return result;
    }

    BigInt operator-(const BigInt& other) const {
        return *this + (-other);
    }

    friend std::ostream& operator<<(std::ostream& out, const BigInt& obj) {
        if (obj.Data.size() == 0) {
            out << 0;
            return out;
        }
        if (obj.Sign == Negative) {
            out << '-';
        }
        out << obj.Data.back();
        for (size_t i = obj.Data.size() - 1; i > 0; --i) {
            out << std::setw(9) << std::setfill('0') << obj.Data[i - 1];
        }
        return out;
    }
};
