//
//  bigint.h
//  
//
//  Created by Мамаев Павел on 10/11/2018.
//

#ifndef bigint_h
#define bigint_h

#include <ostream>
#include <iostream>

using std::ostream;

class BigInt {
    
private:
    
    int vec_size_;
    int num_size_;
    bool sign_;
    char *number_;
    
public:
    
    BigInt(long long a = 0) {
        sign_ = false;
        if (a < 0) {
            a = -a;
            sign_ = true;
        }
        vec_size_ = 32;
        number_ = new char[vec_size_];
        if (a == 0)
            number_[0] = 0;
        int i = 0;
        while (a > 0) {
            number_[i++] = a % 10;
            a /= 10;
        }
        num_size_ = (i == 0) ? 1 : i;
        for (int i = num_size_; i < vec_size_; ++i)
            number_[i] = 0;
    }
    
    ~BigInt() {
        delete[] number_;
    }
    
    BigInt(const BigInt &copied) {
        vec_size_ = copied.vec_size_;
        num_size_ = copied.num_size_;
        sign_ = copied.sign_;
        number_ = new char[vec_size_];
        for (int i = 0; i < vec_size_; i++) {
            number_[i] = copied.number_[i];
        }
    }
    
    BigInt& operator= (const BigInt& copied) {
        if (this == &copied)
            return *this;
        delete[] number_;
        number_ = new char[copied.vec_size_];
        vec_size_ = copied.vec_size_;
        num_size_ = copied.num_size_;
        sign_ = copied.sign_;
        for (int i = 0; i < vec_size_; i++)
            number_[i] = copied.number_[i];
        return *this;
    }
    
    BigInt operator- () const {
        BigInt tmp = *this;
        if (*this != 0)
            tmp.sign_ = (sign_) ? false : true;
        return tmp;
    }
    
    BigInt operator+ (const BigInt &rvalue) const {
        if (rvalue == 0)
            return *this;
        if (sign_ == rvalue.sign_) {
            if (num_size_ < rvalue.num_size_)
                return (rvalue + *this);
            BigInt res;
            res.sign_ = sign_;
            res.vec_size_ = vec_size_;
            if (num_size_ + 1 > vec_size_) {
                res.vec_size_ = 2 * vec_size_;
                delete[] res.number_;
                res.number_ = new char[res.vec_size_];
                for (int i = 0; i < vec_size_; ++i)
                    res.number_[i] = 0;
            }
            int flag = 0;
            for (int i = 0; i <= num_size_; ++i) {
                if (i < rvalue.num_size_ && i < num_size_)
                    res.number_[i] = number_[i] + rvalue.number_[i] + flag;
                else if (i < num_size_)
                    res.number_[i] = number_[i] + flag;
                else if (flag)
                    res.number_[i] = 1;
                if (res.number_[i] > 9) {
                    res.number_[i] -= 10;
                    flag = 1;
                } else
                    flag = 0;
            }
            if (res.number_[num_size_] > 0)
                res.num_size_ = num_size_ + 1;
            else
                res.num_size_ = num_size_;
            return res;
        } else
            return (*this - (-rvalue));
    }
    
    BigInt operator- (const BigInt &rvalue) const {
        if (rvalue == 0)
            return *this;
        if (sign_ == rvalue.sign_) {
            if (*this == rvalue)
                return 0;
            if ((!sign_) && (*this < rvalue))
                return -(rvalue - *this);
            if ((sign_) && (*this > rvalue))
                return -(rvalue - *this);
            BigInt res;
            res.sign_ = sign_;
            res.vec_size_ = vec_size_;
            res.num_size_ = num_size_;
            delete[] res.number_;
            res.number_ = new char[res.vec_size_];
            for (int i = 0; i < vec_size_; ++i)
                res.number_[i] = 0;
            int flag = 0;
            for (int i = 0; i < num_size_; ++i) {
                if (i < rvalue.num_size_)
                    res.number_[i] = number_[i] - rvalue.number_[i] + flag;
                else
                    res.number_[i] = number_[i] + flag;
                if (res.number_[i] < 0) {
                    res.number_[i] += 10;
                    flag = -1;
                } else
                    flag = 0;
            }
            while (res.number_[res.num_size_ - 1] == 0 && res.num_size_ != 1)
                res.num_size_--;
            return res;
        } else
            return (*this + (-rvalue));
    }
    
    bool operator== (const BigInt &rvalue) const {
        if (this == &rvalue)
            return true;
        else {
            if ((sign_ != rvalue.sign_) || (num_size_ != rvalue.num_size_))
                return false;
            for (int i = 0; i < num_size_; i++)
                if (number_[i] != rvalue.number_[i])
                    return false;
        }
        return true;
    }
    
    bool operator< (const BigInt &rvalue) const {
        if (*this == rvalue)
            return false;
        if (sign_ > rvalue.sign_)
            return true;
        if (sign_ < rvalue.sign_)
            return false;
        if (num_size_ > rvalue.num_size_)
            return (!sign_) ? false : true;
        if (num_size_ < rvalue.num_size_)
            return (!sign_) ? true : false;
        for (int i = num_size_ - 1; i >= 0; --i) {
            if (number_[i] > rvalue.number_[i])
                return (!sign_) ? false : true;
            if (number_[i] < rvalue.number_[i])
                return (sign_ == false) ? true : false;
        }
        return 0;
    }
    
    bool operator> (const BigInt &rvalue) const {
        if (rvalue == *this)
            return false;
        return (rvalue < *this);
    }
    
    bool operator>= (const BigInt &rvalue) const {
        return !(*this < rvalue);
    }
    
    bool operator<= (const BigInt &rvalue) const {
        return !(*this > rvalue);
    }
    
    bool operator!= (const BigInt &rvalue) const {
        return !(*this == rvalue);
    }
    friend ostream &operator<< (ostream &, const BigInt &);
};

ostream &operator<< (ostream &out, const BigInt &obj) {
    if (obj.sign_)
        out << '-';
    for (int i = obj.num_size_ - 1; i >= 0; --i)
        out << obj.number_[i] - 0;
    return out;
}

#endif /* bigint_h */
