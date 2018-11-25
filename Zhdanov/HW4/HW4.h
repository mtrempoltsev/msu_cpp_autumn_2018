#include <cstdlib>
#include <iostream>
#include<algorithm> 

using namespace std;

const int max_uint = 10;
const uint8_t max_i8t = 255;
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
	BigInt(const int init_num)
	{
		this->length = 0;
		int tmp_num = init_num;
		while (tmp_num != 0)
		{
			++this->length;
			tmp_num /= max_uint;
		};
		if (init_num < 0)
		{
			sign = false;
			tmp_num = -init_num;
		}
		else
		{
			sign = true;
			tmp_num = init_num;
		};
		number = new uint8_t[length];
		for (int i = 0; i < this->length; ++i)
		{
			number[i] = tmp_num % max_uint;
			tmp_num /= max_uint;
		};
	};
	BigInt(int length, bool sign)
		:length(length), sign(sign)
	{
		number = new uint8_t[length];
		for(int i=0; i<length;++i)
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
	BigInt& operator=(const int& number)
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
		if (this->sign == true && number.sign == false)
			return true;
		if (this->sign == true)
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
		if (this->sign == false)
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
		if (this->sign == true && number.sign == false)
			return true;
		if (this->sign == true)
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
		if (this->sign == false)
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
		if (number < 0)
		{
			if (this->sign == true)
				return false;
			tmp_num = -number;
		};
		if (number >= 0)
		{
			if (this->sign == false)
				return false;
			tmp_num = number;
		};
		bool same = true;
		for (int i = 0; i < this->length; ++i)
		{
			same = same&&(+this->number[i] == (tmp_num % +max_uint));
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
		int min_length = min(this->length, number.length);
		BigInt tmp_num(max_length + 1, number.sign);
		if (this->sign == number.sign)
		{
			tmp_num.sign = number.sign;
			for (int i = 0; i < min_length; ++i)
			{
				tmp_num.number[i] = this->number[i] + number.number[i];
				if (tmp_num.number[i] >= max_uint)
				{
					tmp_num.number[i + 1] += 1;
					tmp_num.number[i] -= max_uint;
				};
			};
			for (int i = min_length; i < max_length; ++i)
			{
				if (this->length > number.length)
					tmp_num.number[i] += this->number[i];
				else tmp_num.number[i] += number.number[i];
			};
		}
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
							tmp_num.number[i] = max_i8t - tmp_num.number[i];
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
							tmp_num.number[i] = max_i8t - tmp_num.number[i];
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
							tmp_num.number[i] = max_i8t - tmp_num.number[i];
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
							tmp_num.number[i] = max_i8t - tmp_num.number[i];
							tmp_num.number[i + 1] -= 1;
						};
					};
				};
			};
		};
		int i = tmp_num.length - 1;
		while (tmp_num.number[i] == 0)
		{
			--tmp_num.length;
			--i;
		};
		BigInt new_number(tmp_num.length, tmp_num.sign);
		for (int i = 0; i < tmp_num.length; ++i)
		{
			new_number.number[i] = tmp_num.number[i];
		};
		return new_number;
	};
	BigInt& operator-()
	{
		if (this->length > 0)
			this->sign = !(this->sign);
		return *this;
	};
	BigInt operator-(const BigInt& number) const
	{
		BigInt new_number = number;
		-new_number;
		new_number = *this + new_number;
		return new_number;
	};
	friend ostream& operator<<(ostream& out, const BigInt& number)
	{
		if (number.sign == false)
			out << '-';
		for(int i=0;i<number.length;++i)
			out << +number.number[number.length-i-1];
		return out;
	};
};