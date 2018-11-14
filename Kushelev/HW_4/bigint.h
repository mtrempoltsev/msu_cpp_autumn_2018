#include<iostream>
#include<cstring>

using namespace std;

class BigInt
{
    int* data;
    bool is_neg = false;
    size_t capacity = 32;
    size_t _size = 0;
public:
    void print()
    {
        cout << "Capacity: " << capacity << endl;
        cout << "Size: " << _size << endl;
        cout << "Is negative: " << is_neg << endl;
        for (size_t i = 1; i <= _size; i++)
        {
            cout << data[_size - i];
        }
        cout << endl << endl;
    }

    void add_digit(int64_t x)
    {
        if (_size == capacity)
        {
            capacity *= 2;
            int* tmp = data;
            delete[] data;
            int* data = new int[capacity];
            memcpy(data, tmp, _size * sizeof(data));
            delete[] tmp;
        }
        data[_size] = x;
        _size++;
    }

    BigInt()
    {
        _size = 0;
        data = new int[capacity];
        add_digit(0);
    }

    BigInt(int64_t x)
    {
        if (x == 0)
        {
            data = new int[capacity];
            _size = 0;
            add_digit(0);
            return;
        }
        if (x < 0)
        {
            x *= -1;
            is_neg = true;
        }
        data = new int[capacity];
        while (x > 0)
        {
            add_digit(x % 10);
            x = x / 10;
        }
    }

    BigInt(const BigInt& x) : _size(x._size), capacity(x.capacity), is_neg(x.is_neg)
    {
        data = new int[capacity];
        memcpy(data, x.data, _size * sizeof(x.data));
    }

    BigInt& operator=(const BigInt& x)
    {
        if (this == &x)
            return *this;
        _size = x._size;
        capacity = x.capacity;
        is_neg = x.is_neg;
        data = new int[capacity];
        memcpy(data, x.data, _size * sizeof(x.data));
    }

    bool operator==(const BigInt& x) const
    {
        if (this == &x)
            return true;
        if ((x._size != _size) || (x.is_neg != is_neg))
            return false;
        for (size_t i = 0; i < _size; i++)
        {
            if (data[i] != x.data[i])
                return false;
        }
        return true;
    }

    bool operator!=(const BigInt& x) const
    {
        return !operator==(x);
    }

    bool operator<(const BigInt& x) const
    {
        if (is_neg != x.is_neg)
            return is_neg;
        if (_size > x._size)
            return is_neg;
        if (_size < x._size)
            return !is_neg;
        for (size_t i = 1; i <= _size; i++)
        {
            if (data[_size - i] > x.data[_size - i])
                return is_neg;
            if (data[_size - i] < x.data[_size - i])
                return !is_neg;
        }
        return false;
    }

    bool operator>(const BigInt& x) const
    {
        return !(operator<(x) || operator==(x));
    }

    bool operator<=(const BigInt& x) const
    {
        return !(operator>(x));
    }

    bool operator>=(const BigInt& x) const
    {
        return !(operator<(x));
    }

    BigInt operator-() const
    {
        BigInt x(*this);
        if (data[_size - 1] == 0)
            return x;
        x.is_neg = !(is_neg);
        return x;
    }

    BigInt operator+(const BigInt& x) const
    {
        int64_t ndig = 0;
        int64_t sum = 0;
        BigInt a(*this);
        BigInt b(x);
        if (a.is_neg && !b.is_neg)
            return b - a.operator-();
        if (!a.is_neg && b.is_neg)
            return a - b.operator-();
        while (a._size < b._size)
        {
            a.add_digit(0);
        }
        while (a._size > b._size)
        {
            b.add_digit(0);
        }
        if (a._size == b._size)
        {
            BigInt res(a);
            for (size_t i = 0; i < a._size; i++)
            {
                sum = a.data[i] + b.data[i] + ndig;
                res.data[i] = sum % 10;
                ndig = (sum - res.data[i]) / 10;
            }
            if (ndig != 0)
                res.add_digit(ndig);
            return res;
        }
    }

    BigInt operator-(const BigInt& a) const
    {
        if (is_neg != a.is_neg)
        {
            if (is_neg)
                return -(operator-() + a);
            else
                return *this + a.operator-();
        }

        if (is_neg) {
            return a.operator-() + *this;
        }
        if (*this < a) {
            return -(a - *this);
        }

        BigInt res(*this);
        char ndig = 0;

        for (size_t i = 0; i < a._size; i++) {
            char diff = res.data[i] - a.data[i] - ndig;

            if (diff < 0) {
                res.data[i] = diff + 10;
                ndig = 1;
            }
            else {
                res.data[i] = diff;
                ndig = 0;
            }
        }
        for (size_t i = a._size; i < res._size; i++) {
            char diff = res.data[i] - ndig;
            if (diff < 0) {
                res.data[i] = diff + 10;
                ndig = 1;
            }
            else {
                res.data[i] = diff;
                ndig = 0;
            }
        }
        for (size_t i = res._size - 1; i > 0; i--) {
            if (res.data[i] > 0) {
                break;
            }
            res._size--;
        }
        if (res._size == 1 && res.data[0] == 0) {
            res.is_neg = false;
        }
        return res;
    }

    bool get_sign() const
    {
        return is_neg;
    }

    size_t get_size() const
    {
        return _size;
    }

    int* get_data() const
    {
        return data;
    }

    ~BigInt()
    {
        delete[] data;
    }
};

std::ostream& operator<<(std::ostream& out, const BigInt& x)
{
    if (x.get_sign())
        out << '-';
    for (int i = 1; i <= x.get_size(); i++)
        out << x.get_data()[x.get_size() - i];
    return out;
}

