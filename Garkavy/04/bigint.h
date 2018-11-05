#pragma once
#include <iostream>
// #include <iomanip>
#include <cstdint>

// uint16_t ~ store uint
// uint32_t ~ 2x store uint
// int32_t ~ constr argument int
// 0x00010000
// 0xFFFF
// 16

class BigInt
{
// public: // delete this later!
    uint16_t* data = nullptr;
    size_t size = 0;
    int8_t sign = 1;

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

    static bool less(const BigInt& a, const BigInt& b)
    {
        if(&a == &b) {
            return false;
        } else if(a.sign != b.sign) {
            return a.sign < b.sign;
        } else if(a.size != b.size) {
            // std::cout << "sizes: " << a.size << ' ' << b.size << '\n';
            return (a.size < b.size) ^ (a.sign == 1);
        } else {
            // std::cout << "last step of comparing" << '\n';
            for(size_t i = 0; i < a.size; i++) {
                if(a.data[i] != b.data[i]) {
                    return (a.data[i] < b.data[i]) ^ (a.sign == 1);
                }
            }
            return false;
        }
    }

    static bool abs_less(const BigInt& a, const BigInt& b)
    {
        if(a.data == b.data) {
            return false;
        } else if(a.size != b.size) {
            // std::cout << "sizes: " << a.size << ' ' << b.size << '\n';
            return a.size < b.size;
        } else {
            // std::cout << "last step of comparing" << '\n';
            for(int i = 0; i < a.size; i++) {
                if(a.data[i] != b.data[i]) {
                    // std::cout << a.data[i] << "<" << b.data[i] << '\n';
                    return a.data[i] < b.data[i];
                }
            }
            // std::cout << "no break! => ==" << '\n';
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
            return 1;
        }
        uint32_t sub = (static_cast<uint32_t>(src) ^ 0x00010000) - num;
        // попробовать убрать статик каст ^ потом
        src = static_cast<uint16_t>(sub & 0xFFFF);
        return static_cast<uint16_t>(~(sub >> 16) & 1);
    }

