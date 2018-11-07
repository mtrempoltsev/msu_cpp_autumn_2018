#include <iostream>
#include <cstdlib>
#include <cstring>


const unsigned int BASE = 1e9;


class BigInt
{
public:
	BigInt(const int num = 0)
		: capacity(1)
		, length(1)
	{
		data = static_cast<size_t *>(std::calloc(1, sizeof(uint64_t)));
		//TODO:
		//case num = 1 234 234 234

		if (num < 0) {
			data[0] = -num;
			is_positive = false;
		} else {
			data[0] = num;
			is_positive = true;
		}
	}

	~BigInt()
	{
		std::free(data);
	}

	BigInt& operator=(const BigInt& other)
	{
		if (this == &other) {
			return *this;
		}

		length = other.length;
		capacity = other.capacity;
		is_positive = other.is_positive;

		free(data);
		data = static_cast<size_t *>(std::calloc(capacity, sizeof(size_t)));

		memcpy(data, other.data, length * sizeof(size_t));
		return *this;
	}

	bool operator==(const BigInt& other) const
	{
		if (this == &other) {
			return true;
		}

		return (is_positive == other.is_positive &&
				length == other.length &&
				memcmp(data, other.data, length * sizeof(uint64_t)) == 0);
	}

	bool operator!=(const BigInt& other) const
	{
		return !(*this == other);
	}

	BigInt operator+(const BigInt& other) const
    {
    	std::cout << *this << " + " << other << "\n";
    	if (!is_positive && other.is_positive) {
        	return (other - *this);
        }
        if (is_positive && !other.is_positive) {
        	return (*this - other);
        }

        BigInt tmp;

    	std::cout << *this << " + " << other << "\n";


        if (!is_positive && !other.is_positive) {
        	tmp.is_positive = false;
        }

        if (length > other.length) {
        	tmp.capacity = capacity;
        	tmp.length = length;
        	tmp.data = static_cast<size_t *>(realloc(tmp.data, tmp.capacity));

        	for (size_t i = 0; i < length; i++) {
        		tmp.data[i] = data[i];
        	}

        	for (size_t i = 0; i < other.length; i++) {
        		tmp.data[i] += other.data[i];
        	}

        	for (size_t i = 0; i < length - 1; i++) {
        		tmp.data[i + 1] += tmp.data[i] % BASE;
        		tmp.data[i] /= BASE;
        	}

        	if (tmp.data[tmp.length - 1] / BASE != 0) {
        		tmp.capacity *= 2;
        		tmp.length += 1;
        		tmp.data = static_cast<size_t *>(realloc(tmp.data, capacity));
        		tmp.data[tmp.length - 1] = tmp.data[tmp.length - 2] / BASE;
        		tmp.data[tmp.length - 2] %= BASE;
        	}
        } else {
        	//other.length >= length
        	tmp.capacity = other.capacity;
        	tmp.length = other.length;
        	tmp.data = static_cast<size_t *>(realloc(tmp.data, tmp.capacity));

        	for (size_t i = 0; i < other.length; i++) {
        		tmp.data[i] = other.data[i];
        	}

        	for (size_t i = 0; i < length; i++) {
        		tmp.data[i] += data[i];
        	}

        	for (size_t i = 0; i < tmp.length - 1; i++) {
        		tmp.data[i + 1] += tmp.data[i] % BASE;
        		tmp.data[i] /= BASE;
        	}

        	if (tmp.data[tmp.length - 1] / BASE != 0) {
        		tmp.capacity *= 2;
        		tmp.length += 1;
        		tmp.data = static_cast<size_t *>(realloc(tmp.data, tmp.capacity));
        		tmp.data[tmp.length - 1] = tmp.data[tmp.length - 2] / BASE;
        		tmp.data[tmp.length - 2] %= BASE;
        	}
        }   

            	std::cout << "tmp " << tmp << "\n";


        return tmp;
    }

    bool operator<(const BigInt& other) const
	{
		if (this == &other) {
			return false;
		}

		if (is_positive && !other.is_positive) {
			return false;
		}
		if (!is_positive && other.is_positive) {
			return true;
		} 

		if (is_positive && other.is_positive) {
			if (length > other.length) {
				return false;
			}
			if (length < other.length) {
				return true;
			} 

			for (int i = length - 1; i >= 0; i--) {
				if (data[i] < other.data[i]) {
					return true;
				}
				if (data[i] > other.data[i]) {
					return false;
				}
			}
		}

		if (!is_positive && !other.is_positive) {
			if (length > other.length) {
				return true;
			}
			if (length < other.length) {
				return false;
			} 

			for (int i = length - 1; i >= 0; i--) {
				if (data[i] < other.data[i]) {
					return false;
				}
				if (data[i] > other.data[i]) {
					return true;
				}
			}
		}

		return false;
	}

	bool operator<=(const BigInt& other) const
	{
		return (*this == other || *this < other);
	}

	bool operator>(const BigInt& other) const
	{
		return !(*this <= other);
	}

	bool operator>=(const BigInt& other) const
	{
		return !(*this < other);
	}

	BigInt operator-()
	{
		BigInt res;
		res = *this;
		res.is_positive = !this->is_positive;

		return res;
	}

	BigInt operator-(const BigInt& other) const
	{
		BigInt tmp;
		std::cout << "tmp " << tmp << "\n";


		if (!other.is_positive) {
			BigInt pos_other = other;
			pos_other.is_positive = true;
			BigInt tmp = (*this + pos_other);

			std::cout << "this " << *this << "\n";
			std::cout << "pos_other " << pos_other << "\n";
			std::cout << "tmp " << tmp << "\n";

			return tmp;
		}

		if (!is_positive) {
			BigInt pos_this = *this;
			pos_this.is_positive = true;
			tmp = pos_this + other;
			tmp.is_positive = false;
			return tmp;
		}

		if (*this == other) {
			return tmp;
		}

		if (*this < other) {
			return other - *this;
		}



	}

	friend std::ostream& operator <<(std::ostream &os, const BigInt& c);

private:
	bool is_positive;
	size_t capacity;
	size_t length;
	uint64_t *data;
};


std::ostream& operator <<(std::ostream &os, const BigInt& c)
{	
	if (!c.is_positive) {
		os << '-';
	}

	size_t len = 0;
	os << c.data[c.length - 1];

	for (int i = c.length - 2; i >= 0; i--) {
		len = std::to_string(c.data[i]).length();
		while (len < 9) { //9 or 10
			os << '0';
			len++;
		}

       	os << c.data[i];
    }
    
    return os;
}

int main()
{
	BigInt x(1);
	BigInt y(9);
	BigInt z = -y;


	std::cout << x - z << "\n";

	return 0;
}