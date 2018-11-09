#include <iostream>
#include <cstring>

class BigInt
{
public:
    BigInt(int64_t val = 0);
    BigInt(const BigInt&);
    BigInt(BigInt&&);
    BigInt& operator=(const BigInt&);
    BigInt& operator=(BigInt&&);

    bool operator==(const BigInt&) const;
    bool operator>(const BigInt&) const;
    bool operator<(const BigInt&) const;
    bool operator!=(const BigInt&) const;
    bool operator<=(const BigInt&) const;
    bool operator>=(const BigInt&) const;

    BigInt operator-() const;
    BigInt operator+(const BigInt&) const;
    BigInt operator-(const BigInt&) const;

    ~BigInt();

    friend std::ostream& operator<<(std::ostream& os, const BigInt& bi);

private:
    struct
    {
        char isLarge;
        union
        {
            struct
            {
                uint8_t* data;
                uint64_t length;
                uint64_t capacity;
            } ldata; //large data
            struct
            {
                uint8_t data[sizeof(ldata) - 1];
                char length;
            } sdata; //small data
        } storage;
    } m_data;
    char m_neg;

    BigInt(uint64_t size, char neg);

    class BitWorker
    {
    public:
        BitWorker(uint64_t index, uint8_t* data, uint64_t size) :
                m_index(index), m_size(size), m_data(data)
        {
        }
        operator uint8_t() const;
        BitWorker& operator=(uint8_t val);
    private:
        uint64_t m_index;
        uint64_t m_size;
        uint8_t* m_data;
    };

    uint64_t size() const;
    void setSize(uint64_t);
    uint64_t capasity() const;

    uint8_t* data();
    const uint8_t* data() const;
    void reserve(uint64_t capasity);

    BitWorker operator[](uint64_t index);
    const BitWorker operator[](uint64_t index) const;

};

BigInt::BigInt(uint64_t size, char neg)
{
    m_neg = neg;
    m_data.isLarge = 0;
    setSize(size);
    memset(data(), 0, capasity());
}

BigInt::BigInt(int64_t val)
{
    m_data.isLarge = 0;
    memset(data(), 0, capasity());
    if (val < 0)
    {
        m_neg = -1;
        val = -val;
    } else
    {
        m_neg = 1;
    }
    if (val == 0)
    {
        setSize(1);
        this->operator[](0) = 0;
    } else
    {
        char bs = 0;
        while (val != 0)
        {
            this->operator[](bs) = val % 10;
            val /= 10;
            ++bs;
        }
        setSize(bs);
    }
}

BigInt::BigInt(const BigInt& bi)
{
    m_data.isLarge = 0;
    m_neg = bi.m_neg;
    if (capasity() < bi.size() / 2 + 1)
        reserve(bi.size() / 2 + 1);
    memcpy(data(), bi.data(), bi.size() / 2 + 1);
    setSize(bi.size());
}

BigInt::BigInt(BigInt&& bi)
{
    memcpy(this, &bi, sizeof(bi));
    if (bi.m_data.isLarge)
        bi.m_data.isLarge = 0;
}

BigInt& BigInt::operator=(const BigInt& bi)
{
    m_neg = bi.m_neg;
    if (capasity() < bi.size() / 2 + 1)
        reserve(bi.size() / 2 + 1);
    memcpy(data(), bi.data(), bi.size() / 2 + 1);
    setSize(bi.size());
    return *this;
}

BigInt& BigInt::operator=(BigInt&& bi)
{
    if (m_data.isLarge)
        delete[] m_data.storage.ldata.data;
    memcpy(this, &bi, sizeof(bi));
    if (bi.m_data.isLarge)
        bi.m_data.isLarge = 0;
    return *this;
}

uint64_t BigInt::size() const
{
    if (m_data.isLarge)
        return m_data.storage.ldata.length;
    else
        return m_data.storage.sdata.length;
}

void BigInt::setSize(uint64_t size)
{
    if (m_data.isLarge)
        m_data.storage.ldata.length = size;
    else
        m_data.storage.sdata.length = size;
    reserve(size / 2 + 1);
}

uint64_t BigInt::capasity() const
{
    if (m_data.isLarge)
        return m_data.storage.ldata.capacity;
    else
        return sizeof(m_data.storage.sdata.data);
}

uint8_t* BigInt::data()
{
    if (m_data.isLarge)
        return m_data.storage.ldata.data;
    else
        return m_data.storage.sdata.data;
}

const uint8_t* BigInt::data() const
{
    if (m_data.isLarge)
        return m_data.storage.ldata.data;
    else
        return m_data.storage.sdata.data;
}

