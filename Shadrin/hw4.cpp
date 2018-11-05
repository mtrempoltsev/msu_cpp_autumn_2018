#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <utility>
#include <cstring>

class BigInt {

    char *data;

    std::size_t data_size;

    bool is_negative;

public:

    BigInt();

    BigInt(long long &&a);

    BigInt(char *, std::size_t, bool);

    BigInt(BigInt &&);

    BigInt(const BigInt &);

    BigInt &operator =(const BigInt &);

    friend char cmp(const BigInt &, const BigInt &);

    friend bool operator ==(const BigInt &, const BigInt &);

    friend bool operator !=(const BigInt &, const BigInt &);

    friend bool operator >(const BigInt &, const BigInt &);

    friend bool operator >=(const BigInt &, const BigInt &);

    friend bool operator <(const BigInt &, const BigInt &);

    friend bool operator <=(const BigInt &, const BigInt &);

    friend std::ostream &operator <<(std::ostream &, const BigInt &);

    friend BigInt operator +(const BigInt &, const BigInt &);

    friend BigInt operator -(const BigInt &, const BigInt &);

    friend BigInt operator -(const BigInt &);

    ~BigInt();

};

BigInt::BigInt()
    : data(new char[1])
    , data_size(1)
    , is_negative(false)
{
    data[0] = 0;
}

BigInt::BigInt(long long &&a)
    : data(nullptr)
    , data_size(0)
    , is_negative(a < 0)
{
    std::string t = std::to_string(a < 0 ? -a : a);

    data_size = t.size();
    data = new char[data_size];
    for (std::size_t i = 0; i < data_size; ++i) {
        data[i] = t[data_size - i - 1] - '0';
    }
}

BigInt::BigInt(BigInt &&a)
    : data(a.data)
    , data_size(a.data_size)
    , is_negative(a.is_negative)
{
    a.data = nullptr;
    a.data_size = 0;
}

BigInt::BigInt(const BigInt &anoth)
    : data(nullptr)
    , data_size(anoth.data_size)
    , is_negative(anoth.is_negative)
{
    data = new char[data_size];
    std::memcpy(data, anoth.data, data_size);
}

BigInt &BigInt::operator =(const BigInt &anoth) {
    if (data == anoth.data) {
        return *this;
    }
    delete[] data;
    data_size = anoth.data_size;
    data = new char[data_size];
    std::memcpy(data, anoth.data, data_size);
    is_negative = anoth.is_negative;
    return *this;
}

char cmp(const BigInt &a, const BigInt &b) {
    if (a.is_negative != b.is_negative) {
        if (a.data_size == 1 &&
                b.data_size == 1 &&
                a.data[0] == 0 &&
                b.data[0] == 0) {
            return 0;
        }
        return a.is_negative ? -1 : 1;
    }
    int is_both_negative = 1;
    if (a.is_negative) {
        is_both_negative = -1;
    }
    if (a.data_size != b.data_size) {
        return (a.data_size > b.data_size ? 1 : -1) * is_both_negative;
    }
    for (int i = static_cast<int>(a.data_size - 1); i >= 0; --i) {
        if (a.data[i] != b.data[i]) {
            return (a.data[i] - b.data[i]) * is_both_negative;
        }
    }
    return 0;
}

bool operator ==(const BigInt &a, const BigInt &b) {
    return cmp(a, b) == 0;
}

bool operator !=(const BigInt &a, const BigInt &b) {
    return cmp(a, b) != 0;
}

bool operator >(const BigInt &a, const BigInt &b) {
    return cmp(a, b) > 0;
}

bool operator >=(const BigInt &a, const BigInt &b) {
    return cmp(a, b) >= 0;
}

bool operator <(const BigInt &a, const BigInt &b) {
    return cmp(a, b) < 0;
}

bool operator <=(const BigInt &a, const BigInt &b) {
    return cmp(a, b) <= 0;
}

BigInt::BigInt(char *data_, std::size_t data_size_, bool is_negative_)
    : data(data_)
    , data_size(data_size_)
    , is_negative(is_negative_)
{ }

std::ostream &operator <<(std::ostream &out, const BigInt &bi) {
    if (bi.is_negative) {
        out << '-';
    }
    for (int i = bi.data_size - 1; i >= 0; --i) {
        out << char(bi.data[i] + '0');
    }
    return out;
}

BigInt operator +(const BigInt &a, const BigInt &b) {
    if (a.data_size < b.data_size) {
        return b + a;
    }

    if (b.is_negative && !a.is_negative) {
        return a - (-b);
    } else if (a.is_negative && !b.is_negative) {
        return b - (-a);
    }

    std::size_t new_data_size = a.data_size + 1;
    char *new_data = new char[new_data_size];

    int sum, cur = 0;
    for (std::size_t i = 0; i < b.data_size; ++i) {
        sum = cur + a.data[i] + b.data[i];
        new_data[i] = sum % 10;
        cur = sum / 10;
    }

    for (std::size_t i = b.data_size; i < a.data_size; ++i) {
        sum = cur + a.data[i];
        new_data[i] = sum % 10;
        cur = sum / 10;
    }

    if (cur) {
        new_data[a.data_size] = cur;
    } else {
        --new_data_size;
    }

    return BigInt(new_data, new_data_size, a.is_negative);
}

BigInt operator -(const BigInt &a, const BigInt &b) {

    if (!a.is_negative && !b.is_negative) {
        if (a < b) {
            return -(b - a);
        }
    } else if (!a.is_negative && b.is_negative) {
        return a + (-b);
    } else if (a.is_negative && !b.is_negative) {
        return -((-a) + b);
    } else if (a.is_negative && b.is_negative) {
        return (-b) - (-a);
    }

    std::size_t new_data_size = a.data_size;
    char *new_data = new char[new_data_size];

    int sum, cur = 0;
    for (std::size_t i = 0; i < b.data_size; ++i) {
        sum = cur + a.data[i] - b.data[i];
        if (sum < 0) {
            new_data[i] = 10 + sum;
            cur = -1;
        } else {
            cur = 0;
            new_data[i] = sum;
        }
    }

    for (std::size_t i = b.data_size; i < a.data_size; ++i) {
        sum = cur + a.data[i];
        if (sum < 0) {
            new_data[i] = 10 + sum;
            cur = -1;
        } else {
            cur = 0;
            new_data[i] = sum;
        }
    }

    for (int i = new_data_size - 1; new_data[i] == 0 &&
            new_data_size > 1; --i) {
        --new_data_size;
    }

    return BigInt(new_data, new_data_size, false);
}

BigInt operator -(const BigInt &a) {
    char *dup = new char[a.data_size];
    std::memcpy(dup, a.data, a.data_size);
    return BigInt(dup, a.data_size, true ^ a.is_negative);
}

BigInt::~BigInt() {
    delete[] data;
}

int main() {
    std::cout << (BigInt(3) - BigInt(16)) << std::endl;
}
