#define BS 1000000000
#include <iostream> 
#include <cstdint>
#include <cmath>
class BigInt
{
private:
	static const int BASE = BS;
	int size;
	bool sign;
	int* Biginteger;
public:
	BigInt(): sign(0), size(0), Biginteger(new int)
	{
		Biginteger[0] = 0;
	}
	BigInt(bool num, int size): sign(0), size(size), Biginteger(new int[size])
	{
		for (int i = 0; i < size; ++i)
			Biginteger[i] = num;
	}
	template <typename T>
	BigInt(const T n): size(0)
	{
		T arg = n;
		if (n >= 0)
			sign = 0;
		else
			sign = 1;
		size = sizeof(n)/4 + 1;
		Biginteger = new int [size];
		for(int i = 0; i < size; ++i)
		{
			Biginteger[i] =  arg % BASE;
			arg = (arg - arg % BASE) / BASE;
		}
	}
	BigInt(const BigInt& copy)
	{
		Biginteger = new int[copy.size];
		size = copy.size;
		sign = copy.sign;
		memcpy(Biginteger, copy.Biginteger, sizeof(int) * size);

	}
	int getSize()
	{
		return Biginteger[0];
	}
	int getSign()
	{
		return sign;
	}
	friend std::ostream& operator<<(std::ostream& os, const BigInt& BI);
	BigInt operator +(const BigInt& copy) const
	{
		int sum;
		BigInt MAX_Int = (*this).Big_Copy(size);
		BigInt MIN_Int = copy.Big_Copy(size);
		if (copy.size > size)
		{
			MAX_Int = copy.Big_Copy(copy.size);
			MIN_Int = (*this).Big_Copy(size);
		}
		BigInt Temp = MIN_Int.Big_Copy(MAX_Int.size);
		for (int i = 0; i < MAX_Int.size - 1; ++i)
		{
			sum = Temp.Biginteger[i] + MAX_Int.Biginteger[i];
			if (NoNone(Temp, i) && sum < 0)//NoNone(Temp, i))
			{
				Temp.Biginteger[i] = (sum % BASE) + BASE;
				Temp.Biginteger[i + 1] += (sum - Temp.Biginteger[i] % BASE) / BASE;
			}
			else
			{
				Temp.Biginteger[i] = sum % BASE;
				Temp.Biginteger[i + 1] += (sum - Temp.Biginteger[i]) / BASE;
			}
		}
		Temp.Biginteger[MAX_Int.size - 1] += MAX_Int.Biginteger[MAX_Int.size - 1];
		if (Temp.Biginteger[0] >= 0)
			Temp.sign = 0;
		else
			Temp.sign = 1;
		if (Temp.Biginteger[MAX_Int.size - 1] > BASE)
		{

			Temp = Temp.Big_Copy(Temp.size + 1);
			int tem = Temp.Biginteger[MAX_Int.size - 2];
			Temp.Biginteger[MAX_Int.size - 2] = tem % BASE;
			Temp.Biginteger[MAX_Int.size - 1] = (tem - tem % BASE) / BASE;
			return Temp;
		}
		else
		{
			return Temp;
		}
	}
	bool NoNone(const BigInt& BI, int index) const
	{
		for (int i = BI.size; i > index + 1; --i)
		{
			if (BI.Biginteger[i - 1] != 0)
				return 1;
		}
		return 0;
	}
	BigInt operator -()const
	{
		BigInt z = *this;
		if(z == BigInt(0))
			return z;
		for (int i = 0; i < z.size; ++i)
			z.Biginteger[i] *= -1;
		z.sign = !(z.sign);
		return z;
	}
	BigInt operator -(const BigInt& a) const
	{
		//BigInt z1 = -a;
		//BigInt z2 = (*this);
		//return z1+z2;
		return BigInt(*this+-a);
	}
	bool operator >(const BigInt& a) const
	{
		if (sign < a.sign)
			return 1;
		if (a == *this)
			return 0;
		int z = (size > a.size ? size : a.size);
		BigInt Temp = *this;
		if (z == size)
		{
			Temp = a.Big_Copy(size);
			for (int i = 0; i < z; ++i)
			{
				if (Biginteger[z - 1 - i] < Temp.Biginteger[z - 1 - i])
					return 0;
			}
		}
		else
		{
			Temp = Temp.Big_Copy(a.size);
			for (int i = 0; i < z; ++i)
			{
				if (Temp.Biginteger[z - 1 - i] < a.Biginteger[z - 1 - i])
					return 0;
			}
		}
		return 1;
	}
	bool operator <(const BigInt& a) const
	{
		return !(*this > a || *this == a);
	}
	bool operator >=(const BigInt& a) const
	{
		return (a < *this || a == *this);
	}
	bool operator <=(const BigInt& a) const
	{
		return ((a > *this) || (a == *this));
	}
	bool operator !=(const BigInt& a) const
	{
		return !(*this == a);
	}
	BigInt Big_Copy(const int length) const
	{
		BigInt tmp(0, length);
		tmp.sign = sign;
		memcpy(tmp.Biginteger, Biginteger, sizeof(int) * size);
		return tmp;
	}
	bool operator ==(const BigInt& a) const
	{
		if (sign != a.sign)
			return 0;
		int z = (size > a.size ? size : a.size);
		BigInt Temp = *this;
		if (z == size)
		{
			Temp = a.Big_Copy(size);
			for (int i = 0; i < z; ++i)
			{
				if (Biginteger[i] != Temp.Biginteger[i])
					return 0;
			}
		}
		else
		{
			Temp = Temp.Big_Copy(a.size);
			for (int i = 0; i < z; ++i)
			{
				if (Temp.Biginteger[i] != a.Biginteger[i])
					return 0;
			}
		}
		return 1;
	}
};
std::ostream& operator<<(std::ostream& os, const BigInt& BI)
{  
	if (BI.sign == 1)
		os << '-';
	for(int i = 0; i < BI.size - 1; ++i)
	{
		if (BI.Biginteger[BI.size - 1 - i] != 0)
			os << abs(BI.Biginteger[BI.size - 1 - i]);
	}
	os << abs(BI.Biginteger[0]);
	return os;  
} 
