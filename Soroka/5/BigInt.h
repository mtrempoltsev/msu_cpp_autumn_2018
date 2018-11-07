#include <algorithm>
#include <iostream>
#include <iomanip>

class BigInt {
private:

    class Memory{
    private:
        size_t Allocated;
        size_t Used;
        static constexpr size_t DefaultSize = 4;
        int* Data;
    public:
        Memory()
        {
            Allocated = DefaultSize;
            Used = 0;
            Data = new int[Allocated];
        }
        Memory(const Memory& data)
        {
            Allocated = data.Allocated;
            Used = data.Used;
            Data = new int[Allocated];
            std::copy(data.cbegin(), data.cend(), begin());
        }
        ~Memory()
        {
            delete[] Data;
        }

        void allocate(size_t size, bool copy = true)
        {
            if (Allocated < size) {
                Allocated = size;
                auto tmp_ptr = new int[Allocated];
                if (copy) {
                    std::copy(begin(), end(), tmp_ptr);
                }
                delete[] Data;
                Data = tmp_ptr;
            }
        }


        void push_back(int value)
        {
            if (Used == Allocated) {
                allocate(Allocated * 2);
            }
            Data[Used++] = value;
        }

        void pop_back()
        {
            if (Used > 0) {
                --Used;
                if(Used < Allocated / 4) {
                    allocate((size_t) (Allocated / 2));
                }
            } else {
                throw std::length_error("pop_back from empty memory");
            }
        }

        bool operator<(const Memory& data) const
        {
            if (Used > data.Used) {
                return false;
            }
            if (Used <data.Used) {
                return true;
            }

            for (size_t i = Used; i > 0; --i) {
                if (Data[i - 1] < data.Data[i - 1]) {
                    return true;
                }
                if (Data[i - 1] > data.Data[i - 1]) {
                    return false;
                }
            }
            return false;
        }

        Memory& operator=(const Memory& data)
        {
            if (this != &data) {
                allocate(data.Allocated, false);
                Used = data.Used;
                std::copy(data.cbegin(), data.cend(), begin());
            }
            return *this;
        }

        bool operator==(const Memory& data) const
        {
            if (Used != data.Used) {
                return false;
            }
            return std::equal(cbegin(), cend(), data.cbegin());
        }

        int operator[](size_t idx) const
        {
            if (idx < Used) {
                return Data[idx];
            }
            throw std::out_of_range("Bad memory index");
        }

        size_t size() const
        {
            return Used;
        }

        int* begin()
        {
            return Data;
        }

        const int* cbegin() const
        {
            return Data;
        }

        int* end()
        {
            return Data + Used;
        }

        const int* cend() const
        {
            return Data + Used;
        }

        int back() const
        {
            if (Used > 0) {
                return Data[Used - 1];
            }
            throw std::length_error("Empty memory");
        }
    };

    enum Sign {
        Positive,
        Negative
    };

    static constexpr int Base = 1e9;
    Memory Data;
    Sign sign;

    int GetDigit(int idx) const {
        return idx < Data.size() ? Data[idx] : 0;
    }

public:
    BigInt(int64_t number = 0)
    {
        sign = number >= 0 ? Positive : Negative;
        if (number < 0) {
            number = -number;
        }
        while(number) {
            Data.push_back(number % Base);
            number /= Base;
        }
    }

    BigInt(const BigInt& data)
    {
        Data = data.Data;
        sign = data.sign;
    }

    BigInt& operator=(const BigInt& data)
    {
        if (this != &data) {
            Data = data.Data;
            sign = data.sign;
        }
        return *this;
    }

    bool operator<(const BigInt& data) const
    {
        bool negative = sign == Negative;
        if (sign != data.sign) {
            return negative;
        }
        return negative ? data.Data < Data : Data < data.Data;
    }

    bool operator>(const BigInt& data) const
    {
        return !(*this < data) && !(*this == data);
    }

    bool operator<=(const BigInt& data) const
    {
        return !(*this > data);
    }

    bool operator>=(const BigInt& data) const
    {
        return !(*this < data);
    }

    bool operator==(const BigInt& data) const
    {
        if (sign != data.sign) {
            return false;
        }
        return Data == data.Data;
    }

    bool operator!=(const BigInt& data) const
    {
        return !(*this == data);
    }

    BigInt abs() const
    {
        auto result = *this;
        result.sign = Positive;
        return result;
    }

    BigInt operator+(const BigInt& data) const
    {
        BigInt result;
        if (sign == data.sign) {
            result.sign = sign;
            auto longest = std::max(Data.size(), data.Data.size());
            int carry = 0;
            for (size_t i = 0; i < longest || carry; ++i) {
                auto cur = carry + GetDigit(i) + data.GetDigit(i);
                result.Data.push_back(cur % Base);
                carry = cur / Base;
            }
        } else if (this->abs() >= data.abs()) {
            result.sign = sign;
            auto longest = std::max(Data.size(), data.Data.size());
            int take = 0;
            for (size_t i = 0; i < longest; ++i) {
                auto cur = GetDigit(i) - data.GetDigit(i) - take;
                if (cur < 0) {
                    cur += Base;
                    take = 1;
                } else {
                    take = 0;
                }
                result.Data.push_back(cur);
            }
        } else {
            return data + *this;
        }
        while (result.Data.size() > 0 && result.Data.back() == 0) {
            result.Data.pop_back();
        }
        if (result.Data.size() == 0) {
            result.sign = Positive;
        }
        return result;
    }

    BigInt operator-() const
    {
        if (*this == 0) {
            return *this;
        }
        auto result = *this;
        result.sign = sign == Positive ? Negative : Positive;
        return result;
    }

    BigInt operator-(const BigInt& data) const
    {
        return *this + (-data);
    }

    friend std::ostream& operator<<(std::ostream& out, const BigInt& obj)
    {
        if (obj.Data.size() == 0) {
            out << 0;
            return out;
        }
        if (obj.sign == Negative) {
            out << '-';
        }
        out << obj.Data.back();
        out << std::setw(9) << std::setfill('0');
        for (size_t i = obj.Data.size() - 1; i > 0; --i) {
            out << obj.Data[i - 1];
        }
        return out;
    }
};
