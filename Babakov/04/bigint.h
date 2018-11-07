#include <string>
#include <iostream>

enum {
    SECTION_MAX = 1000000000,
    SECTION_SIZE = 9
};

std::string norm_string(const std::string& s)
{
    std::string temp = s;
    while (temp[0] == '0' && temp.size() != 1)
        temp = temp.substr(1, temp.size() - 1);
    while (temp.size() % SECTION_SIZE)
        temp = "0" + temp;
    return temp;
}


class BigInt {

    size_t length_;
    int *data_;
    int sign_;

    static BigInt add(const BigInt& add1, const BigInt& add2)
    {
        if (add2.length_ > add1.length_)
            return add(add2, add1);
        BigInt temp(add1);
        size_t i = 0;
        int cf = 0; //carry flag
        for (i; i < add2.length_; i++) {
            temp.data_[i] += add2.data_[i] + cf;
            if (temp.data_[i] > SECTION_MAX) {
                cf = 1;
                temp.data_[i] %= SECTION_MAX;
            } else {
                cf = 0;
            }
        }
        if (cf) {
            if (i == add1.length_) {
                int *temp_data = new int[temp.length_ + 1];
                std::copy(temp.data_, temp_data + temp.length_, temp_data);
                temp_data[i] = 1;
                delete[] temp.data_;
                temp.length_++;
                temp.data_ = temp_data;
            } else {
                temp.data_[i]++;
            }
        }
        return temp;
    }

    static BigInt sub(const BigInt& sub1, const BigInt& sub2) 
    {
        if (sub2 > sub1)
            return -sub(sub2, sub1);
        BigInt temp(sub1);
        size_t i = 0;
        int cf = 0; // carry flag
        for (i; i < sub2.length_; i++) {
            temp.data_[i] -= sub2.data_[i] + cf;
            if (temp.data_[i] >= 0) {
                cf = 0;
            } else {
                cf = 1;
                temp.data_[i] += SECTION_MAX;
            }
        }
        for (i; i < temp.length_; i++) {
            temp.data_[i] -= cf;
            if(temp.data_[i] >= 0) {
                cf = 0;
            } else {
                cf = 1;
                temp.data_[i] += SECTION_MAX;
            }
        }
        int zero_section_amount = 0;
        for (int j = temp.length_ - 1; j >= 1; j--) {
            if (temp.data_[j] == 0)
                zero_section_amount++;
            else
                break;
        }
        if (zero_section_amount) {
            int *temp_data = new int[temp.length_ - zero_section_amount];
            std::copy(temp.data_, temp.data_ + temp.length_ - zero_section_amount,
                      temp_data);
            delete[] temp.data_;
            temp.data_ = temp_data;
            temp.length_ -= zero_section_amount;
        }
        return temp;
    }

public:
    BigInt() : BigInt("0") { }

    BigInt(int64_t init) : BigInt(std::to_string(init)) { }

    BigInt(const std::string& init) 
    {
        std::string temp = init;
        if (temp[0] == '-') {
            sign_ = -1;
            temp = temp.substr(1, temp.size() - 1);
        } else {
            sign_ = 1;
        }

        temp = norm_string(temp);
        length_ = temp.size() / SECTION_SIZE;
        data_ = new int[length_];

        for (size_t i = 0; i < length_; i++) {
            int j = SECTION_SIZE * (length_ - 1 - i);
            data_[i] = std::atoi(temp.substr(j, SECTION_SIZE).c_str());
        }
    }

    BigInt(const BigInt& add)
    {
        sign_ = add.sign_;
        length_ = add.length_;
        data_ = new int[length_];
        for (size_t i = 0; i < length_; ++i) {
            data_[i] = add.data_[i];
        }
    }

    BigInt& operator= (const BigInt& init)
    {
        length_ = init.length_;
        if (data_) 
            delete[] data_;
        data_ = new int[length_];
        for (size_t i = 0; i < length_; i++) {
            data_[i] = init.data_[i];
        }
        sign_ = init.sign_;
        return *this;
    }

    BigInt operator+() const 
    {
        BigInt temp(*this);
        return temp;
    }

    BigInt operator-() const
    {
        BigInt temp(*this);
        temp.sign_ *= -1;
        return temp;
    }

    BigInt absolute() const
    {
        BigInt temp(*this);
        temp.sign_ = 1;
        return temp;
    }

    ~BigInt()
    {
        delete[] data_;
    }

