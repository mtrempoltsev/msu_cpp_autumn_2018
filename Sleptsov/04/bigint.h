#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <algorithm>

constexpr int64_t BASE = 1e16;

class BigInt {
    int64_t *data;
    size_t size;
    bool is_negative;
public:
    BigInt(int64_t val = 0): is_negative(val < 0)
    {
        val = std::abs(val);
        size = 1 + (val >= BASE ? 1 : 0);
        data = new int64_t[size];
        for (size_t i = 0; i < size; i++) {
            data[i] = val % BASE;
            val /= BASE;
        }
    }

    BigInt(const BigInt& x): size(x.size), is_negative(x.is_negative)
    {
        data = new int64_t[x.size];
        std::copy(x.data, x.data + x.size, data);
    }

    BigInt(BigInt&& x): data(x.data), size(x.size), is_negative(x.is_negative)
    {
        x.data = nullptr;
        x.size = 0;
    }

    BigInt& operator=(const BigInt& x)
    {
        if (this == &x) {
            return *this;
        }

        int64_t *temp = new int64_t[x.size];
        delete[] data;
        data = temp;
        size = x.size;
        is_negative = x.is_negative;
        std::copy(x.data, x.data + x.size, data);
        return *this;
    }

    bool operator==(const BigInt& x) const
    {
        if (this->size == 1 && x.size == 1 && this->data[0] == 0 && x.data[0] == 0) {
            return true;
        }
        
        return this->size == x.size && this->is_negative == x.is_negative 
                && !std::memcmp(this->data, x.data, x.size * sizeof(int64_t));
    }

    bool operator!=(const BigInt& x) const
    {
        return !(*this == x);
    }

    bool operator<(const BigInt& x) const
    {
        if (this->is_negative != x.is_negative) {
            return this->is_negative;
        }

        if (this->size != x.size) {
            return (this->size < x.size ? !this->is_negative : this->is_negative);
        }

        for (int64_t i = x.size - 1; i >= 0; i--) {
            if (this->data[i] < x.data[i]) {
                return !this->is_negative;
            } else if (this->data[i] > x.data[i]) {
                return this->is_negative;
            }
        }

        return false;
    }

    bool operator>=(const BigInt& x) const
    {
        return !(*this < x);
    }

    bool operator<=(const BigInt& x) const
    {
        return *this < x || *this == x;
    }

    bool operator>(const BigInt& x) const
    {
        return !(*this <= x);
    }

    BigInt operator-() const
    {
        BigInt temp = *this;
        temp.is_negative = !temp.is_negative;
        return temp;
    }

    BigInt operator+(const BigInt& x) const
    {
        if (x.size > this->size) {
            return x + *this;
        }

        if (!this->is_negative && x.is_negative) {
            return x - (-(*this));
        } else if (this->is_negative && !x.is_negative) {
            return *this - (-x);
        }

        int64_t high = this->data[this->size - 1] + 1;
        if (x.size == this->size) {
            high += x.data[x.size - 1];
        }
        size_t new_size = this->size + (high >= BASE ? 1 : 0);
        
        BigInt ans;
        int64_t *temp = new int64_t[new_size];
        delete[] ans.data;
        ans.data = temp;
        ans.size = new_size;
        ans.is_negative = this->is_negative;

        int64_t carry = 0;
        for (size_t i = 0; i < new_size; i++) {
            ans.data[i] = (i < this->size ? this->data[i] : 0) + (i < x.size ? x.data[i] : 0) + carry;
            carry = ans.data[i] >= BASE;
            if (carry) {
                ans.data[i] -= BASE;
            }
        }
        return ans;
    }

    BigInt operator-(const BigInt& x) const
    {
        if (!this->is_negative && !x.is_negative) {
            if (*this < x) {
                return -(x - *this);
            }
        } else if (!this->is_negative && x.is_negative) {
            return *this + (-x);
        } else if (this->is_negative && !x.is_negative) {
            return -(-(*this) + x);
        } else if (this->is_negative && x.is_negative) {
            return (-x) - (-(*this));
        }

        size_t new_size = this->size;
        BigInt ans;
        int64_t *temp = new int64_t[new_size];
        delete[] ans.data;
        ans.data = temp;
        ans.is_negative = false;

        int64_t carry = 0;
        for (size_t i = 0; i < new_size; i++) {
            ans.data[i] = this->data[i] - (i < x.size ? x.data[i] : 0) - carry;
            carry = ans.data[i] < 0;
            if (carry) {
                ans.data[i] += BASE;
            }
        }

        while (!ans.data[new_size - 1] && new_size > 1) {
            new_size--;
        }
        ans.size = new_size;

        return ans;
    }
    
    friend std::ostream& operator<<(std::ostream& out, const BigInt& x);

    ~BigInt()
    {
        delete[] data;
    }
};

std::ostream& operator<<(std::ostream& out, const BigInt& x)
{
    size_t digits = 0;
    int64_t temp = BASE;
    while (temp) {
        temp /= 10;
        digits++;
    }
    int64_t sign = x.is_negative ? -1 : 1;

    size_t i = x.size - 1;
    while (!x.data[i] && i > 0) {
        i--;
    }
    out << sign * x.data[i];
    for (size_t j = i; j > 0; j--) {
        out << std::setfill('0') << std::setw(digits - 1) << x.data[j - 1];
    }

    return out;
}
