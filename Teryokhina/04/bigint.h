#include <iostream>
#include <string>

class BigInt{
    bool is_positive;
    int64_t *data;
    size_t size;

    static const size_t cell_size = 9;
    static const int64_t cell_max = 1000 * 1000 * 1000;

    BigInt add_abs_values(const BigInt& a, const BigInt& b) const;
    int64_t add_cells(const int64_t& a, const int64_t &b, int &carry) const;

    BigInt sub_abs_values(const BigInt& a, const BigInt& b) const;
    int64_t sub_cells(const int64_t& a, const int64_t &b, int &carry) const;

    bool compare_abs_values_on_less(const BigInt& a, const BigInt& b) const;

public:
    BigInt() : BigInt("0") {}
    BigInt(int64_t x) : BigInt(std::to_string(x)) {}
    BigInt(const std::string& x);

    BigInt(const BigInt& copied);
    BigInt& operator=(const BigInt& copied);

    ~BigInt() { delete[] data; }

    BigInt operator-() const;
    BigInt abs() const;
    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;

    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const { return !(*this == other); }

    bool operator<(const BigInt& other) const;
    bool operator>(const BigInt& other) const { return other < *this; }
    bool operator<=(const BigInt& other) const { return !(*this > other); }
    bool operator>=(const BigInt& other) const { return !(*this < other); }

    friend std::ostream& operator<<(std::ostream& stream, const BigInt& number);
};

BigInt::BigInt(const std::string& x){
    std::string str = x;
    this->is_positive = true;
    if(str[0] == '-'){
        this->is_positive = false;
        str = str.substr(1, str.size() - 1);
    }

    while (str.size() % this->cell_size != 0)
        str = '0' + str;

    this->size = str.size() / this->cell_size;
    this->data = new int64_t[this->size];

    for (size_t i = 0; i < this->size; ++i) {
        std::string tmp = str.substr(i * this->cell_size, this->cell_size);
        this->data[i] = std::atoi(tmp.c_str());
     }
}

BigInt::BigInt(const BigInt& copied){
    this->is_positive = copied.is_positive;
    this->size = copied.size;
    this->data = new int64_t[this->size];
    for (size_t i = 0; i < this->size; ++i)
        this->data[i] = copied.data[i];
}

BigInt& BigInt::operator=(const BigInt& copied){
    if (this == &copied)
        return *this;

    int64_t *data = new int64_t[this->size];
    delete[] this->data;

    this->is_positive = copied.is_positive;
    this->size = copied.size;
    this->data = data;
    for (size_t i = 0; i < this->size; ++i)
        this->data[i] = copied.data[i];
    return *this;
}

BigInt BigInt::operator-() const{
    BigInt tmp(*this);
    tmp.is_positive = !tmp.is_positive;
    return tmp;
}

BigInt BigInt::abs() const{
    BigInt tmp(*this);
    tmp.is_positive = true;
    return tmp;
}

int64_t BigInt::add_cells(const int64_t& a, const int64_t &b, int &carry) const{
    int64_t tmp = a + b + (int64_t)carry;
    if(tmp < this->cell_max)
        carry = 0;
    else{
        carry = 1;
        tmp %= this->cell_max;
    }
    return tmp;
}

BigInt BigInt::add_abs_values(const BigInt& a, const BigInt& b) const{
    // make a > b
    if(a < b)
        return add_abs_values(b, a);

    BigInt result(a); 
    int carry = 0;
    int cur_pos_a = a.size - 1, cur_pos_b = b.size - 1;

    while(cur_pos_b != -1){   
        result.data[cur_pos_a] = add_cells(a.data[cur_pos_a], b.data[cur_pos_b], carry);
         --cur_pos_a; --cur_pos_b;
    }

    while(cur_pos_a != -1){
        result.data[cur_pos_a] = add_cells(a.data[cur_pos_a], (int64_t)0, carry);
        --cur_pos_a;
    }

    if(carry == 1){
        int64_t *data = new int64_t[result.size + 1];
        data[0] = 1;
        for (size_t i = 0; i < result.size; ++i)
            data[i + 1] = result.data[i];
        delete[] result.data;
        result.size += 1;
        result.data = data;
    }
    return result;
}

