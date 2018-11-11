#include <ostream>
#include <iostream>
using std::ostream;

class BigInt
{
private:
	int size_arr;
	int size_num;
	bool sign;
	char *number;

public:
	BigInt(long long a = 0)
	{
		sign = 0;
		if (a < 0) {
			a = -a;
			sign = 1;
		}
		size_arr = 32;
		number = new char[size_arr];
		if (a == 0) {
			number[0] = 0;
		}
		int i = 0;
		while (a > 0)
		{
			number[i++] = a % 10;
			a /= 10;
		}
		size_num = (i == 0) ? 1 : i;
		for (int i = size_num; i < size_arr; ++i) {
			number[i] = 0;
		}
	}
	~BigInt()
	{
		delete[] number;
	}
	BigInt(const BigInt &copied)
	{
		size_arr = copied.size_arr;
		size_num = copied.size_num;
		sign = copied.sign;
		number = new char[size_arr];
		for (int i = 0; i < size_arr; i++) {
			number[i] = copied.number[i];
		}
	}

	BigInt& operator=(const BigInt& copied)
	{
		if (this == &copied) {
			return *this;
		}
		delete[] number;
		number = new char[copied.size_arr];
		size_arr = copied.size_arr;
		size_num = copied.size_num;
		sign = copied.sign;
		for (int i = 0; i < size_arr; i++) {
			number[i] = copied.number[i];
		}
		return *this;
	}

	BigInt operator-() const
	{
		BigInt tmp = *this;
		if (*this != 0) {
			tmp.sign = (sign == 1) ? 0 : 1;
		}
		return tmp;
	}
	
	BigInt operator+ (const BigInt &rvalue) const
	{
		if (rvalue == 0) {
			return *this;
		}
		if (sign == rvalue.sign) {
			if (size_num < rvalue.size_num) {
				return (rvalue + *this);
			}
			BigInt res;
			res.sign = sign;
			res.size_arr = size_arr;
			if (size_num + 1 > size_arr) {
				res.size_arr = 2 * size_arr;
				delete[] res.number;
				res.number = new char[res.size_arr];
				for (int i = 0; i < size_arr; ++i) {
					res.number[i] = 0;
				}
			}
			int flag = 0;
			for (int i = 0; i <= size_num; ++i) {
				if (i < rvalue.size_num && i < size_num) {
					res.number[i] = number[i] + rvalue.number[i] + flag;
				} else if (i < size_num) {
					res.number[i] = number[i] + flag;
				} else if (flag) {
					res.number[i] = 1;
				}
				if (res.number[i] > 9) {
					res.number[i] -= 10;
					flag = 1;
				} else {
					flag = 0;
				}
			}
			if (res.number[size_num] > 0) {
				res.size_num = size_num + 1;
			} else {
				res.size_num = size_num;
			}
			return res;
		} else {
			return (*this - (-rvalue));
		}
	}

	BigInt operator- (const BigInt &rvalue) const
	{
		if (rvalue == 0) {
			return *this;
		}
		if (sign == rvalue.sign) {
			if (*this == rvalue) {
				return 0;
			}
			if ((!sign) && (*this < rvalue)) {
				return -(rvalue - *this);
			}
			if ((sign) && (*this > rvalue)) {
				return -(rvalue - *this);
			}
			BigInt res;
			res.sign = sign;
			res.size_arr = size_arr;
			res.size_num = size_num;
			delete[] res.number;
			res.number = new char[res.size_arr];
			for (int i = 0; i < size_arr; ++i) {
				res.number[i] = 0;
			}
			int flag = 0;
			for (int i = 0; i < size_num; ++i) {
				if (i < rvalue.size_num) {
					res.number[i] = number[i] - rvalue.number[i] + flag;
				} else {
					res.number[i] = number[i] + flag;
				}
				if (res.number[i] < 0) {
					res.number[i] += 10;
					flag = -1;
				} else {
					flag = 0;
				}
			}
			while (res.number[res.size_num - 1] == 0 && res.size_num != 1) {
				res.size_num--;
			}
			return res;
		} else {
			return (*this + (-rvalue));
		}
	}

	bool operator== (const BigInt &rvalue) const
	{
		if (this == &rvalue) {
			return 1;
		} else {
			if ((sign != rvalue.sign) || (size_num != rvalue.size_num)) {
				return 0;
			}
			for (int i = 0; i < size_num; i++) {
				if (number[i] != rvalue.number[i]) {
					return 0;
				}
			}
		}
		return 1;
	}

	bool operator< (const BigInt &rvalue) const
	{
		if (*this == rvalue) {
			return 0;
		}
		if (sign > rvalue.sign) {
			return 1;
		}
		if (sign < rvalue.sign) {
			return 0;
		}
		if (size_num > rvalue.size_num) {
			return (sign == 0) ? 0 : 1;
		}
		if (size_num < rvalue.size_num) {
			return (sign == 0) ? 1 : 0;
		}
		for (int i = size_num - 1; i >= 0; --i) {
			if (number[i] > rvalue.number[i]) {
				return (sign == 0) ? 0 : 1;
			}
			if (number[i] < rvalue.number[i]) {
				return (sign == 0) ? 1 : 0;
			}
		}
		return 0;
	}

	bool operator> (const BigInt &rvalue) const
	{
		if (rvalue == *this){
			return 0;
		}
		return (rvalue < *this);
	}

	bool operator>= (const BigInt &rvalue) const
	{
		return !(*this < rvalue);
	}

	bool operator<= (const BigInt &rvalue) const
	{
		return !(*this > rvalue);
	}

	bool operator!=(const BigInt &rvalue) const
	{
		return !(*this == rvalue);
	}
	friend ostream &operator<< (ostream &, const BigInt &);
};

ostream &operator<< (ostream &out, const BigInt &obj)
{
	if (obj.sign == 1)
		out << '-';
	for (int i = obj.size_num - 1; i >= 0; --i) {
		out << obj.number[i] - 0;
	}
	return out;
}
