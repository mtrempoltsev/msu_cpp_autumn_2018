#pragma once

#include <cstdlib>
#include <iostream>
#include <algorithm> 
#include <sstream>

using namespace std;

const uint8_t max_uint = 100;
const uint8_t max_i8t = 156;
class BigInt
{
public:
	int length;
	bool sign; //+ true  - false
	uint8_t* number;

	BigInt()
		:length(1), sign(true)
	{
		number = new uint8_t[length];
		number[0] = 0;
	};
	BigInt(const int64_t init_num)
	{
		if (init_num == 0)
		{
			length = 1;
			number = new uint8_t[length];
			number[0] = 0;
			sign = true;
		}
		else
		{
			length = 0;
			int64_t tmp_num = abs(init_num);
			while (tmp_num != 0)
			{
				++length;
				tmp_num /= max_uint;
			};
			if (init_num < 0)
				sign = false;
			else
				sign = true;
			tmp_num = abs(init_num);
			number = new uint8_t[length];
			for (int i = 0; i < length; ++i)
			{
				number[i] = tmp_num % max_uint;
				tmp_num /= max_uint;
			};
		};
	};
	BigInt(int init_length, bool init_sign)
	{
		if (init_length == 0)
		{
			length = 0;
			sign = true;
		}
		else
		{
			sign = init_sign;
			length = init_length;
		};
		number = new uint8_t[length];
		for (int i = 0; i < length; ++i)
			number[i] = 0;
	};
	BigInt(const BigInt& to_copy)
		:length(to_copy.length), sign(to_copy.sign)
	{
		number = new uint8_t[length];
		for (int i = 0; i < length; ++i)
			number[i] = to_copy.number[i];
	};
	~BigInt()
	{
		delete[] number;
	};
	BigInt& operator=(const int64_t number)
	{
		BigInt new_number(number);
		*this = new_number;
		return *this;
	};
	BigInt& operator=(const BigInt& number)
	{
		if (this == &number)
			return *this;
		this->sign = number.sign;
		this->length = number.length;
		delete[] this->number;
		this->number = new uint8_t[number.length];
		for (int i = 0; i < number.length; ++i)
			this->number[i] = number.number[i];
		return *this;
	};
	bool operator>=(const BigInt& number) const
	{
		if (this->sign && !number.sign)
			return true;
		if (this->sign)
		{
			if (this->length > number.length)
				return true;
			if (this->length < number.length)
				return false;
			for (int i = this->length - 1; i >= 0; --i)
			{
				if (this->number[i] > number.number[i])
					return true;
				if (this->number[i] < number.number[i])
					return false;
			};
			return true;
		};
		if (!this->sign)
		{
			if (this->length < number.length)
				return true;
			if (this->length > number.length)
				return false;
			for (int i = this->length - 1; i >= 0; --i)
			{
				if (this->number[i] < number.number[i])
					return true;
				if (this->number[i] > number.number[i])
					return false;
			};
			return true;
		};
	};
	bool operator>(const BigInt& number) const
	{
		if (this->sign && !number.sign)
			return true;
		if (this->sign)
		{
			if (this->length > number.length)
				return true;
			if (this->length < number.length)
				return false;
			for (int i = this->length - 1; i >= 0; --i)
			{
				if (this->number[i] > number.number[i])
					return true;
				if (this->number[i] < number.number[i])
					return false;
			};
			return false;
		};
		if (!this->sign)
		{
			if (this->length < number.length)
				return true;
			if (this->length > number.length)
				return false;
			for (int i = this->length - 1; i >= 0; --i)
			{
				if (this->number[i] < number.number[i])
					return true;
				if (this->number[i] > number.number[i])
					return false;
			};
			return false;
		};
	};
	bool operator<=(const BigInt& number) const
	{
		return !(*this > number);
	};
	bool operator<(const BigInt& number) const
	{
		return !(*this >= number);
	};
	bool operator==(const int& number) const
	{
		int tmp_num;
		if ((number < 0) && (this->sign))
			return false;
		if ((number >= 0) && (!this->sign))
			return false;
		tmp_num = abs(number);
		bool same = true;
		for (int i = 0; i < this->length; ++i)
		{
			same = same && (+this->number[i] == (tmp_num % +max_uint));
			tmp_num /= max_uint;
			if (!same)
				break;
		};
		return same;
	};
	bool operator!=(const int& number) const
	{
		return !(*this == number);
	};
	bool operator==(const BigInt& number) const
	{
		if (this->sign != number.sign)
			return false;
		if (this->length != number.length)
			return false;
		for (int i = 0; i < this->length; ++i)
		{
			if (this->number[i] != number.number[i])
				return false;
		};
		return true;
	};
	bool operator!=(const BigInt& number) const
	{
		return !(*this == number);
	};
	BigInt operator+(const BigInt& number) const
	{
		int max_length = max(this->length, number.length);
		int min_length = this->length + number.length - max_length;
		BigInt tmp_num(max_length + 1, number.sign);
		// ������ ����� ��������
		// �������� ����� � ������������ �������
		if (this->sign == number.sign)
		{
			tmp_num.sign = number.sign;
			for (int i = 0; i < min_length; ++i)
			{
				tmp_num.number[i] += (this->number[i] + number.number[i]);
				if (tmp_num.number[i] >= max_uint)
				{
					tmp_num.number[i + 1] += 1;
					tmp_num.number[i] -= max_uint;
				};
			};
			for (int i = min_length; i < max_length; ++i)
			{
				if (this->length > number.length)
				{
					tmp_num.number[i] += this->number[i];
					if (tmp_num.number[i] >= max_uint)
					{
						tmp_num.number[i + 1] += 1;
						tmp_num.number[i] -= max_uint;
					};
				}
				else
				{
					tmp_num.number[i] += number.number[i];
					tmp_num.number[i] += this->number[i];
					if (tmp_num.number[i] >= max_uint)
					{
						tmp_num.number[i + 1] += 1;
						tmp_num.number[i] -= max_uint;
					};
				};
			};
		}
		//�������� ����� � ������� �������
		else
		{
			if (max_length != min_length)
			{
				if (this->length > number.length)
				{
					tmp_num = *this;
					for (int i = 0; i < min_length; ++i)
					{
						tmp_num.number[i] -= number.number[i];
						if (tmp_num.number[i] > max_uint)
						{
							tmp_num.number[i] = tmp_num.number[i] - max_i8t;
							tmp_num.number[i + 1] -= 1;
						};
					};
				}
				else
				{
					tmp_num = number;
					for (int i = 0; i < min_length; ++i)
					{
						tmp_num.number[i] -= this->number[i];
						if (tmp_num.number[i] > max_uint)
						{
							tmp_num.number[i] = tmp_num.number[i] - max_i8t;
							tmp_num.number[i + 1] -= 1;
						};
					};
				};
			}
			else
			{
				bool this_is_bigger = true;
				for (int i = max_length - 1; i >= 0; --i)
				{
					if (this->number[i] > number.number[i])
						break;
					if (this->number[i] < number.number[i])
					{
						this_is_bigger = false;
						break;
					};
				};
				if (this_is_bigger)
				{
					tmp_num = *this;
					for (int i = 0; i < min_length; ++i)
					{
						tmp_num.number[i] -= number.number[i];
						if (tmp_num.number[i] > max_uint)
						{
							tmp_num.number[i] = tmp_num.number[i] - max_i8t;
							tmp_num.number[i + 1] -= 1;
						};
					};
				}
				else
				{
					tmp_num = number;
					for (int i = 0; i < min_length; ++i)
					{
						tmp_num.number[i] -= this->number[i];
						if (tmp_num.number[i] > max_uint)
						{
							tmp_num.number[i] = tmp_num.number[i] - max_i8t;
							tmp_num.number[i + 1] -= 1;
						};
					};
				};
			};
		};
		//����� ����� ��������
		int i = tmp_num.length - 1;
		while (i != -1)
		{
			if (tmp_num.number[i] != 0)
				break;
			--tmp_num.length;
			--i;
		};
		if (tmp_num.length == 0)
		{
			BigInt zero_num(0);
			return zero_num;
		}
		return tmp_num;
	};
	BigInt operator-() const
	{
		BigInt new_number(*this);
		if ((this->length > 1) || (this->number[0] != 0))
			new_number.sign = !(this->sign);
		return new_number;
	};
	BigInt operator-(const BigInt& number) const
	{
		return (*this + (-number));
	};
	friend ostream& operator<<(ostream& out, const BigInt& number)
	{
		if (!number.sign)
			out << '-';
		for (int i = 0; i < number.length; ++i)
			if ((number.number[number.length - i - 1] / 10 == 0) && (i != 0))
			{
				out << 0;
				out << +number.number[number.length - i - 1];
			}
			else
				out << +number.number[number.length - i - 1];
		return out;
	};
};