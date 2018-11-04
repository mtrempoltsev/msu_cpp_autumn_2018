#include <stdint.h>
#include <iostream>
#include <iomanip>

using namespace std;

typedef unsigned long long int ullint;
const int numbers_in_out = 18;
ullint const base = 1.0e+18;

template <class T>
class myVector
{
	T* data_v;
	size_t size_v = 1;
	
public:
	myVector(size_t size=1) : size_v(size)
	{
		data_v = new T[size_v];
		for (size_t i = 0; i < size_v; ++i)
		{
			data_v[i] = 0;
		}
	}
	
	~myVector()
	{
		delete [] data_v;
		size_v = 0;
	}
	
	T& operator [] (const size_t j)
	{
		if (j >= size_v)
		{
			T* temp = data_v;
			size_t size_old = size_v;
			while (j >= size_v)
			{
				size_v *= 2;
			}
			data_v = new T[size_v];
			for (size_t i = 0; i < size_old; ++i)
			{
				data_v[i] = temp[i];
			}
			delete [] temp;
			for (size_t i = size_old; i < size_v; ++i)
			{
				data_v[i] = 0;
			}
		}
		
		return data_v[j];
	}
		
	const T operator [] (const size_t j) const
	{
		if (j < size_v)
		{
			return data_v[j];
		}
		else
		{
			throw std::out_of_range("");
		}
	}
	
	bool operator < (const myVector & other) const
	{
		if (this->size_v > other.size_v)
		{
			return false;
		}		

		if (this->size_v < other.size_v)
		{
			return true;
		}
		
		for (int i = this->size_v - 1; i >= 0; --i)
		{
			if (this->data_v[i] > other.data_v[i]) 
			{
				return false;
			}
			if (this->data_v[i] < other.data_v[i]) 
			{
				return true;
			}
		}
		
		return false;
	}
	
	bool operator > (const myVector & other) const
	{
		if (this->size_v > other.size_v)
		{
			return true;
		}		

		if (this->size_v < other.size_v)
		{
			return false;
		}
		
		for (int i = this->size_v - 1; i >= 0; --i)
		{
			if (this->data_v[i] > other.data_v[i]) 
			{
				return true;
			}
			if (this->data_v[i] < other.data_v[i]) 
			{
				return false;
			}
		}
		
		return false;
	}
	
	bool operator == (const myVector & other) const
	{
		if (this->size_v != other.size_v)
		{
			return false;
		}		
		
		for (int i = this->size_v - 1; i >= 0; --i)
		{
			if (this->data_v[i] != other.data_v[i]) 
			{
				return false;
			}
		}
		return true;
	}
};

class BigInt
{
	myVector<ullint> data;
	size_t size = 0;
	bool sign = 1;
	
public:
	
	BigInt(const long long int num = 0)
	{
		if (num < 0)
		{
			sign = 0;
			ullint tmp = num * (-1);
			if (tmp < base)
			{
				size = 1;
				data[0] = tmp;
			}
			else
			{
				while (tmp != 0)
				{
					data[size++] = tmp % base;
					tmp /= base;
				}
			}	
		}
		else
		{
			sign = 1;
			ullint tmp = num;
			if (tmp < base)
			{
				size = 1;
				data[0] = tmp;
			}
			else
			{
				while (tmp != 0)
				{
					data[size++] = tmp % base;
					tmp /= base;
				}
			}
		}
	}
	
	BigInt operator +(const BigInt& other) const
	{
		if (this->sign == other.sign)
		{
			BigInt tmp;
			tmp.sign = this->sign;
			tmp.size = 0;
			
			size_t i = 0;
			size_t j = 0;
			size_t transfer = 0;
			
			while ((i < this->size) && (j < other.size))
			{
				tmp.data[tmp.size++] = (transfer + this->data[i]
										+ other.data[j]) % base;
				transfer = (transfer + this->data[i] + other.data[j]) / base;
				++i;
				++j;
			}
			
			while (i < this->size)
			{
				tmp.data[tmp.size++] = (transfer + this->data[i]) % base;
				transfer = (transfer + this->data[i]) / base;
				i++;
			}
			
			while (j < other.size)
			{
				tmp.data[tmp.size++] = (transfer + other.data[j]) % base;
				transfer = (transfer + other.data[j]) / base;
				j++;
			}
			
			if (transfer > 0)
			{
				tmp.data[tmp.size++] = transfer;
			}
			
			return tmp;
		}
		else
		{
			if (this->data == other.data)
			{
				BigInt tmp = 0;
				return tmp;
			}
			else
			{
				if (this->data > other.data)
				{
					BigInt tmp1 = other;
					tmp1.sign = !tmp1.sign;
					BigInt tmp = *this - tmp1;
					tmp.sign = this->sign;
					return tmp;
				}
				else
				{
					BigInt tmp1 = *this;
					tmp1.sign = !tmp1.sign;
					BigInt tmp = tmp1 - other;
					tmp.sign = other.sign;
					return tmp;
				}
			}
		}
	}
	
