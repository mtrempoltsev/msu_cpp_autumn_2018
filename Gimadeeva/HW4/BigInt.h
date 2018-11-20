#include <iostream>
#include <cstdlib>
#include <cassert>
#include <memory.h>
 
class BigInt {

protected:
    int *mem_ = nullptr;
    int capacity_;
    int size_;
 
    bool negative_;

public:
    BigInt(long long x = 0) {
        negative_ = (x < 0);
        if (x < 0) {
            x = -x;
        }
 
        allocate(20);   // 10^20 > LLONG_MAX
        if ( x== 0) {
            size_= 1;
            mem_[0] = 0;
        } else {
            size_ = 0;
            while (x > 0) {
                mem_[size_] = x % 10;
                x /= 10;
                ++size_;
            }
        }
    }
 
    BigInt(const BigInt& num) {
        negative_ = num.negative_;
        allocate(num.size_);
        memcpy(mem_, num.mem_, num.size_ * sizeof(int));
        size_ = capacity_;
    }
 
    BigInt& operator =(const BigInt& num) {
        if (&num == this) {
            return *this;
        }
        negative_ = num.negative_;
        reallocate(num.size_);
        memcpy(mem_, num.mem_, num.size_ * sizeof(int));
        size_ = capacity_;
        return *this;
    }

    ~BigInt() {
        clear();
    }
 
    BigInt operator +(const BigInt& num) const {
        BigInt result;
        if (negative_ == num.negative_) {
            result.negative_ = negative_;
            result.reallocate(std::max(size_, num.size_) + 1);
            addAbsoluteValues(result, *this, num);
        } else {
            result.reallocate(std::max(size_, num.size_));
            if (lessByAbsoluteValue(num)) {
                result.negative_ = num.negative_;
                subtractAbsoluteValues(result, num, *this);
            } else {
                result.negative_ = negative_;
                subtractAbsoluteValues(result, *this, num);
            }
        }
        if ((result.size_ == 1) && (result.mem_[0] == 0))
            result.negative_ = false;
        return result;
    }
 
    BigInt operator -(const BigInt& num) const {
        BigInt result;
        if (negative_ != num.negative_) {
            result.negative_ = negative_;
            result.reallocate(std::max(size_, num.size_) + 1);
            addAbsoluteValues(result, *this, num);
        } else {
            result.reallocate(std::max(size_, num.size_));
            if (lessByAbsoluteValue(num)) {
                result.negative_ = !negative_;
                subtractAbsoluteValues(result, num, *this);
            } else {
                result.negative_ = negative_;
                subtractAbsoluteValues(result, *this, num);
            }
        }
        if ((result.size_ == 1) && (result.mem_[0] == 0))
            result.negative_ = false; 
        return result;
    }
 
    BigInt operator -() const {
        BigInt result;
        if ( !(size_ == 1 && mem_[0] == 0) ) {
            result.negative_ = !negative_;
        } else {
            result.negative_=false;
        }
        result.reallocate(size_);
        memcpy(result.mem_, mem_, size_ * sizeof(int));
        result.size_=size_;
            
        return result;
    }
 
    void operator +=(const BigInt& num) {
        *this = *this + num;
    }
 
    void operator -=(const BigInt& num) {
        *this = *this - num;
    }
 
    bool operator <(const BigInt& num) const {
        if (negative_ != num.negative_) {
            return negative_;
        } else if (equalByAbsoluteValue(num)) {
            return false;
        } else {
            return negative_ ^ lessByAbsoluteValue(num);
        }
    }
 
    bool operator ==(const BigInt& num) const {
        if (negative_ != num.negative_) {
            return false;
        }
        else if (equalByAbsoluteValue(num)) {
            return true;
        } else {
            return false;
        }
    }
 
    bool operator >(const BigInt& num) const {
        return num < *this;
    }
 
    bool operator <=(const BigInt& num) const {
        return !(*this > num);
    }
 
