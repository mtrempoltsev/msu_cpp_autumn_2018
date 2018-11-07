#include <iostream>
#include <cstring>
#include <cstdlib>

using namespace std;

class BigInt{
	char *number;
	size_t size;
	int sign;
public:
	BigInt(int64_t numb = 0)
		: number()
		, sign(1)
		, size(0) {
		if (numb < 0) {
			sign = -1;
			numb *= -1;
		}
		else
			sign = 1;
		string s_number = to_string(numb);
		size = s_number.size();
		number = new char[size + 1];
		memcpy(number, s_number.c_str(), size + 1);
	}
	BigInt(const BigInt& other)
		: number()
		, sign(other.sign)
		, size(other.size) {
		number = new char[size + 1];
		memcpy(number, other.number, size + 1);
	}		
	BigInt(BigInt&& other) {
		number = other.number;
		sign = other.sign;
		size = other.size;
		other.number = nullptr;
	}
	~BigInt() {
		if (number != nullptr)
			delete[] number;
	}
	bool operator==(const BigInt& other) const {
		if (this == &other)
			return true;
		if ((!(strcmp(number, "0"))) && (!(strcmp(number, other.number))))
			return true;
		if ((sign != other.sign) || (size != other.size))
			return false;
		if (!strcmp(number, other.number))
			return true;
		return false;
	}
	bool operator!=(const BigInt& other) const {return !(*this == other);}
	bool operator>(const BigInt& other) const {
		if (sign != other.sign)
			return (sign == 1);
		if ((size > other.size)  ||
			((size == other.size) && (strcmp(number, other.number) > 0)))
			return (sign == 1);
		if ((other.size > size)  ||
			((size == other.size) && (strcmp(other.number, number) > 0)))
			return (sign == -1);
	}
	bool operator>=(const BigInt& other) const {return ((*this > other) || (*this == other));}
	bool operator<(const BigInt& other) const {return !(*this >= other);}
	bool operator<=(const BigInt& other) const {return !(*this > other);}
	BigInt& operator=(const BigInt& other) {
		if (this == &other)
			return *this;
		if (number != nullptr)
			delete[] number;
		size = other.size;
		sign = other.sign;
		number = new char[size + 1];
		memcpy(number, other.number, size + 1);
		return *this;
	}
	BigInt& operator=(BigInt&& other) {
		if (this == &other)
			return *this;
		if (number != nullptr)
			delete[] number;
		size = other.size;
		sign = other.sign;
		number = other.number;
		other.number = nullptr; 
		return *this;
	}	
	friend ostream& operator<<(ostream& stream, const BigInt& numb);
	BigInt operator-() const{
		BigInt result(*this);
		result.sign *= -1;
		return result;
	}
	BigInt operator+(const BigInt& other) const {
		if (sign != other.sign) {
			if (sign == 1)
				return (*this - (-other));
			else
				return (other - (-(*this)));
		}
		BigInt result;
		result.sign = sign;
		BigInt op1;
		BigInt op2;
		size_t min_size;
		if (size > other.size){
			result.size = size;
			min_size = other.size;
			op1 = *this;
			op2 = other;
		} else {
			result.size = other.size;
			min_size = size;
			op1 = other;
			op2 = *this;
		}
		char *s_number = new char[result.size + 2];
		int more = 0;
		for (size_t i = 0; i < result.size; i ++){
			s_number[result.size - i] = op1.number[result.size - i - 1]  + more;
			more = 0;
			if (i < min_size)
				s_number[result.size - i] += op2.number[min_size - i - 1] - '0';
			if (s_number[result.size - i] > '9'){
				more = 1;
				s_number[result.size - i] -= 10;
			}
		}
		s_number[result.size + 1] = '\0';
		if (more){
			result.size++;
			s_number[0] = '1';
		}
		delete[] result.number;
		result.number = new char[result.size + 1];
		if (more)
			memcpy(result.number, s_number, result.size + 1);
		else
			memcpy(result.number, s_number + 1, result.size + 1);
		delete[] s_number;
		return result;
	}
	BigInt operator-(const BigInt& other) const{
		if (sign != other.sign)
			return ((*this) + (-other));
		BigInt result;
		BigInt op1;
		BigInt op2;
		size_t max_size = size;
		if ((size > other.size) ||
		((size == other.size) && (strcmp(number, other.number) > 0))){
			result.sign = sign;
			op1 = *this;
			op2 = other;
		}
		else {
			if ((size < other.size) ||
			((size == other.size) && (strcmp(number, other.number) < 0))){
				result.sign = -1 * other.sign;
				op1 = other;
				op2 = *this;
			}
			else
				return result;
		}
		char *s_number = new char[op1.size + 1];
		int more = 0;
		for (size_t i; i < op1.size; i ++){
			s_number[op1.size - i - 1] = op1.number[op1.size - i - 1] - more;
			more = 0;
			if (i < op2.size)
				s_number[op1.size - i - 1] -= (op2.number[op2.size - i -1] - '0'); 
			if (s_number[op1.size - i - 1] < '0'){
				more = 1;
				s_number[op1.size - i - 1] += 10;
			}
		}
		s_number[op1.size] = '\0';
		size_t i = 0;
		for (i = 0; i < op1.size; i ++){
			if (s_number[i] != '0')
				break;
		}
		result.size = op1.size - i;
		delete[] result.number;
		result.number = new char [op1.size + 1 - i];
		memcpy(result.number, s_number + i, op1.size + 1 - i);
		delete[] s_number;
		return result;
	}
};

ostream& operator<<(ostream& stream, const BigInt& numb) {
	if (!numb.size)
		return stream;
	if (numb.sign == -1)
		stream << "-";
	for(int i = 0; i < numb.size; ++i)
		stream << numb.number[i];
	return stream;
}
