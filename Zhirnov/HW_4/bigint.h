class BigInt
{
    int Size;
    bool sign;
    char *innerData;
    
    BigInt addZero(int value) const
    {
        char *data = new char[value + Size];

        for (int i = 0; i < value; i++)
            data[i] = 0;

        std::copy(innerData, innerData + Size, data + value);

        return BigInt(data, value + Size, sign);
    }

    BigInt MultInt( int number ) const
    {
        int size = Size + 1;
        char* multData = new char[size];

        for (int i = 0; i < size; i++)
            multData[i] = 0;

        int discharge = 0;

        for (int i = 0, tmp = 0; i < Size; i++)
        {
            tmp = innerData[i] * number + discharge;
            if (tmp > 9)
            {
                multData[i] = tmp % 10;
                discharge = tmp / 10;
            }
            else
            {
                multData[i] = tmp;
                discharge = 0;
            }
        }

        if (discharge)
            multData[size - 1] = discharge;
        else
            size--;

        return BigInt(multData, size, 0);
    }

public:
    BigInt()
        :Size(1)
        ,sign(false)
        ,innerData(new char[1])
    {
        innerData[0] = 0;
    }

    BigInt(int64_t number)
        :Size(1)
        ,sign(false)
        ,innerData(nullptr)
    {
        int64_t tmp;

        if (number < 0)
        {
            sign = true;
            number *= -1;
        }

        tmp = number;
        for (tmp = number; tmp > 9; tmp /= 10)
            Size++;

        innerData = new char[Size];

        tmp = number;
        for (int j = 0; tmp > 0; j++)
        {
            innerData[j] = tmp % 10;
            tmp /= 10;
        }

        if (number == 0)
            innerData[0] = 0;
    }

    BigInt operator-()
    {
        return BigInt(innerData, Size, !sign);
    }

    BigInt(BigInt &&elem)
        : Size(elem.Size)
        , sign(elem.sign)
        , innerData(elem.innerData)
    {
        elem.innerData = nullptr;
        elem.Size = 0;
    }

    BigInt(const BigInt &elem)
        : Size(elem.Size)
        , sign(elem.sign)
        , innerData(nullptr)
    {
        innerData = new char[Size];
        std::copy(elem.innerData, elem.innerData + elem.Size, innerData);
    }

    BigInt(const char* data, int length, bool minus)
        : Size(length)
        , sign(minus)
        , innerData(nullptr)
    {
        innerData = new char[Size];
        std::copy(data, data + Size, innerData);
    }

    ~BigInt()
    {
        delete[] innerData;
    }

    BigInt& operator=(BigInt &elem)
    {
        if (&elem == this)
            return *this;

        Size = elem.Size;
        sign = elem.sign;

        delete[] innerData;

        innerData = new char[Size];
        std::copy(elem.innerData, elem.innerData + elem.Size, innerData);

        return *this;
    }

    BigInt& operator=(BigInt &&elem)
    {
        delete[] innerData;

        Size = elem.Size;
        sign = elem.sign;
        innerData = elem.innerData;

        elem.innerData = nullptr;
        elem.Size = 0;

        return *this;
    }

    BigInt operator-(const BigInt& elem) const
    {
        bool minus = false;

        if (sign != elem.sign)
            return ((*this) + BigInt(elem.innerData, elem.Size, sign));

        int maxSize = Size > elem.Size ? Size : elem.Size;
        int minSize = Size < elem.Size ? Size : elem.Size;

        int subSize = maxSize;
        char* subData = new char[subSize];

        BigInt tmp1 = *this;
        tmp1.sign = false;

        BigInt tmp2 = elem;
        tmp2.sign = false;


        for (int i = 0; i < subSize; i++)
            subData[i] = 0;

        char *ptr1, *ptr2;
        if (tmp1 < tmp2)
        {
            ptr1 = innerData;
            ptr2 = elem.innerData;
            minus = !elem.sign;
        }
        else
        {
            ptr1 = elem.innerData;
            ptr2 = innerData;
            minus = sign;
        }

        for (int i = 0; i < minSize; i++)
            subData[i] = ptr1[i];

        bool discharge = false;

        for (int i = 0, tmp = 0; i < subSize; i++)
        {
            tmp = ptr2[i] - subData[i] - discharge;
            if (tmp < 0)
            {
                subData[i] = tmp + 10;
                discharge = true;
            }
            else
            {
                subData[i] = tmp;
                discharge = false;
            }
        }

        while ((subData[subSize - 1] == 0) && (subSize > 1))
            subSize--;

        if ((subSize == 1) && (subData[0] == 0))
            minus = 0;

        return BigInt(subData, subSize, minus);
    }

    bool operator>(const BigInt &elem) const
    {
        if ((*this) == elem)
            return false;

        if ((sign) && !(elem.sign))
            return false;

        if (!(sign) && (elem.sign))
            return true;

        if ((sign) && (elem.sign))
            return ((*this) * (-1)) < elem * (-1);

        if (Size > elem.Size)
            return true;
        if (Size < elem.Size)
            return false;

        for (int i = Size - 1; i >= 0; i--)
        {
            if (innerData[i] > elem.innerData[i])
                return true;
            if (innerData[i] < elem.innerData[i])
                return false;
        }


        return false;
    }

    bool operator>=(const BigInt &elem) const
    {
        return ((*this) > elem) || ((*this) == elem);
    }

    bool operator<(const BigInt &elem) const
    {
        return !((*this) >= elem);
    }

    bool operator<=(const BigInt &elem) const
    {
        return !((*this) > elem);
    }

    bool operator==(const BigInt &elem) const
    {
        if (Size != elem.Size)
            return false;

        if ((Size == 1) && (innerData[0] == 0) && (elem.innerData[0] == 0))
            return true;

        if (sign != elem.sign)
            return false;

        for (int i = 0; i < Size; i++)
            if (innerData[i] != elem.innerData[i])
                return false;

        return true;
    }

    bool operator!=(const BigInt &elem) const
    {
        return (!(*this == elem));
    }

    BigInt operator+(const BigInt &elem) const
    {
        bool minus = false;

        if (sign == elem.sign)
            minus = sign;
        else
        {
            if ((*this) > elem)
                return (*this - (elem * (-1)));
            else
                return (elem - ((*this) * (-1)));
        }

        int minSize = Size < elem.Size ? Size : elem.Size;
        int maxSize = Size > elem.Size ? Size : elem.Size;

        int sumSize = maxSize + 1;
        char* sumData = new char[sumSize];

        for (int i = 0; i < sumSize; i++)
            sumData[i] = 0;

        char *ptr1, *ptr2;
        if (Size < elem.Size)
        {
            ptr1 = innerData;
            ptr2 = elem.innerData;
        }
        else
        {
            ptr1 = elem.innerData;
            ptr2 = innerData;
        }

        for (int j = 0; j < minSize; j++)
            sumData[j] = ptr1[j];

        bool discharge = false;

        for (int j = 0, tmp = 0; j < sumSize - 1; j++)
        {
            tmp = sumData[j] + ptr2[j] + discharge;
            if (tmp > 9)
            {
                sumData[j] = tmp - 10;
                discharge = 1;
            }
            else
            {
                sumData[j] = tmp;
                discharge = 0;
            }
        }

        if (discharge)
            sumData[sumSize - 1] = 1;
        else
            sumSize--;

        return BigInt(sumData, sumSize, minus);
    }

    BigInt operator/(const BigInt &elem) const
    {
        BigInt dividend(*this);
        dividend.sign = false;

        BigInt divider = elem;
        divider.sign = false;

        int currentPos = 0, digit = 0, preSize;
        int x = 0, y = 0;

        if (*this == 0)
            return 0;

        if (elem == 0)
            throw "Divizion by zero!!!";

        char *quotientData = new char[Size];

        for (int i = 0; i < Size; i++)
            quotientData[i] = 0;

        BigInt quotient(quotientData, Size, sign xor elem.sign);

        if (dividend < divider)
            quotient.innerData[++currentPos] = 0;
        else
            while (dividend.Size > 0)
            {
                if (dividend < divider)
                {
                    for (int i = 0; i < y; i++)
                        quotient.innerData[++currentPos] = 0;
                    break;
                }

                preSize = dividend.Size;

                char *ptrBegin = dividend.innerData + dividend.Size - divider.Size;

                BigInt tmp(ptrBegin, divider.Size, 0);

                if (tmp < divider) {
                    if (divider.Size == dividend.Size)
                    {
                        quotientData[currentPos] = 0;
                        currentPos++;
                        break;
                    }
                    else
                    {
                        ptrBegin--;
                        tmp = BigInt(ptrBegin, divider.Size + 1, 0);
                    }
                }

                x = y - 1 - (dividend.Size - tmp.Size);
                for (int i = 0; i < x; i++)
                {
                    quotient.innerData[currentPos] = 0;
                    currentPos++;
                }

                for (int j = 1; j <= 10; j++)
                    if (divider * j > tmp)
                    {
                        digit = --j;
                        break;
                    }

                dividend = dividend - ((divider * digit).addZero(dividend.Size - tmp.Size));

                quotient.innerData[currentPos] = digit;
                currentPos++;

                if ((dividend == 0) && (preSize - (divider * digit).Size != 0))
                {
                    for (int i = 0; i < preSize - (divider * digit).Size; i++)
                    {
                        quotient.innerData[currentPos] = 0;
                        currentPos++;
                    }
                    break;
                }


                if ((dividend < divider) && (tmp.Size == preSize))
                    break;

                y = preSize - tmp.Size;
            }
        quotient.Size = currentPos;

        for (int i = 0; i < quotient.Size / 2; i++)
            std::swap(quotient.innerData[i], quotient.innerData[quotient.Size - i - 1]);

        return quotient;
    }

    BigInt operator*(const BigInt &elem) const
    {
        int multSize = Size + elem.Size;
        bool minus = sign xor elem.sign;
        char* multData = new char[multSize];

        if ((*this == 0) || (elem == 0))
            return BigInt(0);

        BigInt tmp1 = *this;
        tmp1.sign = false;

        BigInt tmp2 = elem;
        tmp2.sign = false;

        for (int i = 0; i < multSize; i++)
            multData[i] = 0;

        BigInt mult(multData, multSize, 0);

        for (int i = 0; i < tmp2.Size; i++)
            mult = mult + (tmp1.MultInt(tmp2.innerData[i])).addZero(i);

        if (mult.innerData[mult.Size - 1] == 0)
            mult.Size--;

        mult.sign = minus;

        return mult;
    }

    friend std::ostream& operator<<(std::ostream& out, const BigInt &elem);
};

std::ostream& operator<<(std::ostream& os, const BigInt &elem)
{
    if (elem.sign)
        os << '-';
    for (int j = elem.Size - 1; j >= 0; j--)
        os << char(elem.innerData[j] + '0');
    return os;
}
