#include <iostream>
#include <string>
#include<cstring>
#include <cstdlib>
#include <math.h>
#include <algorithm>

using namespace std;

class BigInt
{
	
	int get_extra_dozen(int64_t& num) const
	{
		int res;
		if (num >= 1000000000000000000)
		{
			res = num / 1000000000000000000;
			num = num % 1000000000000000000;
		}
		else
			res = 0;
		return res;
	}

	void delete_zeros(BigInt& num) const
	{
		for (size_t i = num.size_ - 1; i > 0; i--)
		{
			if (num.value[i] == 0)
				num.size_--;
			else
				break;
		}
	}

	void new_order(BigInt& num,const int extra_dozen) const
	{
		if (extra_dozen != 0)
		{
			num.value = (int64_t*)realloc(num.value, (num.size_ + 1) * sizeof(int64_t));
			num.value[size_] = extra_dozen;
			num.size_++;
		}
	}

	void edit(BigInt& num) const
	{
		num.sign = (num.value[num.size_ - 1] > 0) ? 1 : -1;
		int extra_dozen = 0;
		int sign_;
		for (size_t i = 0; i < num.size_; i++)
		{
			sign_ = (num.value[i] > 0) ? 1 : -1;
			num.value[i] += extra_dozen;
			if (num.sign != sign_)
			{
				num.value[i] = 1000000000000000000 + num.sign*num.value[i];
				extra_dozen = -num.sign;
			}
			else
				extra_dozen = 0;
		}
		for (size_t i = 0; i < num.size_; i++)
			num.value[i] = abs(num.value[i]);
		delete_zeros(num);
	}
	
	size_t size_;
	int sign;
	int64_t* value;
public:

	BigInt() :size_(1), sign(1), value(new int64_t[1]) { value[0] = 0; }

	BigInt(const int64_t& x)
		:size_((x < 1000000000000000000) ? 1 : 2),
		sign((x>0)?1:-1),
		value(new int64_t[size_])
	{
		if(size_==1)
				value[0] = sign * x;
		else
		{
			value[0] = (x % 1000000000000000000)*sign;
			value[1] = (x / 1000000000000000000)*sign;
		}
	}

	BigInt(const int x)
		:size_(1),
		sign((x>0) ? 1 : -1),
		value(new int64_t[size_])
	{
		value[0] = int64_t(x)*sign;
	}

	BigInt(const char* input)
		:size_(strlen(input) % 18 == 0 ? strlen(input) / 18 : strlen(input) / 18 + 1),
		sign(1),
		value(new int64_t[size_])
	{
		string s = input;
		if (input[0] == '-')
		{
			sign = -1;
			s=s.substr(1);
			if (s.length() % 18 == 0)
				size_ -= 1;
		}
		size_t i = 0;
		for (i; i < size_-1; i++)
			value[i] = atoi(s.substr(s.length()-(i+1)* 18,18).c_str());
		value[size_ - 1]= atoi(s.substr(0, s.length() - i * 18).c_str());
	}

	BigInt(const BigInt& copied)
		:size_(copied.size_),
		sign(copied.sign),
		value(new int64_t[size_])
	{
		for (size_t i = 0; i < size_; i++)
			value[i] = copied.value[i];
	}

	~BigInt() { delete[] value; }

	BigInt& operator=(const BigInt& copied)
	{
		sign = copied.sign;
		size_ = copied.size_;
		delete[] value;
		value = new int64_t[size_];
		for (size_t i = 0; i < size_; i++)
			value[i] = copied.value[i];
		return *this;
	}

	BigInt& operator=(const int64_t& num)
	{	
		BigInt tmp(num);
		*this = tmp;
		return *this;
	}

	BigInt& operator=(const char* num)
	{
		BigInt tmp(num);
		*this = tmp;
		return *this;
	}

	BigInt operator+(const BigInt& other) const
	{

		int extra_dozen=0;
		BigInt res;
		if (other.size_ > size_)
		{
			delete[] res.value;
			res.value = new int64_t[other.size_];
			res.size_ = other.size_;
			for (size_t i = 0; i < size_; i++)
			{
				res.value[i] = other.sign*other.value[i] + extra_dozen + value[i] * sign;
				extra_dozen = get_extra_dozen(res.value[i]);
			}
			for (size_t i = size_; i < other.size_; i++)
			{
				res.value[i] = other.sign*other.value[i] + extra_dozen;
				extra_dozen = get_extra_dozen(res.value[i]);

			}
		}
		else
		{
			delete[] res.value;
			res.value = new int64_t[size_];
			res.size_ = size_;
			for (size_t i = 0; i < other.size_; i++)
			{
				res.value[i] = other.sign*other.value[i] + extra_dozen + value[i] * sign;
				extra_dozen = get_extra_dozen(res.value[i]);
			}
			for (size_t i = other.size_; i < size_; i++)
			{
				res.value[i] = extra_dozen + value[i] * sign;
				extra_dozen = get_extra_dozen(res.value[i]);
				if (extra_dozen == 0)
					break;
			}
		}
		new_order(res, extra_dozen);	
		delete_zeros(res);
		edit(res);
		return res;
	}
	
	BigInt operator-() const
	{
		BigInt res = *this;
		if(!(size_ ==1 && value[0] == 0))
			res.sign *= -1;
		return res;
	}

	BigInt operator-(const BigInt& other) const
	{
		BigInt res = *this + (-other);
		return res;
	}

	const bool operator==(const BigInt& other) const
	{
		if (size_ != other.size_)
			return false;
		if (sign != other.sign)
			return false;
		for (size_t i = 0; i < size_; i++)
		{
			if (value[i] != other.value[i])
				return false;
		}
		return true;
	}

	const bool operator>(const BigInt& other) const
	{

		if (sign == -1)
		{
			if (other.sign == 1 || size_ > other.size_)
				return false;
			if (size_ < other.size_)
				return true;
			for (size_t i = size_ - 1; i > 0; i--)
			{
				if (value[i] > other.value[i])
					return false;
			}
			return value[0] < other.value[0];
		}
		if (sign == 1)
		{
			if (other.sign == -1 || size_ > other.size_)
				return true;
			if (size_ < other.size_)
				return false;
			for (int i = size_ - 1; i >= 0; i--)
			{
				if (value[i] < other.value[i])
					return false;
			}
			return value[0] > other.value[0];
		}
		return true;
	}

	const bool operator>=(const BigInt& other) const
	{
		return *this > other || *this == other;
	}

	const bool operator<(const BigInt& other) const
	{
		return !(*this >= other);
	}

	const bool operator<=(const BigInt& other) const
	{
		return !(*this > other);
	}

	const bool operator!=(const BigInt& other) const
	{
		return !(*this == other);
	}

	friend ostream& operator << (ostream& out, const BigInt& num);
};

ostream& operator << (ostream& out, const BigInt& num)
{
	out << num.sign*num.value[num.size_ - 1];
	for (int i = num.size_ - 2; i >= 0; i--)
	{
		for (int j = 17; j > 0; j--)
		{
			if (num.value[i] / int(pow(10, j)) == 0)
				out << 0;
			else
				break;
		}
		out << num.value[i];
	}
	return out;
}
