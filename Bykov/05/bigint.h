#include <iostream>

void itoa(int64_t val, char *str)
{
    while(val > 0)
    {
        *str = val % 10 + '0';
        str++;
        val /= 10;
    }
}

int max(int a, int b)
{
    if(a > b)
        return a;
    return b;
}

class BigInt
{
    int sign;
    int size = 0;
    char *num = NULL;
public:
    BigInt()
    {
        size = 1;
        sign = 1;
        num = new char('0');
    }
    BigInt(int64_t val);
    BigInt(int sizs, char *str, int signs, bool ncopystr=1)
    {
        size = sizs;
        if(ncopystr)
            num = str;
        else
        {
            num = new char[size];
            for(int i = 0; i < size; i++)
                num[i] = str[i];
        }
        sign = signs;
    }
    BigInt operator=(const BigInt& val);
    BigInt(const BigInt &val);
    BigInt operator-()const
    {
        BigInt b = *this;
        b.sign *= -1;
        return b;
    }
    bool operator>(const BigInt& val)const;
    bool operator<(const BigInt& val)const
    {
        return val > *this;
    }
    bool operator!=(const BigInt& val)const
    {
        return *this > val || val > *this;
    }
    bool operator==(const BigInt& val)const
    {
        return !(*this != val);
    }
    bool operator>=(const BigInt& val)const
    {
        return !(*this < val);
    }
    bool operator<=(const BigInt& val)const
    {
        return !(*this > val);
    }
    friend std::ostream& operator<<(std::ostream& os, const BigInt& bi);
    BigInt operator+(const BigInt &val)const;
    BigInt operator-(const BigInt &val)const
    {
        return *this + BigInt(val.size, val.num, -val.sign, 0);
    }
    ~BigInt()
    {
        delete []num;
    }
};

std::ostream& operator<<(std::ostream& os, const BigInt& bi)
{
    bool fl = (bi.sign == -1);
    if((bi.size == 1) && (bi.num[0] == '0'))
    {
        os << "0\0";
        return os;
    }
    char *out = new char[bi.size + fl + 1];
    out[bi.size + fl] = '\0';
    if(fl)
        out[0] = '-';
    for(int i = 0; i < bi.size; i++)
        out[i + fl] = bi.num[bi.size - i -1];
    os << out;
    delete []out;
}

bool BigInt::operator>(const BigInt& val)const
{
    if((size == 1) && (val.size == 1) && (num[0] == '0') && (val.num[0] == '0'))
        return 0;
    if(sign != val.sign)
        return sign == 1;
    int sig = sign;
    if(size != val.size)
        return (size > val.size + (sig == -1)) % 2;
    int i = 0;
    while(i < size)
        if(num[size - i - 1] != val.num[size - i - 1])
        {
            return (num[size - i - 1] > val.num[size - i - 1]) != (sig == -1);
        }
        else
            i++;
    return 0;
}

BigInt BigInt::operator+(const BigInt &val)const
{
    if(sign == val.sign)
    {
        int s;
        char *str = new char[s = max(size, val.size) + 1];
        bool car  = 0;
        int i = 0;
        for(; i < size || i < val.size; i++)
        {
            int sum = (i < size ? num[i] : '0') + (i < val.size ? val.num[i] : '0') + car - 2 * '0';
            car =  sum / 10;
            str[i] = sum % 10 + '0';
        }
        if(car != 0)
            str[i] = '0' + car;
        else
            s--;
        char *str2 = new char[s];
        for(int i = 0; i < s; i++)
            str2[i] = str[i];
        delete []str;
        return BigInt(s, str2, sign);
    }
    else
    {
        int s;
        char *str = new char[s = max(size, val.size)];
        bool car  = 0, b, b1 = false;
        int i = 0, grs, lss;
        char *gr, *ls;
        b = (sign < 0);
        if(BigInt(size, num, 1, 0) >= BigInt(val.size, val.num, 1, 0))
        {
            gr = num;
            grs = size;
            ls = val.num;
            lss = val.size;
            b1 = true;
        }
        else
        {
            gr = val.num;
            ls = num;
            lss = size;
            grs = val.size;
        }
        for(; i < grs; i++)
        {
            int sum = gr[i]  - (i < lss ? ls[i]  : '0') - car;
            car =  sum < 0;
            str[i] = (sum + 10) % 10 + '0';
        }
        int nnull = s - 1;
        while(str[nnull] == '0')
            if(nnull > 0)
                nnull--;
            else
                break;
        char *str2 = new char[nnull + 1];
        for(int i = 0; i < nnull + 1; i++)
            str2[i] = str[i];
        delete []str;
        return BigInt(nnull+1, str2, 2 * (int)(b != b1) - 1);
    }
}

BigInt BigInt::operator=(const BigInt &val)
{
    if(this == &val)
        return *this;
    if(num != NULL)
        delete []num;
    size = val.size;
    sign = val.sign;
    num = new char[size];
    for(int i = 0; i < size; i++)
        num[i] = val.num[i];
    return *this;
}

BigInt::BigInt(const BigInt &val)
{
    size = val.size;
    sign = val.sign;
    num = new char[size];
    for(int i = 0; i < size; i++)
        num[i] = val.num[i];
}

BigInt::BigInt(int64_t val)
{
    if(val < 0)
    {
        sign = -1;
        val *= -1;
    }
    else
        sign = 1;
    if(val == 0)
    {
        size = 1;
        num = new char('0');
        return;
    }
    size = 0;
    int64_t nnum = val;
    while(nnum > 0)
    {
        size++;
        nnum /= 10;
    }
    num = new char[size];
    itoa(val, num);
}
