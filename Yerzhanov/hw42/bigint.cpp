#pragma once

#include <iostream>
#include <cstring>
#include <cstdio>
#include <string>


class BigInt {
private:
    char* data_;
    bool is_pos_;
    size_t size_;
public:
    BigInt(int64_t val = 0)
            :   data_(nullptr)
            ,   is_pos_(true)
            ,   size_(0) {
        if (val < 0) {
            val *= -1;
            is_pos_ = false;
        }
        std::string tmp = std::to_string(val);
        size_ = tmp.size();
        data_ = new char[size_ + 1];
        std::strcpy(data_, tmp.c_str());  //Здесь можеть вылезти ошибка, так как не ставлю ноль в конце
    }

    BigInt(const BigInt& s)
            :   data_(new char[s.size_ + 1])
            ,   is_pos_(s.is_pos_)
            ,   size_(s.size_) {
        std::copy(s.data_, s.data_ + s.size_ + 1, data_);
    }

    BigInt(BigInt&& s)
            :   data_(nullptr)
            ,   is_pos_(s.is_pos_)
            ,   size_(s.size_) {
        std::swap(data_, s.data_);
    }

    ~BigInt(void) {
        if (data_ != nullptr) {
            delete[] data_;
        }
    }

    BigInt& operator=(const BigInt& s) {
        if (data_ != nullptr) {
            delete[] data_;
        }
        is_pos_ = s.is_pos_;
        size_ = s.size_;
        data_ = new char[size_ + 1];
        std::copy(s.data_, s.data_ + s.size_ + 1, data_);
        return *this;
    }

    BigInt& operator=(BigInt&& s) {
        if (data_ != nullptr) {
            delete[] data_;
            data_ = nullptr;
        }
        is_pos_ = s.is_pos_;
        size_ = s.size_;
        std::swap(data_, s.data_);
        return *this;
    }

    bool operator==(const BigInt& s) const {
        if (data_[0] == '0' && data_[1] == 0 && s.data_[0] == '0' && s.data_[1] == 0) {
            return true;
        } else if (is_pos_ != s.is_pos_ || size_ != s.size_) {
            return false;
        }
        return !strcmp(data_, s.data_);
    }

    bool operator>(const BigInt& s) const {
        if (is_pos_ > s.is_pos_ || !(is_pos_ || s.is_pos_) && size_ < s.size_ || is_pos_ && s.is_pos_ && size_ > s.size_) {
            return true;
        } else if (is_pos_ < s.is_pos_ || !(is_pos_ || s.is_pos_) && size_ > s.size_ || is_pos_ && s.is_pos_ && size_ < s.size_) {
            return false;
        }
        return strcmp(data_, s.data_) > 0 ^ !is_pos_;
    }

    bool operator<(const BigInt& s) const { return !(*this == s || *this > s); }
    bool operator>=(const BigInt& s) const { return !(*this < s); }
    bool operator<=(const BigInt& s) const { return !(*this > s); }
    bool operator!=(const BigInt& s) const { return !(*this == s); }

    BigInt operator-(void) const {
        BigInt ret = *this;
        ret.is_pos_ ^= true;
        return ret;
    }

    BigInt operator+(const BigInt& s) const {
        if (is_pos_ ^ s.is_pos_) {
            return *this - (-s);
        }
        BigInt ret;
        char* big(data_);
        char* small(s.data_);
        if (size_ < s.size_) {
            std::swap(big, small);
        }
        size_t max = std::max(size_, s.size_);
        size_t min = std::min(size_, s.size_);
        ret.data_ = new char[max + 2];
        int8_t ovf = 0;
        for (size_t i = 0; i < min; ++i) {
            int8_t tmp = big[max - 1 - i] - '0' + small[min - 1 - i] - '0' + ovf;
            ret.data_[max - i] = tmp % 10 + '0';
            ovf = tmp / 10;
        }
        for (size_t i = min; i < max; ++i) {
            size_t tmp = big[max - 1 - i] - '0' + ovf;
            ret.data_[max - i] = tmp % 10 + '0';
            ovf = tmp / 10;
        }
        if (ovf != 0) {
            ret.data_[0] = ovf + '0';
            ret.size_ = max + 1;
            ret.data_[max + 1] = 0;
        } else {
            for (size_t i = 0; i < max; ++i) {
                ret.data_[i] = ret.data_[i + 1];
            }
            ret.size_ = max;
            ret.data_[max] = 0;
        }
        ret.is_pos_ = is_pos_;
        return ret;
    }

    BigInt operator-(const BigInt& s) const {
        if (is_pos_ ^ s.is_pos_) {
            return *this + (-s);
        }
        BigInt ret;
        char* big(data_);
        char* small(s.data_);
        if (is_pos_ && s.is_pos_ && *this >= s || !(is_pos_ || s.is_pos_) && *this <= s) {
            ret.is_pos_ = is_pos_;
        } else {
            ret.is_pos_ = s.is_pos_ ^ true;
            std::swap(big, small);
        }
        size_t max = std::max(size_, s.size_);
        size_t min = std::min(size_, s.size_);
        bool ovf = false;
        ret.data_ = new char[max + 1];
        ret.data_[max] = 0;
        for (size_t i = 0; i < min; ++i) {
            ovf = big[max - 1 - i] < small[min - 1 - i];
            big[max - 2 - i] -= ovf;
            ret.data_[max - 1 - i] = big[max - 1 - i] + 10 * ovf - small[min - 1 - i] + '0';
        }
        for (size_t i = min; i < max; ++i) {
            ovf = big[max - 1 - i] < '0';
            big[max - 2 - i] -= ovf;
            ret.data_[max - 1 - i] = big[max - 1 - i] + 10 * ovf;
        }
        size_t k = 0;
        while (ret.data_[k++] == '0');
        if (k == max + 1) {
            ret.data_[0] = '0';
            ret.data_[1] = 0;
            return ret;
        }
        for (size_t i = 0; i < max - k + 1; ++i) {
            ret.data_[i] = ret.data_[i + k - 1];
        }
        ret.data_[max - k + 1] = 0;
        return ret;
    }
    friend std::ostream& operator<<(std::ostream& out, const BigInt& self);
};


std::ostream& operator<<(std::ostream& out, const BigInt& obj) {
    if (!obj.is_pos_) {
        out << '-';
    }
    out << obj.data_;
    return out;
}
void check(int64_t x, int64_t y)
{
    const BigInt bigX = x;
    const BigInt bigY = y;

    std::cout << bigX - bigY << '\n';
    std::cout << bigX - bigY << '\n';
    if (bigX + bigY != BigInt(x + y))
    {
        std::cout << x << " + " << y << " != " << x + y << " got " << bigX + bigY << '\n';
    }

    if (bigX - bigY != BigInt(x - y))
    {
        std::cout << x << " - " << y << " != " << x - y << " got " << bigX - bigY << '\n';
    }
}

int main(void) {
    check(21, 9);
    std::cout << BigInt(21) + BigInt(9) << '\n';
    return 0;
}
