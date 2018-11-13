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
            ,   is_pos_(val >= 0)
            ,   size_(0) {
        if (!is_pos_) {
            val *= -1;
        }
        int64_t tmp = val;
        do {
            ++size_;
            tmp /= 10;
        } while (tmp);
        data_ = new char[size_ + 1];
        data_[size_] = 0;
        for (size_t i = size_ - 1; i > 0; --i) {
            data_[i] = val % 10 + '0';
            val /= 10;
        }
        data_[0] = val + '0';
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

    ~BigInt() {
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

    BigInt operator-() const {
        BigInt ret = *this;
        ret.is_pos_ ^= true;
        return ret;
    }

    BigInt operator+(const BigInt& s) const {
        if (is_pos_ ^ s.is_pos_) {
            return *this - (-s);
        }
        BigInt ret;
        const char* big(data_);
        const char* small(s.data_);
        if (size_ < s.size_) {
            std::swap(big, small);
        }
        size_t max = std::max(size_, s.size_);
        size_t min = std::min(size_, s.size_);
        char* mod = new char[max + 2];
        int8_t ovf = 0;
        for (size_t i = 0; i < min; ++i) {
            int8_t tmp = big[max - 1 - i] - '0' + small[min - 1 - i] - '0' + ovf;
            mod[max - i] = tmp % 10 + '0';
            ovf = tmp / 10;
        }
        for (size_t i = min; i < max; ++i) {
            size_t tmp = big[max - 1 - i] - '0' + ovf;
            mod[max - i] = tmp % 10 + '0';
            ovf = tmp / 10;
        }
        mod[max + 1] = 0;
        if (ovf != 0) {
            mod[0] = ovf + '0';
            ret.data_ = new char[max + 2];
            std::copy(mod, mod + max + 2, ret.data_);
            ret.size_ = max + 1;
        } else {
            ret.data_ = new char[max + 1];
            std::copy(mod + 1, mod + max + 2, ret.data_);
            ret.size_ = max;
        }
        if (mod != nullptr) {
            delete[] mod;
        }
        ret.is_pos_ = is_pos_;
        return ret;
    }

    BigInt operator-(const BigInt& s) const {
        if (is_pos_ ^ s.is_pos_) {
            return *this + (-s);
        }
        BigInt ret;
        const char* big = data_;
        const char* small = s.data_;
        if (is_pos_ && s.is_pos_ && *this >= s || !(is_pos_ || s.is_pos_) && *this <= s) {
            ret.is_pos_ = is_pos_;
        } else {
            ret.is_pos_ = s.is_pos_ ^ true;
            std::swap(big, small);
        }
        size_t max = std::max(size_, s.size_);
        size_t min = std::min(size_, s.size_);
        bool ovf = false, ovf_next = false;
        char* mod = new char[max + 1];
        for (size_t i = 0; i < min; ++i) {
            ovf_next = big[max - 1 - i] - ovf < small[min - 1 - i];
            mod[max - 1 - i] = big[max - 1 - i] - ovf + 10 * ovf_next - small[min - 1 - i] + '0';
            ovf = ovf_next;
        }
        for (size_t i = min; i < max; ++i) {
            ovf_next = big[max - 1 - i] - ovf < '0';
            mod[max - 1 - i] = big[max - 1 - i] - ovf + 10 * ovf_next;
            ovf = ovf_next;
        }
        size_t k = 0;
        while (mod[k++] == '0');
        if (k == max + 1) {
            ret.data_ = new char[2];
            ret.data_[0] = '0';
            ret.data_[1] = 0;
            ret.size_ = 1;
            if (mod != nullptr) {
                delete[] mod;
            }
            return ret;
        }
        mod[max] = 0;
        ret.data_ = new char[max - k + 2];
        std::copy(mod + k - 1, mod + max + 1, ret.data_);
        ret.size_ = max - k + 1;
        if (mod != nullptr) {
            delete[] mod;
        }
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
