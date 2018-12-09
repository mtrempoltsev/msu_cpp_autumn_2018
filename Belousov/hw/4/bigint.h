#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cmath>

int64_t const BASE = 1000000000000000000;

class BigInt {
public:
	BigInt() : _sign('+'), _size(1), _data(new int64_t [_size]) {
		_data[0] = 0;
	}

	BigInt(int64_t const & value) : _sign('+') {
		int64_t tmp = value;

		if (value < 0) {
			_sign = '-';
			tmp = -tmp;
		}

		if (value < BASE) {
			_size = 1;
			_data = new int64_t [1];
			_data[0] = tmp;
		} else {
			_size = 2;
			_data = new int64_t [2];
			_data[0] = tmp % BASE;
			_data[1] = tmp / BASE;
		}
	}

	BigInt(BigInt const & other)
		: _sign(other._sign), _size(other._size), _data(new int64_t [_size]) {
			for (size_t i = 0; i < _size; i++)
				_data[i] = other._data[i];
	}

	~BigInt() {delete [] _data;}

	BigInt & operator=(BigInt const & other) {
		if (this == &other)
			return *this;

		int64_t * tmp = new int64_t [other._size];
		delete [] _data;

		_data = tmp;
		_size = other._size;
		_sign = other._sign;
		for (size_t i = 0; i < _size; i++)
			_data[i] = other._data[i];

		return *this;
	}

	bool operator==(BigInt const & other) const {
		if (this == &other)
			return true;

		if (_sign != other._sign || _size != other._size)
			return false;

		for (size_t i = 0; i < _size; i++)
			if (_data[i] != other._data[i])
				return false;

		return true;
	}

	bool operator!=(BigInt const & other) const {
		return !(*this == other);
	}

	bool operator>(BigInt const & other) const {
		if (this == &other)
			return false;

		if (_sign == '+') {
			if (other._sign == '-' || _size > other._size)
				return true;

			if (_size	< other._size)
				return false;

			for (size_t i = 0; i < _size; i++)
				if (_data[i] > other._data[i])
					return true;

			return false;
		} else {
			if (other._sign == '+' || _size > other._size)
				return false;

			if (_size < other._size)
				return true;

			for (size_t i = 0; i < _size; i++)
				if (_data[i] < other._data[i])
					return true;

			return false;
		}

	}

	bool operator<(BigInt const & other) const {
		return !(*this == other || *this > other);
	}

	bool operator>=(BigInt const & other) const {
		return !(*this < other);
	}

	bool operator<=(BigInt const & other) const {
		return !(*this > other);
	}

	BigInt operator-() const {
		BigInt tmp(*this);

		if (tmp._size != 1 || tmp._data[0] != 0)
			tmp._sign == '-' ? tmp._sign = '+' : tmp._sign = '-';
		
		return tmp;
	}

	BigInt operator+(BigInt const & other) const {
		if (*this == BigInt(0))
			return other;
		if (other == BigInt(0))
			return *this;

		BigInt tmp;

		if (_sign == other._sign) {
			tmp = _size > other._size ? *this : other;
			size_t idx = 0;
			bool transfer = false;

			while (idx < _size && idx < other._size) {
				transfer ? transfer = false, tmp._data[idx] = 1 : tmp._data[idx] = 0;
				tmp._data[idx] = _data[idx] + other._data[idx];

				if (tmp._data[idx] >= BASE) {
					transfer = true;
					tmp._data[idx] %= BASE;
				}

				idx++;
			}

			while (idx < tmp._size) {
				transfer = false;
				if (tmp._data[idx] < BASE)
					break;
				else {
					tmp._data[idx] %= BASE;
					transfer = true;
				}
				idx++;
			}

			if (idx == tmp._size && transfer) {
				int64_t * tmp_ptr = tmp._data;
				tmp._data = new int64_t [++tmp._size];

				for (size_t i = 0; i < tmp._size - 1; i++)
					tmp._data[i] = tmp_ptr[i];

				tmp._data[tmp._size - 1] = 1;
				delete [] tmp_ptr;
			}
		} else {
			return *this - -other;
		}

		return tmp;
	}

	BigInt operator-(BigInt const & other) const {
		if (*this == BigInt(0))
			return -other;
		if (other == BigInt(0))
			return *this;

		BigInt tmp;
		int64_t is_first = 1;

		if (_sign == other._sign) {

			if (*this == other)
				return 0;

			if (_sign == '+') {
				if (*this > other)
					tmp = *this;
				else {
					is_first = -1;
					tmp = other;
					tmp._sign = '-';
				}
			}
			else {
				if (*this < other)
					tmp = *this;
				else {
					is_first = -1;
					tmp = other;
					tmp._sign = '+';
				}
			} 

			size_t idx = 0;

			while (idx < _size && idx < other._size) {
				tmp._data[idx] = is_first * (_data[idx] - other._data[idx]);

				if (tmp._data[idx] < 0) {
					tmp._data[idx] += BASE;
					tmp._data[idx + 1] = -1; 
				}

				idx++;
			}

		} else
			return *this + -other;

		return tmp;
	}

	std::ostream & show(std::ostream & out) const {
		bool is_null = true;
		for (size_t i = 0; i < _size; i++) {
			if (_data[i] != 0) {
				is_null = false;
				break;
			}
		}

		if (is_null) {
			out << 0;
			return out;
		}

		if (_sign == '-')
			out << _sign;

		size_t real_size = _size - 1;
		while (_data[real_size] == 0)
			real_size--;

		out << _data[real_size];

		size_t base_number = number_counter(BASE) - 1;

		for (size_t i = 1; i <= real_size; i++) {
			for (size_t j = 0; j < base_number - number_counter(_data[real_size - i]); j++)
				out << 0;

			out << _data[real_size - i];
		}

		return out;
	}

	size_t size() const {return _size;}


private:
	char _sign;
	size_t _size;
	int64_t * _data;

	size_t number_counter(int64_t value) const {
		size_t counter = 1;
		int64_t divitor = 10;

		while (value / divitor != 0) {
			counter++;
			divitor *= 10;
		}

		return counter;
	}
};

BigInt operator+(int x, BigInt const & y) {
	return y + x;
}

std::ostream & operator<<(std::ostream & out, BigInt const & value) {
	return value.show(out);
}
