#include "bigint.h"

size_t BigInt::start_size = 0;

BigInt::BigInt(int64_t x)
{
    if (this->start_size == 0)
    {
        for (int64_t lim = std::numeric_limits<int64_t>::max(); lim != 0; lim /= BigInt::BASE)
            start_size++;
        //how much we need + 1
        start_size++;
    }

    this->data_size = this->start_size;
    this->data = new int64_t[data_size];

    this->sign = x >= 0 ? PLUS : MINUS;

    int64_t x_abs = std::abs(x);

    size_t i = 0;
    for (int64_t x_cut = x_abs; x_cut != 0 || i == 0; x_cut /= BigInt::BASE)
        this->data[i++] = x_cut % BigInt::BASE;

    this->size = i;
}


BigInt::BigInt(const BigInt& copied)
    : data(new int64_t[copied.data_size]),
    data_size(copied.data_size), 
    size(copied.size), sign(copied.sign) 
{
    std::copy(copied.data, copied.data + this->size, this->data);
}


BigInt& BigInt::operator=(const BigInt& copied)
{
    if (this == &copied)
        return *this;

    int64_t* ptr = new int64_t[copied.data_size];
    delete[] this->data;
    this->data = ptr;
    this->data_size = copied.data_size;
    this->size = copied.size;
    this->sign = copied.sign;
    std::copy(copied.data, copied.data + this->size, this->data);
    return *this;
}


BigInt BigInt::operator+(const BigInt& b) const
{
    if (this->sign == b.sign)
        return sum(*this, b, this->sign);
    else
    {   
        bool compare = absLessCompare(*this, b);
        //abs(a) >= abs(b), a > 0, b < 0
        if (this->sign == PLUS && compare == false)
            return diff(*this, b, PLUS);
        //abs(a) >= abs(b), a < 0, b > 0
        else if (this->sign == MINUS && compare == false)
            return diff(*this, b, MINUS);
        //abs(a) < abs(b), a < 0, b > 0
        else if (this->sign == MINUS && compare == true)
            return diff(b, *this, PLUS);
        //abs(a) < abs(b), a > 0, b < 0
        else
            return diff(b, *this, MINUS);
    }
}


BigInt BigInt::operator-(const BigInt& b) const
{
    if (this->sign != b.sign)
        return sum(*this, b, this->sign);
    else
    {   
        bool compare = absLessCompare(*this, b);
        //abs(a) >= abs(b), a > 0, b > 0
        if (this->sign == PLUS && compare == false)
            return diff(*this, b, PLUS);
        //abs(a) >= abs(b), a < 0, b < 0
        else if (this->sign == MINUS && compare == false)
            return diff(*this, b, MINUS);
        //abs(a) < abs(b), a > 0, b > 0
        else if (this->sign == MINUS && compare == true)
            return diff(b, *this, PLUS);
        //abs(a) < abs(b), a < 0, b < 0
        else
            return diff(b, *this, MINUS);
    }
}


BigInt BigInt::operator-() const
{
    BigInt ans = *this;

    if (ans.sign == PLUS)
        ans.sign = MINUS;
    else
        ans.sign = PLUS;

    return ans;
}


bool BigInt::operator==(const BigInt& b) const
{
    if (this == &b)
        return true;

    if (this->size != b.size)
        return false;

    //compare zeros; 0 == -0
    if (this->size == 1 && this->data[0] == 0 && b.data[0] == 0)
        return true;

    if (this->sign != b.sign)
        return false;

    return memcmp(this->data, b.data, this->size * sizeof(int64_t)) == 0;
}


bool BigInt::operator<(const BigInt& b) const
{
    if (this == &b)
        return false;

    if (this->sign == b.sign)
    {
        if (this->sign == PLUS)
            return absLessCompare(*this, b);
        else
            return absLessCompare(b, *this);
    }
    else
    {
        if (this->sign == PLUS)
            return false;
        else
            return true;
    }
}


std::ostream& operator<<(std::ostream& stream, const BigInt& bigint)
{
    //zero case: -0 output as 0
    if (bigint.size == 1 && bigint.data[0] == 0)
    {
        stream << 0;
        return stream;
    }

    if (bigint.sign == BigInt::MINUS)
        stream << '-';

    stream << bigint.data[bigint.size - 1];
    for (size_t i = bigint.size - 1; i > 0; --i)
        for (size_t j = BigInt::BASE / 10; j != 0; j /= 10)
            stream << bigint.data[i - 1] / j % 10;

    return stream;
}


BigInt::BigInt(size_t _data_size, sign_type _sign) : 
    data(new int64_t[_data_size]), data_size(_data_size), 
    size(0), sign(_sign) {}


BigInt BigInt::sum(const BigInt& a, const BigInt& b, sign_type sign) const
{
    size_t ans_data_size = std::max(a.data_size, b.data_size);
    //if answer can overfill original size
    if (a.size == ans_data_size || b.size == ans_data_size)
        ans_data_size *= 2;

    BigInt res(ans_data_size, sign);

    size_t i;
    int64_t carry = 0;

    for (i = 0; i < std::min(a.size, b.size); ++i)
    {       
        res.data[i] = (a.data[i] + b.data[i] + carry) % BigInt::BASE;
        carry = (a.data[i] + b.data[i] + carry) / BigInt::BASE;
    }


    for (; i < a.size; ++i)
    {       
        res.data[i] = (a.data[i] + carry) % BigInt::BASE;
        carry = (a.data[i] + carry) / BigInt::BASE;
    }
        
    for (; i < b.size; ++i)
    {       
        res.data[i] = (b.data[i] + carry) % BigInt::BASE;
        carry = (b.data[i] + carry) / BigInt::BASE;
    }

    if (carry == 1)
    {
        res.data[i] = carry;
        ++i;        
    }


    res.size = i;

    return res;
}


BigInt BigInt::diff(const BigInt& a, const BigInt& b, sign_type sign) const
{
    //implying abs(a) >= abs(b)
    size_t ans_data_size = std::max(a.data_size, b.data_size);
    //answer can't overfill original size
    BigInt res(ans_data_size, sign);

    size_t i;
    int64_t carry = 0;

    for (i = 0; i < std::min(a.size, b.size); ++i)
    {   
        int64_t diff = a.data[i] - b.data[i] + carry;

        if (diff < 0)
        {
            carry = -1;
            res.data[i] = diff + BigInt::BASE;    
        }

        else
        {
            carry = 0;
            res.data[i] = diff;            
        }
    }

    for (; i < a.size; ++i)
    {   
        int64_t diff = a.data[i] + carry;
        if (diff < 0)
        {
            carry = -1;
            res.data[i] = diff + BigInt::BASE;            
        }

        else
        {
            carry = 0;
            res.data[i] = diff;            
        }

    }

    //"deleting" zeroes in the beginning
    for (; (i > 0) && (res.data[i - 1] == 0); i--);

    res.size = i > 0 ? i : 1;

    return res;
}


bool BigInt::absLessCompare(const BigInt& a, const BigInt& b) const
{
    //return abs(a) < abs(b)
    if (a.size < b.size)
        return true;
    else if (a.size > b.size)
        return false;
    else
    {
        for (size_t i = a.size; i > 0; --i)
        {
            if (a.data[i - 1] < b.data[i - 1])
                return true;
            else if (a.data[i - 1] > b.data[i - 1])
                return false;
        }

        //abs(a) == abs(b)
        return false;
    }
}