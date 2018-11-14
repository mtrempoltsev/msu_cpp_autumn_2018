#include <iostream>
#include <cstdlib>
#include <cstring>


const uint64_t BASE = 1e18;


class BigInt
{
public:
	BigInt(const int64_t num = 0)
		: capacity(1)
		, length(1)
	{
		int64_t tmp_num;

		if (num < 0) {
			tmp_num = -num;
			is_positive = false;
		} else {
			tmp_num = num;
			is_positive = true;
		}

		if (tmp_num / BASE == 0) {
			data = static_cast<int64_t *>(std::calloc(capacity, sizeof(int64_t)));

			data[0] = tmp_num;
		} else {
			capacity = 2;
			length = 2;
			data = static_cast<int64_t *>(std::calloc(capacity, sizeof(int64_t)));

			data[0] = tmp_num % BASE;
			data[1] = tmp_num / BASE;
		}
	}

	BigInt(const BigInt& other)
		: capacity(other.capacity)
		, length(other.length)
		, is_positive(other.is_positive)
	{
		data = static_cast<int64_t *>(std::calloc(length, sizeof(int64_t)));
		for (size_t i = 0; i < length; i++) {
			data[i] = other.data[i];
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
		capacity = length;
		is_positive = other.is_positive;

		//std::cout << "element before: " << *this << "\n";
		data = static_cast<int64_t *>(std::calloc(length, sizeof(int64_t)));

		for (size_t i = 0; i < length; i++) {
			data[i] = other.data[i];
		}
		//memcpy(data, other.data, length * sizeof(int64_t));

		return *this;
	}

	bool operator==(const BigInt& other) const
	{
		if (this == &other) {
			return true;
		}

		return (is_positive == other.is_positive &&
				length == other.length &&
				memcmp(data, other.data, length * sizeof(int64_t)) == 0);
	}

	bool operator!=(const BigInt& other) const
	{
		return !(*this == other);
	}

	BigInt operator+(const BigInt& other) const
    {
    	if (!is_positive && other.is_positive) {
        	return (other - -*this);
        }
        if (is_positive && !other.is_positive) {
        	return (*this - -other);
        }

        BigInt tmp;

        if (!is_positive && !other.is_positive) {
        	tmp.is_positive = false;
        }

        if (length > other.length) {
        	tmp.capacity = capacity;
        	tmp.length = length;
        	tmp.data = static_cast<int64_t *>(realloc(tmp.data, tmp.capacity * sizeof(int64_t)));

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
        		tmp.data = static_cast<int64_t *>(realloc(tmp.data, capacity * sizeof(int64_t)));
        		tmp.data[tmp.length - 1] = tmp.data[tmp.length - 2] / BASE;
        		tmp.data[tmp.length - 2] %= BASE;
        	}
        } else {
        	//other.length >= length
        	tmp.capacity = other.capacity;
        	tmp.length = other.length;
        	tmp.data = static_cast<int64_t *>(realloc(tmp.data, tmp.capacity * sizeof(int64_t)));

        	for (size_t i = 0; i < other.length; i++) {
        		tmp.data[i] = other.data[i];
        	}

        	for (size_t i = 0; i < length; i++) {
        		tmp.data[i] += data[i];
        	}

        	for (size_t i = 0; i < tmp.length - 1; i++) {
        		tmp.data[i + 1] += tmp.data[i] / BASE;
        		tmp.data[i] %= BASE;
        	}

        	if (tmp.data[tmp.length - 1] / BASE != 0) {
        		tmp.capacity *= 2;
        		tmp.length += 1;
        		tmp.data = static_cast<int64_t *>(realloc(tmp.data, tmp.capacity * sizeof(int64_t)));
        		tmp.data[tmp.length - 1] = tmp.data[tmp.length - 2] / BASE;
        		tmp.data[tmp.length - 2] %= BASE;
        	}
        }
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

	BigInt operator-() const
	{
		BigInt res;
		res = *this;
		if (*this != 0) {
			res.is_positive = !this->is_positive;
		}

		return res;
	}

	BigInt operator-(const BigInt& other) const
	{
		if (*this == other) {
			return 0;
		}

		if (!other.is_positive) {
			return (*this + (-other));
		}

		BigInt tmp;


		if (!is_positive) {
			tmp = -*this + other;
			tmp.is_positive = false;
			return tmp;
		}

		if (*this < other) {
			return -(other - *this);
		}

		tmp = *this;
		for (size_t i = 0; i < other.length; i++) {
			tmp.data[i] -= other.data[i];
		}

		for (size_t i = 0; i < other.length; i++) {
			if (tmp.data[i] < 0) {
				tmp.data[i + 1]--;
				tmp.data[i] += BASE;
			}
		}

		if (tmp.data[length - 1] == 0) {
			tmp.length--;
		}
		return tmp;
	}

	friend std::ostream& operator <<(std::ostream &os, const BigInt& c);

private:
	bool is_positive;
	size_t capacity;
	size_t length;
	int64_t *data;
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
		while (len < 18) { //9 or 10
			os << 0;
			len++;
		}

       	os << c.data[i];
    }
    
    return os;
}
