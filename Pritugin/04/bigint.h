#include <iostream>
#include "vector.h"
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

class BigInt 
{
	static const int64_t BASE = 100000000000;

	MyVector<int64_t> _data_;

	bool negative;

	void del_zeros()
	{
		while (_data_.size() > 1 && _data_.back() == 0) 
		{
			_data_.pop_back();
		}

		if (_data_.size() == 1 && _data_[0] == 0) negative = false;
	}
public:

	BigInt() 
	{
		negative = false;
	}
	
	explicit BigInt(const string& str)
	{
		size_t begin = 0;
		if (str[0] == '-')
			begin = 1;
			
		if (str.length() == 0)
			negative = false;
		else 
		{
			if (str[0] == '-')
				negative = true;
			else
				negative = false;

			for (long long i = str.length(); i > begin; i -= 9) 
			{
				if (i < 9)
					_data_.push_back((int64_t)atoi(str.substr(0, i).c_str()));
				else
					_data_.push_back((int64_t)atoi(str.substr(i - 9, 9).c_str()));
			}

			del_zeros();
		}
	}
	
	BigInt(const char* c)
	{
		size_t begin = 0;
		std::string str(c);
		if (str[0] == '-')
			begin = 1;
		if (str.length() == 0)
			negative = false;
		else 
		{
			if (str[0] == '-') 
				negative = true;
			else 
				negative = false;

			for (long long i = str.length(); i > begin; i -= 9)
			{
				if (i < 9)
					_data_.push_back(atoi(str.substr(0, i).c_str()));
				else
					_data_.push_back(atoi(str.substr(i - 9, 9).c_str()));
			}

			del_zeros();
		}
	}
	

	BigInt(int64_t i)
	{
		if (i < 0)
			negative = true;
		else
			negative = false;
			
		_data_.push_back(std::abs(i) % BASE);
		i /= BASE;
		if (i != 0) _data_.push_back(abs(i));
	}
	
	BigInt(int i) : BigInt((int64_t)i) {}


	friend ostream& operator <<(ostream& os, const BigInt& bi)
	{
		if (bi._data_.empty())
			os << 0;
		else 
		{
			if (bi.negative)
				os << '-';
			os << bi._data_.back();
			char old_fill = os.fill('0');
			for (long long i = (long long)(bi._data_.size()) - 2; i >= 0; --i) 
				os << setw(9) << bi._data_[i];
			os.fill(old_fill);
		}
		
		return os;
	}
	operator string() const
	{
		std::stringstream ss;
		ss << *this;
		return ss.str();
	}
	const BigInt operator + () const
	{
		return BigInt(*this);
	}
	const BigInt operator - () const
	{
		BigInt copy(*this);
		copy.negative = !copy.negative;
		copy.del_zeros();
		return copy;
	}
	const BigInt operator * (const BigInt& other) const
	{
		BigInt result;
		result._data_.resize(_data_.size() + other._data_.size());
		for (size_t i = 0; i < _data_.size(); ++i) 
		{
			int carry = 0;
			for (size_t j = 0; j < other._data_.size() || carry != 0; ++j)
			{
				long long cur = result._data_[i + j] +
								_data_[i] * 1LL * (j < other._data_.size() ? other._data_[j] : 0) + carry;
				result._data_[i + j] = (cur % BASE);
				carry = (cur / BASE);
			}
		}

		result.negative = negative != other.negative;
		
		result.del_zeros();
		
		return result;
	}
	
	bool operator == (const BigInt& other) const
	{
		if (negative != other.negative)
			return false;
		if (_data_.empty())
		{
			if (other._data_.empty() || (other._data_.size() == 1 && other._data_[0] == 0))
				return true;
			else
				return false;
		}

		if (other._data_.empty()) {
			if (_data_.size() == 1 && _data_[0] == 0)
				return true;
			else
				return false;
		}

		if (_data_.size() != other._data_.size())
			return false;
		for (size_t i = 0; i < _data_.size(); ++i) if (_data_[i] != other._data_[i])
			return false;

		return true;
	}
	
	bool operator < (const BigInt& other) const
	{
		if (*this == other)
			return false;
		if (negative) 
		{
			if (other.negative)
				return ((-other) < (-*this));
			else
				return true;
		}
		else 
		{
			if (other.negative)
				return false;
			else 
			{
				if (_data_.size() != other._data_.size()) {
					return _data_.size() < other._data_.size();
				}
				else 
				{
					for (long long i = _data_.size() - 1; i >= 0; --i)
					{
						if (_data_[i] != other._data_[i])
							return _data_[i] < other._data_[i];
					}

					return false;
				}
			}
		}
	}
	
	bool operator != (const BigInt& other) const
	{
		return !operator == (other);
	}
	
	bool operator <= (const BigInt& other) const
	{
		return operator < (other) || operator == (other);
	}
	
	bool operator > (const BigInt& other) const
	{
		return !operator <= (other);
	}
	
	bool operator >= (const BigInt& other) const
	{
		return !operator < (other);
	}
	
	const BigInt operator + (const BigInt& other) const
	{
		BigInt result(*this);
		if (result.negative) 
		{
			if (other.negative)
				return -(-result + (-other));
			else
				return other - (-result);
		}
		else if (other.negative)
				return result - (-other);
				
		int carry = 0;
		for (size_t i = 0; i < std::max(result._data_.size(), other._data_.size()) || carry != 0; ++i)
		{
			if (i == result._data_.size()) 
				result._data_.push_back((int64_t)0);
				
			result._data_[i] += carry + (i < other._data_.size() ? other._data_[i] : 0);
			carry = result._data_[i] >= BASE;
			if (carry != 0)
				result._data_[i] -= BASE;
		}

		return result;
	}
	
	BigInt& operator += (const BigInt& other)
	{
		return *this = (*this + other);
	}
	
	const BigInt operator - (const BigInt& other) const
	{
		BigInt result(*this);
		if (other.negative)
			return result + (-other);
		else if (negative)
				return -(-result + other);
			else if (result < other)
					return -(other - result);
					
		int carry = 0;
		for (size_t i = 0; i < other._data_.size() || carry != 0; ++i)
		{
			result._data_[i] -= carry + (i < other._data_.size() ? other._data_[i] : 0);
			carry = result._data_[i] < 0;
			if (carry != 0) result._data_[i] += BASE;
		}

		result.del_zeros();
		return result;
	}
	
	BigInt& operator -= (const BigInt& other)
	{
		return *this = (*this - other);
	}
	
	const BigInt sqrt() const
	{
		BigInt l(0), r(*this);
		BigInt m, res;
		while (l <= r)
		{
			m = (l + r);
			m /= 2;
			if (m * m <= *this)
			{
				res = m;
				l = m + 1;
			}
			else
				r = m - 1;
		}
		return res;
	}

	BigInt& operator /= (int64_t b)
	{
		int carry = 0;
		for (int i=(int)_data_.size()-1; i>=0; --i) 
		{
			long long cur = _data_[i] + carry * 1ll * BASE;
			_data_[i] = int64_t (cur / b);
			carry = int64_t (cur % b);
		}
		return *this;
	}
	
	const BigInt operator / (int64_t b) const
	{
		BigInt result(*this);
		int carry = 0;
		for (int i=(int)result._data_.size()-1; i>=0; --i) 
		{
			long long cur = result._data_[i] + carry * 1ll * BASE;
			result._data_[i] = int64_t (cur / b);
			carry = int64_t (cur % b);
		}
		
		return result;
	}
};