void BigInt::reserve(uint64_t cap)
{
    uint64_t ownCap = capasity();
    if (ownCap < cap)
    {
        auto reserveSpace = ownCap;
        while (reserveSpace < cap)
            reserveSpace *= 2;
        uint8_t* dat = new uint8_t[reserveSpace];
        memset(dat, 0, reserveSpace);
        memcpy(dat, data(), ownCap);
        if (m_data.isLarge)
        {
            delete[] m_data.storage.ldata.data;
        } else
        {
            m_data.isLarge = 1;
            m_data.storage.ldata.length = m_data.storage.sdata.length;
        }
        m_data.storage.ldata.capacity = reserveSpace;
        m_data.storage.ldata.data = dat;
    }
}

BigInt::BitWorker& BigInt::BitWorker::operator =(uint8_t val)
{
    char clearMask = 0xF;
    if (!(m_index % 2))
        clearMask = ~clearMask;
    char mask = (val % 10) << ((m_index % 2) * 4);
    m_data[m_index / 2] &= clearMask;
    m_data[m_index / 2] |= mask;
    return *this;
}

BigInt::BitWorker::operator uint8_t() const
{
    if (m_index > m_size)
        return 0;
    char offset = ((m_index % 2) * 4);
    char mask = 15 << offset;
    return (m_data[m_index / 2] & mask) >> offset;
}

BigInt::BitWorker BigInt::operator[](uint64_t index)
{
    return BitWorker(index, data(), size());
}

const BigInt::BitWorker BigInt::operator[](uint64_t index) const
{
    return BitWorker(index, (uint8_t*) data(), size());
}

bool BigInt::operator==(const BigInt& bi) const
{
    return bi.m_neg == m_neg && size() == bi.size()
            && !memcmp(data(), bi.data(), size() / 2 + 1);

}

bool BigInt::operator>(const BigInt& bi) const
{
    int64_t s1 = size();
    int64_t s2 = bi.size();
    if (m_neg != bi.m_neg)
        return m_neg > bi.m_neg;
    if (s1 != s2)
        return m_neg * (s1 - s2) > 0;
    for (auto i = s1; i != 0; --i)
        if (operator[](i - 1) != bi[i - 1])
            return m_neg * (int8_t) (operator[](i - 1) - bi[i - 1]) > 0;
    return false;

}

bool BigInt::operator<(const BigInt& bi) const
{
    int64_t s1 = size();
    int64_t s2 = bi.size();
    if (m_neg != bi.m_neg)
        return m_neg < bi.m_neg;
    if (s1 != s2)
    {
        return m_neg * (s1 - s2) < 0;
    }
    for (auto i = s1; i != 0; --i)
        if (operator[](i - 1) != bi[i - 1])
            return m_neg * (int8_t) (operator[](i - 1) - bi[i - 1]) < 0;
    return false;
}

bool BigInt::operator!=(const BigInt& bi) const
{
    return !operator ==(bi);
}

bool BigInt::operator<=(const BigInt& bi) const
{
    return !operator>(bi);
}

bool BigInt::operator>=(const BigInt& bi) const
{
    return !operator<(bi);
}

BigInt BigInt::operator-() const
{
    BigInt ret(*this);
    if (ret != 0)
        ret.m_neg *= -1;
    return ret;
}

BigInt BigInt::operator+(const BigInt& bi) const
{
    uint64_t s1 = size();
    uint64_t s2 = bi.size();
    uint64_t maxS = (s1 > s2) ? s1 : s2;
    BigInt ret(maxS + 1, 1);
    if (m_neg * bi.m_neg > 0)
    {
        maxS++;
        ret.m_neg = m_neg;
        char flag = 0;
        for (uint64_t i = 0; i < maxS; ++i)
        {
            char a = operator[](i) + bi[i] + flag;
            flag = a / 10;
            a = a % 10;
            ret[i] = a;
        }
    } else
    {
        char flag = 0;
        char sign = 1;
        if ((m_neg > 0 && *this < -bi) || (bi.m_neg > 0 && -*this < bi))
            sign = -1;
        ret.m_neg = m_neg * sign;
        for (uint64_t i = 0; i < maxS; ++i)
        {
            char a = flag + operator[](i) * sign - bi[i] * sign;
            flag = (a < 0) ? -1 : 0;
            a += 10;
            ret[i] = a % 10;
        }
    }
    while (ret[maxS - 1] == 0 && maxS != 1)
        --maxS;
    ret.setSize(maxS);
    if (maxS == 1 && ret[0] == 0)
        ret.m_neg = 1;
    return ret;
}

BigInt BigInt::operator-(const BigInt& bi) const
{
    return BigInt(operator+(-bi));
}

BigInt::~BigInt()
{
    if (m_data.isLarge)
        delete[] m_data.storage.ldata.data;
}

std::ostream& operator<<(std::ostream& os, const BigInt& bi)
{
    if (bi.m_neg < 0)
        os << '-';
    for (auto i = bi.size(); i != 0; --i)
        os << (int) bi[i - 1];
    return os;
}
