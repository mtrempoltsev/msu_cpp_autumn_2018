#include <iostream>
#include <cstdint>

class BigInt
{
    uint16_t* data;
    size_t size;
    int8_t sign;
public:
    BigInt(int32_t value = 0)
    {
        if(value >= 0) {
            data = new uint16_t[1](value);
            sign = 1;
        } else {
            data = new uint16_t[1](-value);
            sign = -1;
        }
    }

    BigInt(const BigInt& copied)
    {
        //одинаковые числа хранятся в разных местах
        //каждый объект монопольно владеет своей памятью
        ptr = new uint16_t[copied.size];
        size = copied.size;
        for(size_t i = 0; i < size; ++i) {
            ptr[i] = copied.data[i];
        }
        sign = copied.sign;
        delete[] data;
        data = ptr;
    }

    BigInt& operator=(const BigInt& copied)
    {
        if(this == &copied) {
            return *this;
        }
        ptr = new uint16_t[copied.size];
        size = copied.size;
        for(size_t i = 0; i < size; ++i) {
            ptr[i] = copied.data[i];
        }
        sign = copied.sign;
        delete[] data;
        data = ptr;
        return *this;
    }

    bool operator==(const BigInt& num) const
    {
        if(this == &num) {
            return true;
        } else if(size != num.size) {
            return false;
        } else {
            for(int i = 0; i < size; ++i) {
                if(data[i] != num.data[i]) {
                    return false;
                }
            }
            return true;
        }
    }

    bool operator!=(const BigInt& num) const
    {
        return !(*this == num);
    }

    bool operator<(const BigInt& num) const
    {
        return less(*this, num);
    }

    bool operator>(const BigInt& num) const
    {
        return less(num, *this);
    }

    bool operator<=(const BigInt& num) const
    {
        return !less(num, *this);
    }

    bool operator>=(const BigInt& num) const
    {
        return !less(*this, num);
    }

    BigInt operator-() const
    {
        BigInt tmp = *this;
        tmp.sign = -sign;
        return tmp;
    }

    BigInt operator+(const BigInt& num) const
    {
        if(sign == num.sign) {
            if(size > num.size) {
                return add(*this, num, sign);
            } else {
                return add(num, *this, sign);
            }
        } else {
            if(abs_less(num, *this)) {
                return sub(*this, num, sign);
            } else {
                return sub(num, *this, num.sign);
            }
        }
    }

    BigInt operator-(const BigInt& num) const
    {
        if(sign != num.sign) {
            if(size > num.size) {
                return add(*this, num, sign);
            } else {
                return add(num, *this, sign);
            }
        } else {
            if(abs_less(num, *this)) {
                return sub(*this, num, -sign);
            } else {
                return sub(num, *this, -sign);
            }
        }
    }
private:
    friend BigInt add(const BigInt& bigger, const BigInt& smaller, int8_t sign);
    // сумма модулей аргументов, взятая со знаком sign
    // при условии bigger.size >= smaller.size
    friend BigInt sub(const BigInt& bigger, const BigInt& smaller, int8_t sign);
    // разность модулей аргументов, взятая со знаком sign
    // при условии bigger >= smaller
    friend bool less(const BigInt& a, const BigInt& b);
    friend bool abs_less(const BigInt& a, const BigInt& b);

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
};

bool BigInt::less(const BigInt& a, const BigInt& b)
{
    if(&a == &b) {
        return false;
    } else if(a.sign != b.sign) {
        return a.sign < b.sign;
    } else if(a.size != a.size) {
        return a.size < a.size;
    } else {
        for(int i = 0; i < size; i++) {
            if(a.data[i] < b.data[i]) {
                return true;
            }
        }
        return false;
    }
}

bool BigInt::abs_less(const BigInt& a, const BigInt& b)
{
    if(a.data == b.data) {
        return false;
    } else if(a.size != a.size) {
        return a.size < a.size;
    } else {
        for(int i = 0; i < size; i++) {
            if(a.data[i] < b.data[i]) {
                return true;
            }
        }
        return false;
    }
}


BigInt BigInt::add(const BigInt& bigger, const BigInt& smaller, int8_t sign)
// сумма модулей аргументов, взятая со знаком sign
// при условии bigger.size >= smaller.size
{
    BigInt tmp = bigger;
    for(size_t i = 0; i < smaller.size; ++i) {
        uint16_t overflow = smaller[i];
        for(int j = tmp.size - smaller.size + i; j >= 0; --j) {
            overflow = add_overflow(tmp[j], overflow);
            if(!overflow) {
                break;
            }
        if(j == 0 && overflow) {
            tmp.double_size();
            tmp.data[tmp.size / 2 - 1] = overflow;
        }
    }
    tmp.sign = sign;
    return tmp;
}

BigInt BigInt::sub(const BigInt& bigger, const BigInt& smaller, int8_t sign)
// разность модулей аргументов, взятая со знаком sign
// при условии bigger >= smaller
{
    BigInt tmp = bigger;
    for(size_t i = 0; i < smaller.size; ++i) {
        bool = smaller[i];
        uint16_t overflow = smaller[i];
        for(int j = tmp.size - smaller.size + i; j >= 0; --j) {
            overflow = sub_overflow(tmp[j], overflow);
            if(!overflow) {
                break;
            }
        }
    }
    for(i = 0; i < tmp.size - 1; ++i) {
        if(tmp[i] != 0) {
            break;
        }
    }
    size_t significant = tmp.size - i;
    size_t new_size = 1;
    while(new_size < significant) {
        new_size *= 2;
    }
    if(new_size < size) {
        ptr = new uint16_t[new_size];
        for(size_t i = 0; i < new_size; ++i) {
            ptr[i] = tmp.data[tmp.size - new_size + i];
        }
        delete[] tmp.data;
        tmp.data = ptr;
    }
    tmp.sign = sign;
    return tmp;
}

uint16_t add_overflow(uint16_t& src, uint16_t num)
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

uint16_t sub_overflow(uint16_t& src, uint16_t num)
// вычитает num из src по модулю uint16_t и возвращает
// 1, если src - num < 0 (т.е. надо занимать 1 из след.блока BigInt), иначе 0
{
    if(num == 0) {
        return 1;
    }
    uint32_t sub = (static_cast<uint32_t>(src) ^ 0x00010000) - num;
    // попробовать убрать статик каст ^ потом
    src = static_cast<uint16_t>(sum & 0xFFFF);
    return static_cast<uint16_t>(~(sum >> 16) & 1);
}

std::ostream& BigInt::operator<<(std::ostream& stream, const BigInt& num)
{
    for(size_t i = 0; i < num.size; ++i) {
        if(num.sign == -1) {
            stream << '-';
        }
        stream << std::hex << num.data[i];
    }
}