BigInt BigInt::operator+(const BigInt& other) const{
    // a + b
    if(this->is_positive && (this->is_positive == other.is_positive)) 
        return add_abs_values(*this, other);
    // -a + -b
    if(this->is_positive == other.is_positive)
        return -(abs() + other.abs());
    // a + -b
    if(this->is_positive)
        return *this - other.abs();
    // -a + b
    return other - abs();
}

int64_t BigInt::sub_cells(const int64_t& a, const int64_t &b, int &carry) const{
    int64_t tmp = a - b - (int64_t)carry;
    if(tmp >= 0)
        carry = 0;
    else{
        carry = 1;
        tmp += this->cell_max;
    }
    return tmp;
}

BigInt BigInt::sub_abs_values(const BigInt& a, const BigInt& b) const{
    // make a > b
    if(a < b)
        return -sub_abs_values(b, a);

    BigInt result(a); 
    int carry = 0;
    int cur_pos_a = a.size - 1, cur_pos_b = b.size - 1;

    while(cur_pos_b != -1){
        result.data[cur_pos_a] = sub_cells(a.data[cur_pos_a], b.data[cur_pos_b], carry);
        --cur_pos_a; --cur_pos_b;
    }

    while(cur_pos_a != -1){
        result.data[cur_pos_a] = sub_cells(a.data[cur_pos_a], (int64_t)0, carry);
        --cur_pos_a;
    }
    // delete zeros
    for (size_t i = 0; i < result.size; ++i) {
        if (result.data[i] != 0)
            break;
        int64_t *data = new int64_t[result.size - i];
        for (size_t j = 0; j < result.size - i; ++j) {
            data[j] = result.data[j + i];
        }
        delete[] result.data;
        result.size -= i;
        result.data = data;
    }
    return result;
}

BigInt BigInt::operator-(const BigInt& other) const{
    // a - b
    if(this->is_positive && (this->is_positive == other.is_positive)) 
        return sub_abs_values(*this, other);
    // -a - -b
    if(this->is_positive == other.is_positive)
        return other.abs() - abs();
    // a - -b
    if(this->is_positive)
        return *this + other.abs();
    // -a - b
    return -(abs() + other);
}

bool BigInt::operator==(const BigInt& other) const{
    if((this->data[0] == 0) && (other.data[0] == 0))
        return true;
    if(this->size != other.size)
        return false;
    if(this->is_positive != other.is_positive)
        return false;
    for (size_t i = 0; i < this->size; ++i)
        if (this->data[i] != other.data[i])
            return false;
    return true;
}

bool BigInt::compare_abs_values_on_less(const BigInt& a, const BigInt& b) const{
    if(a.size < b.size)
        return true;
    if(a.size > b.size)
        return false;
    for(size_t i = 0; i < a.size; ++i){
        if(a.data[i] < b.data[i])
            return true;
        else if(a.data[i] > b.data[i])
            return false;
    }
    // a == b
    return false;

}

bool BigInt::operator<(const BigInt& other) const{
    if(this == &other)
        return false;
    // a > 0, b > 0
    if(this->is_positive && (this->is_positive == other.is_positive))
        return compare_abs_values_on_less(*this, other);
    // a < 0, b < 0
    if(this->is_positive == other.is_positive)
        return compare_abs_values_on_less(other, *this);
    // a > 0, b < 0
    if(this->is_positive)
        return false;
    // a < 0, b > 0
    return true;
}

std::ostream& operator<<(std::ostream& stream, const BigInt& number){
    if (number.is_positive == false)
        stream << '-';
    stream << std::to_string(number.data[0]);
    for (size_t i = 1; i < number.size; ++i) {
        std::string tmp = std::to_string(number.data[i]);
        while (tmp.size() % number.cell_size != 0)
            tmp = '0' + tmp;
        stream << tmp;
    }
    return stream;
}