	BigInt operator -(const BigInt& other) const
	{
		if (this->sign != other.sign)
		{
			BigInt tmp1 = other;
			tmp1.sign = !tmp1.sign;
			BigInt tmp = *this + tmp1;
			tmp.sign = this->sign;
			return tmp;
		}
		else
		{
			if (this->data == other.data)
			{
				BigInt tmp = 0;
				return tmp;
			}
			else
			{	
				BigInt tmp;
				tmp.size = 0;
				size_t i = 0;
				size_t j = 0;
				size_t transfer = 0;
				
				if (this->data > other.data)
				{
					tmp.sign = this->sign;
					
					while ((i < this->size) && (j < other.size))
					{
						if (this->data[i] >= other.data[i] + transfer)
						{
							tmp.data[tmp.size++] = this->data[i] -
							                       transfer - other.data[j];
							transfer = 0;
						}
						else
						{
							tmp.data[tmp.size++] = this->data[i] -
							                       transfer + base - other.data[j];
							transfer = 1;
						}
						++i;
						++j;
					}
					
					while (i < this->size)
					{
						if (this->data[i] >= transfer){
							tmp.data[tmp.size++] = this->data[i] - transfer;
							transfer = 0;
						}
						else
						{
							tmp.data[tmp.size++] = this->data[i] - transfer + base;
							transfer = 1;
						}
						i++;
					}

					return tmp;
				}
				else
				{
					tmp.sign = !other.sign;

					while ((i < this->size) && (j < other.size))
					{
						if (other.data[i] >= this->data[i] + transfer)
						{
							tmp.data[tmp.size++] = other.data[i] -
							                       transfer - this->data[j];
							transfer = 0;
						}
						else
						{
							tmp.data[tmp.size++] = other.data[i] -
							                       transfer + base - this->data[j];
							transfer = 1;
						}
						++i;
						++j;
					}
					
					while (i < other.size)
					{
						if (other.data[i] >= transfer){
							tmp.data[tmp.size++] = other.data[i] - transfer;
							transfer = 0;
						}
						else
						{
							tmp.data[tmp.size++] = other.data[i] - transfer + base;
							transfer = 1;
						}
						i++;
					}

					return tmp;
				}
			}
		}
	}
	
	bool operator != (const BigInt& other) const
	{
		if ((this->size != other.size) || (this->sign != other.sign))
		{
			return true;
		}
		
		for (size_t i = 0; i < size; ++i)
		{
			if (this->data[i] != other.data[i])
			{
				return true;
			}
		}
		
		return false;
	}
	
	bool operator == (const BigInt& other) const
	{
		return !(*this != other);
	}
	
	bool operator < (const BigInt& other) const
	{
		if (this->sign == other.sign) 
		{
			if (this->sign == 0)
			{		
				if (this->size > other.size)
				{
					return true;
				}
				
				if (this->size < other.size)
				{
					return false;
				}
				
				for (size_t i = 0; i < size; ++i)
				{
					if (this->data[i] > other.data[i])
					{
						return true;
					}
					
					if (this->data[i] < other.data[i])
					{
						return false;
					}			
				}
				return false;
			}
			else
			{
				if (this->size > other.size)
				{
					return false;
				}
				
				if (this->size < other.size)
				{
					return true;
				}
				
				for (size_t i = 0; i < size; ++i)
				{
					if (this->data[i] > other.data[i])
					{
						return false;
					}
					
					if (this->data[i] < other.data[i])
					{
						return true;
					}			
				}
				return false;
			}			
		}
		else
		{
			if (this->sign == 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	
	bool operator > (const BigInt& other) const
	{
		if (this->sign == other.sign) 
		{
			if (this->sign == 0)
			{		
				if (this->size > other.size)
				{
					return false;
				}
				
				if (this->size < other.size)
				{
					return true;
				}
				
				for (size_t i = 0; i < size; ++i)
				{
					if (this->data[i] > other.data[i])
					{
						return false;
					}
					
					if (this->data[i] < other.data[i])
					{
						return true;
					}			
				}
				return false;
			}
			else
			{
				if (this->size > other.size)
				{
					return true;
				}
				
				if (this->size < other.size)
				{
					return false;
				}
				
				for (size_t i = 0; i < size; ++i)
				{
					if (this->data[i] > other.data[i])
					{
						return true;
					}
					
					if (this->data[i] < other.data[i])
					{
						return false;
					}			
				}
				return false;
			}			
		}
		else
		{
			if (this->sign == 0)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}	

	bool operator <= (const BigInt& other) const
	{
		return !(*this > other);
	}
	
	bool operator >= (const BigInt& other) const
	{
		return !(*this < other);
	}
		
	BigInt(const BigInt& copied): size(copied.size), sign(copied.sign) 
    {
		for (size_t i = 0; i < size; ++i)
		{
			data[i] = copied.data[i];
		}
    }
    
    BigInt operator - ()
    {
		BigInt tmp;
		tmp.size = this->size;
		for (size_t i = 0; i < size; ++i)
		{
			tmp.data[i] = this->data[i];
		}
		tmp.sign = this->sign;
		if (!((tmp.size == 1) && (tmp.data[0] == 0)))
		{
			tmp.sign = !tmp.sign;
		}
		return tmp;
	}
	
    BigInt& operator=(const BigInt& copied)
    {
		if (this == &copied)
			return *this;
		
		for (size_t i = 0; i < size; ++i)
		{
			data[i] = copied.data[i];
		}

		return *this;
	}
    
    friend ostream& operator<<(ostream &os, const BigInt& a);	
};

ostream& operator<<(ostream &os, const BigInt& a)
{
	bool write_ullint;
	
	if (a.sign == 0) os << '-';
		write_ullint = false;
		for (int i = a.size - 1; i >= 0; --i)
		{
			if (write_ullint)
			{
				os << setw(numbers_in_out) << std::setfill('0') << a.data[i];
			}
			else
			{
				if ((i == 0) || (a.data[i] > 0))
				{
					os << a.data[i];
				}
				
				if (a.data[i] > 0)
				{
					write_ullint = true;
				}
			}

		}
	return os;		
}
