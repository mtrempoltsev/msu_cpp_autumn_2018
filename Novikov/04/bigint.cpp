//Реализовать оператор вывода в поток, сложение, вычитание, унарный минус, все операции сравнения.
using namespace std;

class BigInt {
private:
	bool _is_negative = false;
	char * _all_digits;
	void add_to_end(char digits);
	size_t quantity = 64;
	size_t position = 0;

public:
	BigInt();
	BigInt(int64_t number);
	BigInt(const BigInt& number);
	BigInt kill_minus() const;
	BigInt operator-(const BigInt& number) const;
	BigInt operator-() const;
	BigInt& operator=(const BigInt& number);
	bool operator!=(const BigInt& number) const;
	bool operator==(const BigInt& number) const;
	bool operator>(const BigInt& number) const;
	bool operator>=(const BigInt& number) const;
	bool operator<(const BigInt& number) const;
	bool operator<=(const BigInt& number) const;
	friend ostream& operator<<(std::ostream& out, const BigInt& number);
	BigInt operator+(const BigInt& number) const;
	~BigInt();
};

BigInt::BigInt() {
	_all_digits = new char[quantity];
	add_to_end(0);
}

BigInt::BigInt(const BigInt& number) : position(number.position), _is_negative(number._is_negative), quantity(number.quantity) {
	_all_digits = new char[quantity];
	for(size_t i = 0; i < quantity; i++) 
		_all_digits[i] = number._all_digits[i];
}

void BigInt::add_to_end(char number) {
	_all_digits[position] = number;
	++position;
	if(position == quantity) {
		quantity = 2 * quantity;
		char* temp = new char[quantity];
		for (size_t i = 0; i < quantity; i++) temp[i] = _all_digits[i];
		delete[] _all_digits;
		_all_digits = temp;
	}
}

BigInt& BigInt::operator=(const BigInt& number) {
	if(this == &number) return *this;
	position = number.position;
	quantity = number.quantity;
	_is_negative = number._is_negative;
	delete[] _all_digits;
	_all_digits = new char[quantity];
	memcpy(_all_digits, number._all_digits, position);
	return *this;
}

std::ostream& operator<<(std::ostream& out, const BigInt& number) {
	if(number._is_negative) out << '-';
	for(int i = number.position - 1; i != -1; i--)
		out << char('0' + number._all_digits[i]);
	return out;
}

bool BigInt::operator==(const BigInt& number) const {
	if(_is_negative != number._is_negative || position != number.position)
		return false;
	for(size_t i = 0; i < position; i++)
		if(_all_digits[i] != number._all_digits[i]) 
			return false;
	return true;
}

bool BigInt::operator!=(const BigInt& number) const {
	return !operator==(number);
}

bool BigInt::operator>(const BigInt& number) const {
	return !operator<(number) && operator!=(number);
}

bool BigInt::operator>=(const BigInt& number) const {
	return !operator<(number);
}

BigInt::BigInt(int64_t number) {
	if(number < 0) {
		_is_negative = true;
		number = (-1) * number;
	}
	_all_digits = new char[quantity];
	if(number == 0) {
		add_to_end(0);
		return;
	}
	for(size_t i = 0; number > 0; i++) {
		add_to_end(number%10);
		number = (number - number%10)/10;
	}
}

bool BigInt::operator<(const BigInt& number) const {
	if(_is_negative != number._is_negative)
		return _is_negative;
	if(position > number.position) return _is_negative;
	else if(position < number.position) return !_is_negative;
	else {
		for(int i = position - 1; i != -1; i--)
			if(_all_digits[i] > number._all_digits[i])
				return _is_negative;
			else if(_all_digits[i] < number._all_digits[i])
				return !_is_negative;
		return false;
	}
}

bool BigInt::operator<=(const BigInt& number) const {
	return !operator>(number);
}

BigInt BigInt::operator-(const BigInt& number) const {
	if(_is_negative != number._is_negative) {
		if(_is_negative) return -(kill_minus() + number);
		else return *this + number.kill_minus();
	}
	if(_is_negative) return number.kill_minus() + *this;
	if(*this < number) return -(number - *this);
	BigInt result(*this);
	char temp = 0;
	for(size_t i = 0; i < number.position; i++) {
		char r = result._all_digits[i] - number._all_digits[i] - temp;
		if(r < 0) {
			result._all_digits[i] = r + 10;
			temp = 1;
		}
		else {
			result._all_digits[i] = r;
			temp = 0;
		}
	}
	for(size_t i = number.position; i < result.position; i++) {
		char r = result._all_digits[i] - temp;
		if(r < 0) {
			result._all_digits[i] = r + 10;
			temp = 1;
		}
		else {
			result._all_digits[i] = r;
			temp = 0;
		}
	}
	for(size_t i = result.position - 1; i > 0; i--) {
		if(result._all_digits[i] > 0) break;
		result.position--;
	}

	if(result.position == 1 && result._all_digits[0] == 0)
		result._is_negative = false;
	return result;
}

BigInt BigInt::operator-() const {
	BigInt result(*this);
	if(position == 1 && _all_digits[0] == 0) return result;
	result._is_negative = !(_is_negative);
	return result;
}

BigInt BigInt::kill_minus() const {
	BigInt result(*this);
	result._is_negative = false;
	return result;
}

BigInt BigInt::operator+(const BigInt& number) const {
	BigInt result(*this);
	char temp = 0;
	if(number._is_negative && !_is_negative)
		return (*this) - number.kill_minus();
	else if(!number._is_negative && _is_negative)
		return number.kill_minus() - (*this).kill_minus();

	if(number.position == result.position) {
		for(size_t i = 0; i < number.position; i++) {
			char summ = result._all_digits[i] + number._all_digits[i] + temp;
			result._all_digits[i] = summ%10;
			temp = (summ - summ%10)/10;
		}
		if(temp) result.add_to_end(temp);
	}
	else if(number.position > result.position) {
		for(size_t i = result.position; i < number.position; i++)
			result.add_to_end(0);
		for(size_t i = 0; i < number.position; i++) {
			char summ = result._all_digits[i] + number._all_digits[i] + temp;
			result._all_digits[i] = summ%10;
			temp = (summ - summ%10)/10;
		}
		if(temp) result.add_to_end(temp);
	}
	else if(number.position < result.position) {
		for(size_t i = 0; i < number.position; i++) {
			char summ = result._all_digits[i] + number._all_digits[i] + temp;
			result._all_digits[i] = summ%10;
			temp = (summ - summ%10)/10;
		}
		for(size_t i = number.position; i < result.position; i++) {
			char summ = result._all_digits[i] + temp;
			result._all_digits[i] = summ%10;
			temp = summ/10;
		}
		if(temp) result.add_to_end(temp);
	}
	for(size_t i = result.position - 1; i > 0; i--) {
		if(result._all_digits[i] > 0) break;
		result.position--;
	}
	if(result.position == 1 && result._all_digits[0] == 0)
		result._is_negative = false;
	return result;
}

BigInt::~BigInt() {
	delete[] _all_digits;
}