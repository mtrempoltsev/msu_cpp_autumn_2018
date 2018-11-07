#include <iostream>
#include <limits>
#include <cstring>


class BigInt {
public:
    char *data;
    signed char sign = 1;
    size_t length;

    BigInt(char *d, char s, size_t l) : sign(s), length(l) {
        data = static_cast<char *>(malloc(length + 1));
        strcpy(data, d);
        if (data[0] == '0')//потому что at line 280: -0 != 0.
            sign = 1;
    }

    void del_zero(char *string, char *s, size_t *len) const {
        int i = 0;
        for (; (string[i] == '0') && (string[i + 1] != '\0'); ++i) {};
        for (int j = 0; i < strlen(string) + 1; ++i, ++j)
            string[j] = string[i];
        *len = strlen(string);
    }

public:
    ~BigInt() {
        delete[] data;
    }

    BigInt() : sign(1), length(2) {
        data = new char[length];
        sprintf(data, "%i", 0);
    }

    BigInt(const int64_t init) {
        length = sizeof(int64_t);
        data = new char[length];
        if (init < 0)
            sign = -1;

        sprintf(data, "%li", init * sign);
        length = strlen(data);
    }

    BigInt(const BigInt &bi) {
        length = bi.length;
        data = bi.data;
        sign = bi.sign;
    }

    friend std::ostream &operator<<(std::ostream &os, const BigInt &bi) {
        if (bi.sign == -1)
            os << '-';
        os << bi.data;
        return os;
    }


    BigInt &operator=(const BigInt &copied) {
        if (this == &copied)
            return *this;
        char *ptr = new char[copied.length];
        delete[] data;
        strcpy(ptr, copied.data);
        data = ptr;
        length = copied.length;
        sign = copied.sign;
        return *this;
    }

    BigInt operator+(const BigInt &add2) const {
        char s;
        if (sign * add2.sign == 1)
            s = sign;
        else if (sign == -1)
            return add2 - (-(*this));
        else
            return *this - (-add2);

        if (add2.length > this->length)
            return add2 + *this;

        size_t tmp_len = length;
        char *tmp_data = new char[tmp_len + 1];
        strcpy(tmp_data, data);

        int shift = 0;
        size_t add1_last = length - 1;
        size_t add2_last = add2.length - 1;
        size_t i;

        for (i = tmp_len - 1; add2_last + 1 > 0; --i, --add1_last, --add2_last) {
            int sum = this->data[add1_last] - '0' + add2.data[add2_last] - '0' + shift;
            tmp_data[i] = static_cast<char>(sum % 10 + '0');
            shift = sum / 10;
        }
        if (shift != 0) {
            for (i; i + 1 > 0; --i)
                if (tmp_data[i] != '9') {
                    ++tmp_data[i];
                    break;
                } else
                    tmp_data[i] = '0';
            if (i + 1 == 0) {
                ++tmp_len;
                char *new_data = new char[tmp_len + 1];
                new_data[0] = '1';
                for (int i = 1; i <= tmp_len; ++i)
                    new_data[i] = tmp_data[i - 1];
                delete[] tmp_data;
                tmp_data = new_data;
            }
        }
        del_zero(tmp_data, &s, &tmp_len);
        return BigInt(tmp_data, s, tmp_len);
    }


    BigInt operator-(const BigInt &sub2) const {
        char s = 1;

        if (this->sign == -1)
            return -((-*this) + sub2);
        if (sub2.sign == -1)
            return *this + (-sub2);

        if (sub2.length > this->length)
            return -(sub2 - *this);

        size_t tmp_len = length;
        char *tmp_data = new char[tmp_len + 1];
        strcpy(tmp_data, data);

        int shift = 0;
        size_t sub1_last = length - 1;
        size_t sub2_last = sub2.length - 1;

        for (size_t i = tmp_len - 1; sub2_last + 1 > 0; --i, --sub1_last, --sub2_last) {
            int sub = this->data[sub1_last] - sub2.data[sub2_last] - shift;
            if (sub < 0) {
                if (i == 0) { return -(sub2 - *this); }
                sub += 10;
                shift = 1;
            } else
                shift = 0;

            tmp_data[i] = static_cast<char>(sub + '0');
        }
        if (shift != 0) {
            for (size_t i = sub1_last; i + 1 > 0; --i)
                if (tmp_data[i] != 0) {
                    --tmp_data[i];
                    shift = 0;
                    break;
                }
            if (shift != 0)
                s = -1;
        }
        del_zero(tmp_data, &s, &tmp_len);
        return BigInt(tmp_data, s, tmp_len);
    }

    BigInt operator-() const {
        return BigInt(data, -sign, length);
    }

    BigInt operator+() const {
        return BigInt(data, sign, length);
    }

    bool operator==(const BigInt &cmp2) const {
        BigInt tmp = *this - cmp2;
        for (size_t i = 0; i < tmp.length; ++i)
            if (tmp.data[i] != '0')
                return false;
        return true;
    }

    bool operator!=(const BigInt &cmp2) const {
        return !(*this == cmp2);
    }

    bool operator>(const BigInt &cmp2) const {
        return (*this - cmp2 - BigInt(1)).sign == 1;
    }

    bool operator<(const BigInt &cmp2) const {
        return (*this - cmp2).sign == -1;
    }

    bool operator>=(const BigInt &cmp2) const {
        return !(*this < cmp2);
    }

    bool operator<=(const BigInt &cmp2) const {
        return !(*this > cmp2);
    }

};
