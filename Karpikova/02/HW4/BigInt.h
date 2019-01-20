#include <iostream>
using namespace std;

class BigInt
{
    static constexpr size_t Size = 256;
    int8_t data_[Size];
    int curSize;
    bool sign = 0;
public:
    BigInt(long long a = 0)
		{
			if (a < 0) {
				a = -a;
				sign = 1;
			}
			if (a == 0) {
				data_[0] = 0;
			}
			int i = 0;
			while (a)
			{
				data_[i++] = a % 10;
				a /= 10;
			}
			curSize = !i ? 1 : i;
			for (int i = curSize; i < Size; ++i) {
				data_[i] = 0;
			}
		}

    bool operator==(const BigInt& other) const
    {
        if (this == &other) {
            return true;
        }
        if (sign != other.sign) {
            return false;
        }

        for (size_t i = 0; i < Size; ++i)
            if (data_[i] != other.data_[i])
                return false;

        return true;
    }

	bool operator!=(const BigInt& other) const
	{
		return !(*this == other);
  }

	BigInt operator-() const
	{
		BigInt tmp = *this;
		if (*this != 0) {
			tmp.sign = !sign;
		}
		return tmp;
  }

  BigInt operator+(const BigInt &other) const
	{
		if (other == 0) {
			return *this;
		}
		if (sign == other.sign) {
			if (curSize < other.curSize) {
				return other + *this;
			}
			BigInt res;
			res.sign = sign;
			int flag = 0;
			for (int i = 0; i <= curSize; ++i) {
      	res.data_[i] = data_[i] + other.data_[i] + flag;
        flag = res.data_[i] / 10;
        res.data_[i] %= 10;
			}
			res.curSize = curSize;
			if (res.data_[curSize])
				++res.curSize;
			return res;
		} else {
			return *this - (-other);
		}
  }

	BigInt operator-(const BigInt &other) const
	{
		if (other == 0) {
			return *this;
		}
		if (sign == other.sign) {
			if (*this == other) {
				return 0;
			}
			if (!sign && (*this < other)) {
				return -(other - *this);
			}
			if (sign && (*this > other))
        return -other + *this;
			BigInt res = 0;
			res.sign = sign;
			res.curSize = curSize;
			int flag = 0;
			for (int i = 0; i < curSize; ++i) {
        res.data_[i] = data_[i] - other.data_[i] + flag;
        if (res.data_[i] < 0) {
          flag = -1;
          res.data_[i] += 10;
        } else {
          flag = 0;
        }
			}
			while (!res.data_[res.curSize - 1] && res.curSize != 1) {
				--res.curSize;
			}
			return res;
		} else {
			return *this + (-other);
		}
	}

  bool operator<(const BigInt& other) const
  {
    if (*this == other) {
      return false;
    }
    if (sign != other.sign) {
      return (sign > other.sign);
    }
		if (curSize > other.curSize) {
			return sign;
		}
		if (curSize < other.curSize) {
			return !sign;
		}
    for (size_t i = curSize; i >= 0; --i) {
			if (data_[i] < other.data_[i]) {
				return !sign;
			}
			if (data_[i] > other.data_[i]) {
				return sign;
      }
    }
    return false;
  }


	bool operator>(const BigInt& other) const
	{
		if (*this == other) {
			return false;
		}
		return (other < *this);
	}

	bool operator>=(const BigInt& other) const
	{
		return !(*this < other);
	}

	bool operator<=(const BigInt& other) const
	{
		return !(*this > other);
	}

  friend ostream &operator<<(ostream&, const BigInt&);
};

ostream &operator<<(ostream& out, const BigInt& obj)
{
	if (obj.sign == 1)
		out << '-';
	for (int i = obj.curSize - 1; i >= 0; --i) {
		out << obj.data_[i] - 0;
	}
	return out;
}