    friend std::ostream& operator<<(std::ostream& os, const BigInt& num);
    friend bool operator==(const BigInt& comp1, const BigInt& comp2);
    friend bool operator!=(const BigInt& comp1, const BigInt& comp2);
    friend bool operator>=(const BigInt& comp1, const BigInt& comp2);
    friend bool operator<=(const BigInt& comp1, const BigInt& comp2);
    friend bool operator>(const BigInt& comp1, const BigInt& comp2);
    friend bool operator<(const BigInt& comp1, const BigInt& comp2);
    friend BigInt operator+(const BigInt& add1, const BigInt& add2);
    friend BigInt operator-(const BigInt& sub1, const BigInt& sub2);

};

std::ostream& operator<<(std::ostream& os, const BigInt& num)
{
    if (num == BigInt(0)) {
        os << '0';
        return os;
    }
    if (num.sign_ == -1) {
        os << '-';
    }
    for (int i = num.length_ - 1; i >= 0; i--) {
        std::string tmp = std::to_string(num.data_[i]);
        if (i != num.length_ - 1) 
            tmp = norm_string(tmp);
        os << tmp;
    }
    return os;
}

bool operator==(const BigInt& comp1, const BigInt& comp2) 
{
    if (comp1.data_[0] == 0 && comp2.data_[0] == 0 &&
        comp2.length_ <= 1 && comp1.length_ <= 1)
        return true;
    if (comp1.sign_ != comp2.sign_)
        return false;
    if (comp1.length_ != comp2. length_)
        return false;
    for (size_t i = 0; i < comp1.length_; i++)
        if (comp2.data_[i] != comp1.data_[i])
            return false;
    return true;
}

bool operator!=(const BigInt& comp1, const BigInt& comp2) 
{
    return !(comp1 == comp2);
}

bool operator>=(const BigInt& comp1, const BigInt& comp2)
{
    if (comp1.sign_ > comp2.sign_)
        return true;
    else if (comp1.sign_ < comp2.sign_)
        return false;

    if (comp1.length_ == comp2.length_) {
        if (comp1.sign_ > 0) {
            for (int i = comp1.length_ - 1; i >= 0; i--)
                if (comp1.data_[i] > comp2.data_[i])
                    return true;
                else if (comp1.data_[i] < comp2.data_[i])
                    return false;
            return true;
        } else {
            for (int i = comp1.length_ - 1; i >= 0; i--)
                if (comp1.data_[i] < comp2.data_[i])
                    return true;
                else if (comp1.data_[i] > comp2.data_[i])
                    return false;
            return true;
        }
    } else {
        if (comp1.sign_ > 0)
            return comp1.length_ > comp2.length_;
        else 
            return comp1.length_ < comp2.length_;
    }
}

bool operator<=(const BigInt& comp1, const BigInt& comp2)
{
    if (comp1.sign_ < comp2.sign_)
        return true;
    else if (comp1.sign_ > comp2.sign_)
        return false;

    if (comp1.length_ == comp2.length_) {
        if (comp1.sign_ > 0) {
            for (int i = comp1.length_ - 1; i >= 0; i--)
                if (comp1.data_[i] < comp2.data_[i])
                    return true;
                else if (comp1.data_[i] > comp2.data_[i])
                    return false;
            return true;
        } else {
            for (int i = comp1.length_ - 1; i >= 0; i--)
                if (comp1.data_[i] > comp2.data_[i])
                    return true;
                else if (comp1.data_[i] < comp2.data_[i])
                    return false;
            return true;
        }
    } else {
        if (comp1.sign_ > 0)
            return comp1.length_ < comp2.length_;
        else 
            return comp1.length_ > comp2.length_;
    }
}

bool operator>(const BigInt& comp1, const BigInt& comp2)
{
    return !(comp1 <= comp2);
}

bool operator<(const BigInt& comp1, const BigInt& comp2)
{
    return !(comp1 >= comp2);
}

BigInt operator+(const BigInt& add1, const BigInt& add2)
{
    if (add1.sign_ == add2.sign_)
        if (add1.sign_ == 1)
            return BigInt::add(add1, add2);
        else
            return -(add1.absolute() + add2.absolute());
    else
        if (add1.sign_ == 1)
            return add1 - add2.absolute();
        else
            return add2 - add1.absolute();
}

BigInt operator-(const BigInt& sub1, const BigInt& sub2)
{
    if (sub1.sign_ == sub2.sign_)
        if (sub1.sign_ > 0)
            return BigInt::sub(sub1, sub2);
        else
            return -(sub1.absolute() - sub2.absolute());
    else
        if (sub1.sign_ > 0)
            return sub1 + sub2.absolute();
        else
            return -(sub2 + sub1.absolute());
}
