#include <iostream>


class BigInt {
private:
    char *_data;
    bool _sign = false;
    size_t index = 0;
    size_t length = 64;

    void push_back(char element);

public:
    BigInt();

    BigInt(const BigInt &a);

    BigInt(int64_t a);

    ~BigInt();

    BigInt operator-() const;

    BigInt abs() const;

    BigInt &operator=(const BigInt &a);

    BigInt operator+(const BigInt &a) const;

    BigInt operator-(const BigInt &a) const;

    bool operator==(const BigInt &a) const;

    bool operator!=(const BigInt &a) const;

    bool operator<(const BigInt &a) const;

    bool operator>(const BigInt &a) const;

    bool operator<=(const BigInt &a) const;

    bool operator>=(const BigInt &a) const;

    friend std::ostream &operator<<(std::ostream &out, const BigInt &a);
};

BigInt::BigInt() {
    _data = new char[length];
    push_back(0);
}

BigInt::BigInt(int64_t a) {
    if (a < 0) {
        _sign = true;
        a = a * -1;
    }

    _data = new char[length];

    if (a == 0) {
        push_back(0);
        return;
    }

    for (size_t i = 0; a > 0; i++) {
        push_back(a % 10);
        a = (a - a % 10) / 10;
    }
}

BigInt::BigInt(const BigInt &a) : index(a.index), _sign(a._sign), length(a.length) {
    _data = new char[length];
    for (size_t i = 0; i < length; i++) {
        _data[i] = a._data[i];
    }
}

void BigInt::push_back(char a) {
    _data[index] = a;
    ++index;
    if (length == index) {
        length = length * 2;
        char *tmp = new char[length];

        for (size_t i = 0; i < length; i++) {
            tmp[i] = _data[i];
        }

        delete[] _data;
        _data = tmp;
    }
}

BigInt &BigInt::operator=(const BigInt &a) {
    if (this == &a) {
        return *this;
    }

    index = a.index;
    length = a.length;
    _sign = a._sign;

    delete[] _data;
    _data = new char[length];
    memcpy(_data, a._data, index);
    return *this;
}

BigInt::~BigInt() {
    delete[] _data;
}

std::ostream &operator<<(std::ostream &out, const BigInt &a) {

    if (a._sign) {
        out << '-';
    }

    for (int i = a.index - 1; i != -1; i--) {
        out << char('0' + a._data[i]);
    }
    return out;
}

bool BigInt::operator==(const BigInt &a) const {
    if (index != a.index || _sign != a._sign) {
        return false;
    }

    for (size_t i = 0; i < index; i++) {
        if (_data[i] != a._data[i]) {
            return false;
        }
    }

    return true;
}

bool BigInt::operator!=(const BigInt &a) const {
    return !operator==(a);
}

bool BigInt::operator<(const BigInt &a) const {
    if (_sign != a._sign) {
        return _sign;
    }

    if (index > a.index) {
        return _sign;
    } else if (index < a.index) {
        return !_sign;
    } else {
        for (int i = index - 1; i != -1; i--) {
            if (_data[i] > a._data[i]) {
                return _sign;
            } else if (_data[i] < a._data[i]) {
                return !_sign;
            }
        }
        return false;
    }
}

bool BigInt::operator>(const BigInt &a) const {
    return !operator<(a) && operator!=(a);
}

bool BigInt::operator<=(const BigInt &a) const {
    return !operator>(a);
}

bool BigInt::operator>=(const BigInt &a) const {
    return !operator<(a);
}

BigInt BigInt::operator+(const BigInt &a) const {
    BigInt answer(*this);
    char tmp = 0;

    if (a._sign && !_sign) {
        return (*this) - a.abs();
    } else if (!a._sign && _sign) {
        return a.abs() - (*this).abs();
    }

    if (a.index == answer.index) {
        for (size_t i = 0; i < a.index; i++) {
            char sum = answer._data[i] + a._data[i] + tmp;
            answer._data[i] = sum % 10;
            tmp = (sum - sum % 10) / 10;
        }
        if (tmp != 0) {
            answer.push_back(tmp);
        }
    } else if (a.index > answer.index) {
        for (size_t i = answer.index; i < a.index; i++) {
            answer.push_back(0);
        }

        for (size_t i = 0; i < a.index; i++) {
            char sum = answer._data[i] + a._data[i] + tmp;
            answer._data[i] = sum % 10;
            tmp = (sum - sum % 10) / 10;
        }
        if (tmp != 0) {
            answer.push_back(tmp);
        }
    } else if (a.index < answer.index) {
        for (size_t i = 0; i < a.index; i++) {
            char sum = answer._data[i] + a._data[i] + tmp;
            answer._data[i] = sum % 10;
            tmp = (sum - sum % 10) / 10;
        }
        for (size_t i = a.index; i < answer.index; i++) {
            char sum = answer._data[i] + tmp;
            answer._data[i] = sum % 10;
            tmp = sum / 10;
        }
        if (tmp != 0) {
            answer.push_back(tmp);
        }
    }

    for (size_t i = answer.index - 1; i > 0; i--) {
        if (answer._data[i] > 0) {
            break;
        }
        answer.index--;
    }

    if (answer.index == 1 && answer._data[0] == 0) {
        answer._sign = false;
    }

    return answer;
}

BigInt BigInt::operator-() const {
    BigInt answer(*this);

    if (index == 1 && _data[0] == 0) {
        return answer;
    }

    answer._sign = !(_sign);

    return answer;
}

BigInt BigInt::abs() const {
    BigInt answer(*this);
    answer._sign = false;

    return answer;
}

BigInt BigInt::operator-(const BigInt &a) const {

    if (_sign != a._sign) {
        if (_sign) {
            return -(abs() + a);
        } else {
            return *this + a.abs();
        }
    }

    if (_sign) {
        return a.abs() + *this;
    }
    if (*this < a) {
        return -(a - *this);
    }

    BigInt answer(*this);
    char tmp = 0;

    for (size_t i = 0; i < a.index; i++) {
        char diff = answer._data[i] - a._data[i] - tmp;

        if (diff < 0) {
            answer._data[i] = diff + 10;
            tmp = 1;
        } else {
            answer._data[i] = diff;
            tmp = 0;
        }
    }

    for (size_t i = a.index; i < answer.index; i++) {
        char diff = answer._data[i] - tmp;
        if (diff < 0) {
            answer._data[i] = diff + 10;
            tmp = 1;
        } else {
            answer._data[i] = diff;
            tmp = 0;
        }
    }

    for (size_t i = answer.index - 1; i > 0; i--) {
        if (answer._data[i] > 0) {
            break;
        }
        answer.index--;
    }

    if (answer.index == 1 && answer._data[0] == 0) {
        answer._sign = false;
    }

    return answer;
}