    bool operator >=(const BigInt& num) const {
        return !(*this < num);
    }
 
    bool operator !=(const BigInt& num) const {
        return !(*this == num);
    }
 
    int size() const {
        return size_;
    }
 
    const int* getMem() const {
        return mem_;
    }
 
private:
    void clear() {
        if (mem_) {
            free(mem_);
            mem_ = nullptr;
        }
        capacity_ = 0;
    }
 
    void allocate(int capacity) {
        assert(!mem_);
        capacity_ = capacity;
        mem_ = (int*)malloc(sizeof(int) * capacity_);
        memset(mem_, 0, sizeof(int) * capacity_);
    }
 
    void reallocate(int capacity) {
        clear();
        allocate(capacity);
    }
 
    bool equalByAbsoluteValue(const BigInt& num) const {
        if (size_ != num.size_) {
            return false;
        } 
        else {
            return memcmp(mem_, num.mem_, size_ * sizeof(int)) == 0;
        }
    }
 
    bool lessByAbsoluteValue(const BigInt& num) const {
        if (size_ != num.size_) {
            return size_ < num.size_;
        } else {
            for (int i = size_-1; i >= 0; --i) {
                if (mem_[i] != num.mem_[i]) {
                    return mem_[i] < num.mem_[i];
                }
            }
            return false;
        }
    }
 
    void addAbsoluteValues(BigInt& sum, const BigInt& first, const BigInt& second) const {
        int carry = 0;
        const int *first_mem = first.mem_, *second_mem = second.mem_;
        sum.size_ = 0;
        while (carry > 0 || sum.size_ < first.size_ || sum.size_ < second.size_) {
            if (sum.size_ < first.capacity_) {
                carry += first_mem[sum.size_];
            }
            if (sum.size_ < second.capacity_) {
                carry += second_mem[sum.size_];
            }
            sum.mem_[sum.size_] = carry % 10;
            carry /= 10;
            ++sum.size_;
        }
        // std::cerr << sum.size_ << " " << sum.mem_[sum.size_ - 1] << "\n";
    }
 
    void subtractAbsoluteValues(BigInt& sum, const BigInt& first, const BigInt& second) const {
        bool t = false;
        int s=0;
        for (int i = 0; i < second.size_; ++i) {
            if (first.mem_[i] >= second.mem_[i]) {
                if (t == false) {
                    sum.mem_[i] = first.mem_[i]-second.mem_[i];
                } else {
                    if (first.mem_[i] > second.mem_[i]) {
                        sum.mem_[i] = first.mem_[i]-1-second.mem_[i];
                        t = false;
                    } else {
                        sum.mem_[i] = first.mem_[i]+9-second.mem_[i];
                    }
                }
            } else {
                if (t == false) {
                    t = true;
                    sum.mem_[i] = first.mem_[i]+10-second.mem_[i];
                } else {
                    sum.mem_[i] = first.mem_[i]+9-second.mem_[i];
                }
            }
            if (sum.mem_[i] > 0) s = i;
        }
        for (int i = second.size_; i < first.size_; ++i) {
            if (t == false) {
                sum.mem_[i] = first.mem_[i];
            } else {
                if (first.mem_[i] > 0) {
                    sum.mem_[i] = first.mem_[i]-1;
                    t = false;
                } else {
                    sum.mem_[i] = first.mem_[i]+9;
                }
            }
            if (sum.mem_[i] > 0) s = i;
        }
        sum.size_ = s + 1;
    }
};
 
std::ostream& operator <<(std::ostream& ostream, const BigInt& num) {
    if (num.size() == 0) {
        ostream << "0";
        return ostream;
    }
    if (num < 0) {
        ostream << "-";
    }
    const int *mem = num.getMem();
    for (int i = num.size() - 1; i >= 0; --i) {
        ostream << mem[i];
    }
    return ostream;
}
