#include<iostream>
#include<cstring>
#include<cstdlib>
using namespace std;

class BigInt
{
    bool it_is_positiv = true;
    char* number_in_str;
    size_t size_number;
    friend ostream& operator<<(ostream& out, const BigInt& x);
    public:
    BigInt(int64_t i = 0)
    {
        if (i < 0)
        {
            it_is_positiv = false;
            i *= -1;
        }
        int64_t c = i;
        int k = 0;
        while (c > 0)
        {
            k++;
            c /= 10;
        }
        size_number = max(k, 1);
        number_in_str = new char[max(k + 1, 2)];
        sprintf (number_in_str, "%lld", i);
        number_in_str[max(k + 1, 2) - 1] = '\0';
    }
    BigInt(const BigInt& other): size_number(other.size_number), it_is_positiv(other.it_is_positiv)
    {
        number_in_str = new char [other.size_number + 1];
        copy(other.number_in_str, other.number_in_str + size_number + 1 , number_in_str);
    }
    ~BigInt()
    {
        delete []number_in_str;
    }
    BigInt& operator= (const BigInt& other)
    {
        if (this == &other)
            return *this;
        char* a = new char[other.size_number + 1];
        delete[] number_in_str;
        number_in_str = a;
        size_number = other.size_number;
        copy(other.number_in_str, other.number_in_str + size_number + 1, number_in_str);
        return *this;
    }
    bool operator> (const BigInt& other) const
    {
        if (size_number > other.size_number)
        {
            return it_is_positiv;
        }
        if (size_number < other.size_number)
        {
            return !other.it_is_positiv;
        }
        if ((it_is_positiv) && (other.it_is_positiv))
        {
            return strcmp(number_in_str, other.number_in_str) > 0;
        }
        if (it_is_positiv)
        {
            return true;
        }
        if (other.it_is_positiv)
        {
            return false;
        }
        return strcmp(number_in_str, other.number_in_str) < 0;
    }
    bool operator== (const BigInt& other) const
    {
        return (size_number == other.size_number) && (strcmp(number_in_str, other.number_in_str) == 0) && (it_is_positiv == other.it_is_positiv);
    }
    bool operator!= (const BigInt& other) const
    {
        return !(*this == other);
    }
    bool operator< (const BigInt& other) const
    {
        return !((*this > other) || (*this == other));
    }
    bool operator<= (const BigInt& other)
    {
        return !(*this > other);
    }
    bool operator>= (const BigInt& other)
    {
        return !(*this < other);
    }
    BigInt operator- () const
    {
        BigInt a(*this);
        if (a.number_in_str[0] != '0')
        {
            a.it_is_positiv = !it_is_positiv;
        }
        return a;
    }
    BigInt operator+(const BigInt& other) const
    {
        size_t Size = max(size_number, other.size_number);
        char *a = new char[Size + 1];
        char *b = new char[Size + 1];
        char *c;
        int in_brain = 0;
        size_t i;
        size_t Size2 = Size - size_number;
        bool is_pos;
        for (i = 0; i < Size2; i++)
        {
            a[i] = '0';
        }
        Size2 = strlen(number_in_str);
        for (size_t j = 0; j < Size2; j++)
        {
            a[i + j] = number_in_str[j];
        }
        Size2 = Size - other.size_number;
        for (i = 0; i < Size2; i++)
        {
            b[i] = '0';
        }
        Size2 = strlen(other.number_in_str);
        for (size_t j = 0; j < Size2; j++)
        {
            b[i + j] = other.number_in_str[j];
        }
        a[Size] = b[Size] = '\0';
        if (strcmp(a, b) < 0)
        {
            is_pos = other.it_is_positiv;
            c = a;
            a = b;
            b = c;
        }
        else
        {
            is_pos = it_is_positiv;
        }
        if (it_is_positiv == other.it_is_positiv)
        {
            int p;
            for (int j = Size - 1; j > -1; j--)
            {
                p = (a[j] + b[j] - '0' - '0' + in_brain);
                a[j] = p % 10 + '0';
                in_brain = p / 10;
            }
            if (in_brain == 1)
            {
                delete []b;
                b = new char[Size + 2];
                b[0] = 1 + '0';
                for (size_t j = 0; j < Size; j++)
                {
                    b[j + 1] = a[j];
                }
                b[Size + 1] = '\0';
                delete []a;
                a = b;
                Size++;
            }
        }
        else
        {
            int p;
            for (int j = Size - 1; j > -1; j--)
            {
                p = (a[j] - b[j] - in_brain);
                if (p < 0)
                {
                    p += 10;
                    in_brain = 1;
                }
                else
                {
                    in_brain = 0;
                }
                a[j] = p + '0';
            }
                delete []b;
        }
        BigInt New;
        delete []New.number_in_str;
        New.number_in_str = new char[Size + 1];
        c = a;
        Size2 = Size;
        while ((Size2 > 1) && (c[0] == '0'))
        {
            c++;
            Size2--;
        }
        copy(c, c + Size2 + 1, New.number_in_str);
        if (c[0] != '0')
        {
            New.it_is_positiv = is_pos;
        }
        else
        {
            New.it_is_positiv = true;
        }
        New.size_number = Size2;
        delete []a;
        return New;
    }
    BigInt operator-(const BigInt& other) const
    {
        return (*this) + (-other);
    }
};
ostream& operator<< (ostream& out, const BigInt& x)
{
    if (!x.it_is_positiv)
        out<<'-';
    out<<x.number_in_str;
    return out;
}
