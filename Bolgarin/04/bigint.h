#include <iostream>
#include <cstring>

class BigInt {
    using Size = size_t;
    Size capacity_ = 8;

    char* data_ = nullptr;
    Size length_ = 0;
    bool negative_ = false;

    void AddRight(const BigInt& number, BigInt& result) const {
        char extra = 0;

        for (Size i = 0; i < result.capacity_; i++) {
            if (i >= length_ && i >= number.length_ && extra == 0)
                break;

            char left = (i < length_) ? data_[i] : 0;
            char right = (i < number.length_) ? 
                        (number.data_[i] - extra / 10) : 0;

            extra = (right >= left) ? 0 : 10;
            result.data_[result.length_++] = 
                                    (right - left + extra) % 10;
        }
    }

    void AddLeft(const BigInt& number, BigInt& result) const {
        char extra = 0;

        for (Size i = 0; i < result.capacity_; i++) {
            if (i >= length_ && i >= number.length_ && extra == 0)
                break;

            char left = (i < length_) ? (data_[i] - extra / 10) : 0;
            char right = (i < number.length_) ? number.data_[i] : 0;

            extra = (left >= right) ? 0 : 10;
            result.data_[result.length_++] = 
                                    (left - right + extra) % 10;
        }
    }

    void UpdateSize() {
        while (data_[length_ - 1] == 0 && length_ > 1)
            length_--;

        if (length_ == 1 && data_[0] == 0)
            negative_ = false;
    }

public:
    BigInt() 
        : data_(new char[capacity_])
        , length_(1)
    {
        std::memset(data_, 0, capacity_ * sizeof(char));
    }


    BigInt(int64_t number) 
        : data_(new char[capacity_])
        , negative_(number < 0)
    {
        if (number == 0) 
            length_ = 1;
        
        number *= negative_ ? -1 : 1;

        std::memset(data_, 0, capacity_ * sizeof(char));

        while (number) {
            data_[length_++] = number % 10;
            number = number / 10;

            if (length_ == capacity_) {
                char* temp = data_;
                data_ = new char[2 * capacity_];
                std::memcpy(data_, temp, capacity_ * sizeof(char));
                capacity_ *= 2;

                delete[] temp;
            }
        }
    }

    BigInt(const BigInt& copied) 
        : length_(copied.length_)
        , capacity_(copied.capacity_)
        , negative_(copied.negative_)
    {
        if (data_ != nullptr)
            delete[] data_;

        data_ = new char[capacity_];
        std::memcpy(data_, copied.data_, capacity_ * sizeof(char));
    }

    ~BigInt() {
        delete[] data_;
    }

    BigInt& operator=(const BigInt& copied) {
        if (data_ != nullptr)
            delete[] data_;

        length_ = copied.length_;
        capacity_ = copied.capacity_;
        negative_ = copied.negative_;

        data_ = new char[capacity_];
        std::memcpy(data_, copied.data_, capacity_ * sizeof(char));

        return *this;
    }

    bool operator==(const BigInt& number) const {
        return length_ == number.length_ && 
               negative_ == number.negative_ && 
               !std::memcmp(data_, number.data_, length_ * sizeof(char));
    }

    bool operator!=(const BigInt& number) const {
        return !(*this == number);
    }

    bool operator<(const BigInt& number) const {
        if (negative_ == number.negative_) {
            if (length_ == number.length_) {

                int diff = 0;
                for (int i = length_ - 1; (!diff && i >= 0); i--)
                    diff = data_[i] - number.data_[i];

                return (diff != 0) && (diff < 0 ? !negative_ : negative_);

            } else {
                return (length_ < number.length_) ? !negative_ : negative_;
            }
        } else {
            return negative_;
        }
    }

    bool operator<=(const BigInt& number) const {
        return (*this < number) || (*this == number);
    }

    bool operator>(const BigInt& number) const {
        return !(*this <= number);
    }

    bool operator>=(const BigInt& number) const {
        return !(*this < number);
    }

    BigInt operator-() const {
        BigInt number(*this);
        if (number != 0)
            number.negative_ = !negative_;

        return number;
    }

    BigInt operator+(const BigInt& number) const {
        BigInt result;
        delete[] result.data_;

        result.capacity_ = std::max(capacity_, number.capacity_) + 1;
        result.length_ = 0;
        result.data_ = new char[result.capacity_];
        std::memset(result.data_, 0, result.capacity_ * sizeof(char));

        if (negative_ == number.negative_) {
            result.negative_ = negative_;
            char extra = 0;
            for (Size i = 0; i < result.capacity_; i++) {

                if (i >= length_ && i >= number.length_ && extra == 0)
                    break;

                char left = (i < length_) ? data_[i] : 0;
                char right = (i < number.length_) ? number.data_[i] : 0;
                char sum = (left + right + extra);

                result.data_[result.length_++] = sum % 10;
                extra = sum / 10;
            }

        } else if (negative_) {
            if (-*this < number) {
                result.negative_ = false;
                AddRight(number, result);
            } else {
                result.negative_ = true;
                AddLeft(number, result);
            }
        } else {
            if (*this <= -number) {
                result.negative_ = true;
                AddRight(number, result);
            } else {
                result.negative_ = false;
                AddLeft(number, result);
            }
        }

        result.UpdateSize();
        return result;
    }

    BigInt operator-(const BigInt& number) const {
        return *this + (-number);
    }

    friend std::ostream& operator<<(std::ostream& out, const BigInt& number) {
        if (number.negative_)
            out << "-";

        for (Size i = 0; i < number.length_; i++)
            out << (int)number.data_[number.length_ - i - 1];

        return out;
    }
};