    static BigInt abs_add(const BigInt& bigger, const BigInt& smaller, int8_t sign)
    // сумма модулей аргументов, взятая со знаком sign
    // при условии bigger.size >= smaller.size
    {
        // std::cout << "'add':test1" << '\n';
        BigInt tmp = bigger;
        for(size_t i = 0; i < smaller.size; ++i) {
            // std::cout << "'add':test2" << '\n';
            uint16_t overflow = smaller.data[i];
            int j;
            for(j = tmp.size - smaller.size + i; j >= 0; --j) {
                overflow = add_overflow(tmp.data[j], overflow);
                if(!overflow) {
                    break;
                }
            }
            if(j < 0 && overflow) {
                // std::cout << "to double" << '\n';
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
        // std::cout << "B - S: " << bigger << '-' << smaller << '\n';
        BigInt tmp = bigger;
        // std::cout << "tmp.size = " << tmp.size << '\n';
        // std::cout << "smaller.size = " << smaller.size << '\n';
        for(size_t i = 0; i < smaller.size; ++i) {
            uint16_t overflow = smaller.data[i];
            int j;
            for(j = tmp.size - smaller.size + i; j >= 0; --j) {
                overflow = sub_overflow(tmp.data[j], overflow);
                // std::cout << "data, overflow:" << tmp.data[j] << " " << overflow << '\n';
                if(!overflow) {
                    break;
                }
            }
            // if(j < 0 && overflow) {
            //     std::cout << "i = " << i << ": ASSERT EXCEPTION" << '\n';
            // }
        }
        size_t i;
        for(i = 0; i < tmp.size - 1; ++i) {
            if(tmp.data[i] != 0) {
                break;
            }
        }
        // std::cout << "found i = " << i << '\n';
        size_t significant = tmp.size - i;
        size_t new_size = 1;
        while(new_size < significant) {
            new_size *= 2;
        }
        // std::cout << "new_size=" << new_size << '\n';
        if(new_size < tmp.size) {
            uint16_t* ptr = new uint16_t[new_size];
            for(size_t i = 0; i < new_size; ++i) {
                ptr[i] = tmp.data[tmp.size - new_size + i];
            }
            delete[] tmp.data;
            tmp.data = ptr;
            tmp.size = new_size;
        }
        tmp.sign = tmp == 0 ? 1 : sign;
        return tmp;
    }

public:
    BigInt(int32_t value = 0)
    {
        data = new uint16_t[1];
        size = 1;
        if(value >= 0) {
            data[0] = value;
            sign = 1;
        } else {
            data[0] = -value;
            sign = -1;
        }
    }

    BigInt(const BigInt& copied)
    {
        // std::cout << "copy test: data=" <a< data << '\n';
        // std::cout << "copy test: size=" << size << '\n';
        //одинаковые числа хранятся в разных местах
        //каждый объект монопольно владеет своей памятью
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

    friend bool operator==(const BigInt& a, const BigInt& b);
    friend bool operator!=(const BigInt& a, const BigInt& b);
    friend bool operator<(const BigInt& a, const BigInt& b);
    friend bool operator>(const BigInt& a, const BigInt& b);
    friend bool operator<=(const BigInt& a, const BigInt& b);
    friend bool operator>=(const BigInt& a, const BigInt& b);
    friend BigInt operator+(const BigInt& a, const BigInt& b);
    friend BigInt operator-(const BigInt& a, const BigInt& b);
    // bool operator==(const BigInt& num) const
    // {
    //     if(this == &num) {
    //         return true;
    //     } else if(size != num.size) {
    //         return false;
    //     } else {
    //         for(int i = 0; i < size; ++i) {
    //             if(data[i] != num.data[i]) {
    //                 return false;
    //             }
    //         }
    //         return true;
    //     }
    // }
    //
    // bool operator!=(const BigInt& num) const
    // {
    //     return !(*this == num);
    // }
    //
    // bool operator<(const BigInt& num) const
    // {
    //     return less(*this, num);
    // }
    //
    // bool operator>(const BigInt& num) const
    // {
    //     return less(num, *this);
    // }
    //
    // bool operator<=(const BigInt& num) const
    // {
    //     return !less(num, *this);
    // }
    //
    // bool operator>=(const BigInt& num) const
    // {
    //     return !less(*this, num);
    // }
    //
    BigInt operator-() const
    {
        BigInt tmp = *this;
        tmp.sign = -sign;
        return tmp;
    }
    //
    // BigInt operator+(const BigInt& num) const
    // {
    //     if(sign == num.sign) {
    //         if(size > num.size) {
    //             return abs_add(*this, num, sign);
    //         } else {
    //             return abs_add(num, *this, sign);
    //         }
    //     } else {
    //         if(abs_less(num, *this)) {
    //             return abs_sub(*this, num, sign);
    //         } else {
    //             return abs_sub(num, *this, -sign);
    //         }
    //     }
    // }
    //
    // BigInt operator-(const BigInt& num) const
    // {
    //     if(sign != num.sign) {
    //         if(size > num.size) {
    //             return abs_add(*this, num, sign);
    //         } else {
    //             return abs_add(num, *this, -sign);
    //         }
    //     } else {
    //         if(abs_less(num, *this)) {
    //             // std::cout << "case1: " << num << "<" << *this << '\n';
    //             return abs_sub(*this, num, sign);
    //         } else {
    //             // std::cout << "case2: " << *this << "<" << num << '\n';
    //             return abs_sub(num, *this, -sign);
    //         }
    //     }
    // }
    friend std::ostream& operator<<(std::ostream&, const BigInt& num);
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
        // std::cout << "sizes: " << a.size << ' ' << b.size << '\n';
        return (a.size < b.size) == (a.sign == 1);
    } else {
        // std::cout << "last step of comparing" << '\n';
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
            return BigInt::abs_add(b, a, -a.sign);
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
    // stream << "hex:";
    // if(num.sign == -1) {
    //     stream << '-';
    // }
    // for(size_t i = 0; i < num.size; ++i) {
    //     stream << std::setfill('0') << std::setw(4) << std::hex << num.data[i] << ' ';
    // }
    // stream << std::dec << '\n';

    size_t n_bits = 16 * num.size;
    size_t scratch_size = n_bits / 3;
    char *scratch = new char[scratch_size + 1];
    for(size_t i = 0; i < scratch_size + 1; ++i) {
        scratch[i] = 0;
    }
    int smin = scratch_size - 2;    /* speed optimization */
    // std::cout << "ddtest1\n";
    for (size_t i = 0; i < num.size; ++i) {
        for (size_t j = 0; j < 16; ++j) {
            /* This bit will be shifted in on the right. */
            int shifted_in = (num.data[i] & (1 << (15 - j))) ? 1 : 0;

            /* Add 3 everywhere that scratch[k] >= 5. */
            for(int k = smin; k < scratch_size; ++k) {
                if(scratch[k] >= 5) {
                    scratch[k] += 3;
                }
            }
            /* Shift scratch to the left by one position. */
            if(scratch[smin] >= 8) {
                smin -= 1;
            }
            for(int k = smin; k < scratch_size - 1; ++k) {
                scratch[k] <<= 1;
                scratch[k] &= 0xF;
                scratch[k] |= (scratch[k + 1] >= 8);
            }

            /* Shift in the new bit from arr. */
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
    // if(scratch[scratch_size] != 0) {
    //     std::cout << "ASSERT EXCEPTION IN SCRATCH" << '\n';
    // }
    stream << scratch + k;
    delete[] scratch;
    return stream;
}
