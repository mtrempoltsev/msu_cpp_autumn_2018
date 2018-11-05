#pragma once
#include <iostream>
#include <iomanip>
#include <cstdint>

class BigInt
// любые два числа хранятся в разных местах
// каждый объект монопольно владеет своей памятью
// size - кол-во эл-тов в массиве data
// size - мин. степень двойки, необходимая для хранения числа
{
    uint16_t* data = nullptr;
    size_t size = 0;
    int8_t sign = 1;

public:
    BigInt(int64_t value = 0)
    {
        uint64_t abs, abs_copy;
        if(value >= 0) {
            abs = value;
            sign = 1;
        } else {
            abs = -value;
            sign = -1;
        }
        size_t significant = 0;
        abs_copy = abs;
        do {
            abs_copy >>= 16;
            ++significant;
        } while(abs_copy);
        size_t new_size = calc_size(significant);
        data = new uint16_t[new_size];
        size = new_size;
        for(int i = size - 1; i >= 0; --i) {
            data[i] = abs & 0xFFFF;
            abs >>= 16;
        }
    }

    BigInt(const BigInt& copied)
    {
        uint16_t* ptr = new uint16_t[copied.size];
        size = copied.size;
        for(size_t i = 0; i < size; ++i) {
            ptr[i] = copied.data[i];
        }
        sign = copied.sign;
        if(data != nullptr) {
            delete[] data;
        }
        data = ptr;
    }

    BigInt& operator=(const BigInt& copied)
    {
        if(this == &copied) {
            return *this;
        }
        uint16_t* ptr = new uint16_t[copied.size];
        size = copied.size;
        for(size_t i = 0; i < size; ++i) {
            ptr[i] = copied.data[i];
        }
        sign = copied.sign;
        delete[] data;
        data = ptr;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream&, const BigInt& num);
    friend bool operator==(const BigInt& a, const BigInt& b);
    friend bool operator!=(const BigInt& a, const BigInt& b);
    friend bool operator<(const BigInt& a, const BigInt& b);
    friend bool operator>(const BigInt& a, const BigInt& b);
    friend bool operator<=(const BigInt& a, const BigInt& b);
    friend bool operator>=(const BigInt& a, const BigInt& b);
    friend BigInt operator+(const BigInt& a, const BigInt& b);
    friend BigInt operator-(const BigInt& a, const BigInt& b);

    BigInt operator-() const
    {
        BigInt tmp = *this;
        tmp.sign = -sign;
        return tmp;
    }

private:
    void double_size()
    {
        uint16_t* ptr = new uint16_t[size * 2];
        int i;
        for(i = 0; i < size; ++i) {
            ptr[i] = 0;
        }
        for( ; i < size * 2; ++i) {
            ptr[i] = data[i - size];
        }
        delete[] data;
        data = ptr;
        size *= 2;
    }

    static size_t calc_size(size_t significant_blocks)
    // block = 16 bits
    // size = number of blocks
    {
        size_t size = 1;
        while(size < significant_blocks) {
            size *= 2;
        }
        return size;
    }

    static bool abs_less(const BigInt& a, const BigInt& b)
    {
        if(a.data == b.data) {
            return false;
        } else if(a.size != b.size) {
            return a.size < b.size;
        } else {
            for(int i = 0; i < a.size; i++) {
                if(a.data[i] != b.data[i]) {
                    return a.data[i] < b.data[i];
                }
            }
            return false;
        }
    }

    static uint16_t add_overflow(uint16_t& src, uint16_t num)
    // прибавляет к src слагаемое num
    // и возвращает то, что вышло за пределы uint16_t
    // (т.е. то, что надо прибавить к след.блоку BigInt)
    {
        if(num == 0) {
            return 0;
        }
        uint32_t sum = src + num;
        src = static_cast<uint16_t>(sum & 0xFFFF);
        return static_cast<uint16_t>(sum >> 16);
    }

    static uint16_t sub_overflow(uint16_t& src, uint16_t num)
    // вычитает num из src по модулю uint16_t и возвращает
    // 1, если src - num < 0 (т.е. надо занимать 1 из след.блока BigInt), иначе 0
    {
        if(num == 0) {
            return 0;
        }
        uint32_t sub = (static_cast<uint32_t>(src) ^ 0x00010000) - num;
        src = static_cast<uint16_t>(sub & 0xFFFF);
        return static_cast<uint16_t>(~(sub >> 16) & 1);
    }

    static BigInt abs_add(const BigInt& bigger, const BigInt& smaller, int8_t sign)
    // сумма модулей аргументов, взятая со знаком sign
    // при условии bigger.size >= smaller.size
    {
        BigInt tmp = bigger;
        for(size_t i = 0; i < smaller.size; ++i) {
            uint16_t overflow = smaller.data[i];
            int j;
            for(j = tmp.size - smaller.size + i; j >= 0; --j) {
                overflow = add_overflow(tmp.data[j], overflow);
                if(!overflow) {
                    break;
                }
            }
            if(j < 0 && overflow) {
                tmp.double_size();
                tmp.data[tmp.size / 2 - 1] = overflow;
            }
        }
        tmp.sign = tmp == 0 ? 1 : sign;
        return tmp;
    }

    static BigInt abs_sub(const BigInt& bigger, const BigInt& smaller, int8_t sign)
    // разность модулей аргументов, взятая со знаком sign
    // при условии bigger >= smaller
    {
        BigInt tmp = bigger;
        for(size_t i = 0; i < smaller.size; ++i) {
            uint16_t overflow = smaller.data[i];
            int j;
            for(j = tmp.size - smaller.size + i; j >= 0; --j) {
                overflow = sub_overflow(tmp.data[j], overflow);
                if(!overflow) {
                    break;
                }
            }
        }
        size_t i;
        for(i = 0; i < tmp.size - 1; ++i) {
            if(tmp.data[i] != 0) {
                break;
            }
        }
        tmp.sign = (tmp.data[tmp.size - 1] == 0 && i >= tmp.size - 1) ? 1 : sign;
        // если tmp == 0, то sign = 1
        size_t new_size = calc_size(tmp.size - i);
        if(new_size < tmp.size) {
            uint16_t* ptr = new uint16_t[new_size];
            for(size_t i = 0; i < new_size; ++i) {
                ptr[i] = tmp.data[tmp.size - new_size + i];
            }
            delete[] tmp.data;
            tmp.data = ptr;
            tmp.size = new_size;
        }
        return tmp;
    }
};

