#include <iostream>
#include <algorithm>
#include <string>
#include <limits>

class BigInt {
private:
    static const size_t BASE = 10; // numeral system base for easy output
    static const size_t START_SIZE = 20; // fits 64-bit numbers in BASE

    char *data;
    size_t size;
    size_t alloc;

    int sign;
public:
    void clear() {
        for (size_t i = size; i < alloc; ++i) {
            data[i] = 0;
        }
    }

    bool is_zero() const {
        return (size == 1 && data[0] == 0);
    }

    void check_zero() {
        if (size == 0) {
            size = 1;
            data[0] = 0;
        }
        if (this->is_zero()) {
            sign = 1;
        }
    }

    void init(uint64_t num) {
        size = 0;
        while (num) {
            data[size] = num % BASE;
            num /= BASE;
            ++size;
        }
        this->check_zero();
    }

    BigInt () : data(new char[START_SIZE]), size(0), alloc(START_SIZE), sign(1) {
        this->clear();
        size = 1;
    }

    BigInt (uint64_t num) : data(new char[START_SIZE]), size(0), alloc(START_SIZE), sign(1) {
        this->clear();
        this->init(num);
    }

    BigInt (int64_t num) : data(new char[START_SIZE]), size(0), alloc(START_SIZE) {
        this->clear();
        sign = (num >= 0 ? 1 : -1);
        if (sign == 1) {
            this->init(num);
        } else {
            this->init(-num);
        }
    }

    BigInt (uint32_t num) : data(new char[START_SIZE]), size(0), alloc(START_SIZE), sign(1) {
        this->clear();
        this->init(num);
    }

    BigInt (int32_t num) : data(new char[START_SIZE]), size(0), alloc(START_SIZE) {
        this->clear();
        sign = (num >= 0 ? 1 : -1);
        if (sign == 1) {
            this->init(num);
        } else {
            this->init(-num);
        }
    }

    BigInt (const BigInt &other) : data(new char[other.size * 2]), size(other.size), 
                                   alloc(other.size * 2), sign(other.sign) {
        this->clear();
        std::copy(other.data, other.data + size, data);
    }

    BigInt (BigInt &&other) : data(other.data), size(other.size), 
                                   alloc(other.alloc), sign(other.sign) {
        this->clear();
        other.data = nullptr;
    }

    ~BigInt() {
        if (data != nullptr) {
            delete[] data;
        }
    }

    BigInt& operator=(const BigInt &other) {
        sign = other.sign;

        if (data != nullptr) {
            delete[] data;
        }
        size = other.size;
        alloc = size * 2;
        data = new char[alloc];
        std::copy(other.data, other.data + size, data);
        this->clear();

        return *this;
    }

    BigInt& operator=(BigInt &&other) {
        sign = other.sign;
        if (data != nullptr) {
            delete[] data;
        }
        data = other.data;
        other.data = nullptr;
        size = other.size;
        alloc = other.alloc;

        return *this;
    }

    void trunc() {
        while (size > 1 && data[size - 1] == 0) {
            --size;
        }
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool operator==(const BigInt& other) const {
        if (sign != other.sign || size != other.size) {
            return false;
        }
        for (size_t i = 0; i < size; ++i) {
            if (data[i] != other.data[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const BigInt& other) const {
        return !(*this == other);
    }

    bool operator<(const BigInt& other) const {
        if (sign != other.sign) {
            return sign == -1 ? true : false;
        }
        if (size != other.size) {
            return sign == 1 ? (size < other.size) : (size > other.size);
        }

        if (sign == -1) {
            return (-(*this) > -other);
        }

        for (int i = size - 1; i >= 0; --i) {
            if (data[i] < other.data[i]) {
                return true;
            } else if (data[i] > other.data[i]) {
                return false;
            }
        }
        return false;
    }

    bool operator>=(const BigInt& other) const {
        return !(*this < other);
    }

    bool operator>(const BigInt& other) const {
        if (sign != other.sign) {
            return sign == 1 ? true : false;
        }
        if (size != other.size) {
            return sign == 1 ? (size > other.size) : (size < other.size);
        }

        if (sign == -1) {
            return (-(*this) < -other);
        }

        for (int i = size - 1; i >= 0; --i) {
            if (data[i] > other.data[i]) {
                return true;
            } else if (data[i] < other.data[i]) {
                return false;
            }
        }
        return false;
    }

    bool operator<=(const BigInt& other) const {
        return !(*this > other);
    }

//////////////////////////////////////////////////////////////////////////////////////////////////////////

    BigInt operator-() const {
        BigInt res(*this);
        res.sign = -res.sign;
        res.check_zero();
        return res;
    }

    BigInt operator+(const BigInt& other) const {
        if (other.is_zero()) {
            return (*this);
        }
        if (sign != other.sign) {
            return (*this - (-other));
        }
        char cur = 0;
        BigInt res = *this;

        if (size < other.size) {
            res.size = other.size;
        }
        if (res.size >= res.alloc) {
            res.alloc = 2 * res.size;
            char* tmp = new char[res.alloc];
            std::copy(res.data, res.data + res.size, tmp);
            delete[] res.data;
            res.data = tmp;
            res.clear();
        }

        size_t i;
        for (i = 0; i < other.size || cur; ++i) {
            if (i == res.size) {
                res.size += 1;
            }
            if (i < other.size) {
                cur += other.data[i];
            }
            cur += res.data[i];
            if (cur >= BASE) {
                res.data[i] = cur - BASE;
                cur = 1;
            } else {
                res.data[i] = cur;
                cur = 0;
            }
        }

        return res;
    }

    BigInt operator-(const BigInt& other) const {
        if (other.is_zero()) {
            return (*this);
        }
        if (sign != other.sign) {
            return (*this + (-other));
        }

        if ((sign == 1 && *this < other) ||
            (sign == -1 && *this > other)) {
            return -(other - *this);
        }

        char cur = 0;
        BigInt res = *this;

        for (size_t i = 0; i < other.size || cur; ++i) {
            if (i == res.size) {
                res.size += 1;
            }
            cur += res.data[i];
            if (i < other.size) {
                cur -= other.data[i];
            }
            if (cur < 0) {
                res.data[i] = cur + BASE;
                cur = -1;
            } else {
                res.data[i] = cur;
                cur = 0;
            }
        }

        res.trunc();
        res.check_zero();
        return res;
    }

    friend std::ostream& operator<<(std::ostream& out, const BigInt& value);
};

std::ostream& operator<<(std::ostream& out, const BigInt& value)
{
    if (value.sign == -1) {
        out << '-';
    }
    for (int i = value.size - 1; i >= 0; --i) {
        out << static_cast<char>('0' + value.data[i]);
    }
    return out;
}