bool operator==(const BigInt& a, const BigInt& b)
{
    if(&a == &b) {
        return true;
    } else if(a.size != b.size) {
        return false;
    } else {
        size_t size = a.size;
        for(int i = 0; i < size; ++i) {
            if(a.data[i] != b.data[i]) {
                return false;
            }
        }
        return true;
    }
}

bool operator!=(const BigInt& a, const BigInt& b)
{
    return !(a == b);
}

bool operator<(const BigInt& a, const BigInt& b)
{
    if(&a == &b) {
        return false;
    } else if(a.sign != b.sign) {
        return a.sign < b.sign;
    } else if(a.size != b.size) {
        return (a.size < b.size) == (a.sign == 1);
    } else {
        for(size_t i = 0; i < a.size; i++) {
            if(a.data[i] != b.data[i]) {
                return (a.data[i] < b.data[i]) == (a.sign == 1);
            }
        }
        return false;
    }
}

bool operator>(const BigInt& a, const BigInt& b)
{
    return b < a;
}

bool operator<=(const BigInt& a, const BigInt& b)
{
    return !(b < a);
}

bool operator>=(const BigInt& a, const BigInt& b)
{
    return !(a < b);
}

BigInt operator+(const BigInt& a, const BigInt& b)
{
    if(a.sign == b.sign) {
        if(a.size > b.size) {
            return BigInt::abs_add(a, b, a.sign);
        } else {
            return BigInt::abs_add(b, a, a.sign);
        }
    } else {
        if(BigInt::abs_less(b, a)) {
            return BigInt::abs_sub(a, b, a.sign);
        } else {
            return BigInt::abs_sub(b, a, -a.sign);
        }
    }
}

BigInt operator-(const BigInt& a, const BigInt& b)
{
    if(a.sign != b.sign) {
        if(a.size > b.size) {
            return BigInt::abs_add(a, b, a.sign);
        } else {
            return BigInt::abs_add(b, a, a.sign);
        }
    } else {
        if(BigInt::abs_less(b, a)) {
            return BigInt::abs_sub(a, b, a.sign);
        } else {
            return BigInt::abs_sub(b, a, -a.sign);
        }
    }
}

std::ostream& operator<<(std::ostream& stream, const BigInt& num)
{
    // реализация алгоритма double dabble для получения десятичного представления
    size_t n_bits = 16 * num.size;
    size_t scratch_size = n_bits / 3;
    char *scratch = new char[scratch_size + 1];
    for(size_t i = 0; i < scratch_size + 1; ++i) {
        scratch[i] = 0;
    }
    int smin = scratch_size - 2;
    for (size_t i = 0; i < num.size; ++i) {
        for (size_t j = 0; j < 16; ++j) {
            int shifted_in = (num.data[i] & (1 << (15 - j))) ? 1 : 0;
            for(int k = smin; k < scratch_size; ++k) {
                if(scratch[k] >= 5) {
                    scratch[k] += 3;
                }
            }
            if(scratch[smin] >= 8) {
                smin -= 1;
            }
            for(int k = smin; k < scratch_size - 1; ++k) {
                scratch[k] <<= 1;
                scratch[k] &= 0xF;
                scratch[k] |= (scratch[k + 1] >= 8);
            }
            scratch[scratch_size - 1] <<= 1;
            scratch[scratch_size - 1] &= 0xF;
            scratch[scratch_size - 1] |= shifted_in;
        }
    }
    if(num.sign == -1) {
        stream << '-';
    }
    size_t k;
    for(k = 0; k < scratch_size - 1; ++k) {
        if(scratch[k] != 0) {
            break;
        }
    }
    for(size_t i = k; i < scratch_size; ++i) {
        scratch[i] += '0';
    }
    stream << scratch + k;
    delete[] scratch;
    return stream;
}
