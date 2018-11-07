#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <memory>
#include <exception>

template <typename T>
class MyVector {
private:
    size_t size;
    size_t capacity;
    T* data;
private:
    bool is_valid_idx(size_t i) const;
public:
    using iterator = T*;
    MyVector();
    MyVector(size_t size, const T& default_elem);
    const T& operator[] (size_t i) const;
    T& operator[] (size_t i);
    size_t get_size() const;
    void push_back(const T& elem);
    void pop_back();
    T& back();
    MyVector (const MyVector<T>& copied);
    MyVector<T>& operator=(const MyVector<T>& copied);
    MyVector (MyVector<T>&& moved);
    MyVector<T>& operator=(MyVector<T>&& moved);
    bool operator==(const MyVector<T>& rhs);
    bool operator!=(const MyVector<T>& rhs);
    ~MyVector();
};

template <typename T>
MyVector<T>::MyVector() : size(0), capacity(0), data(nullptr) {}

template <typename T>
MyVector<T>::MyVector(size_t size_, const T& default_elem) {
    size = size_;
    capacity = size;
    data = new T[size];
    for (size_t i = 0; i < size; i++) {
        data[i] = default_elem;
    }
}

template <typename T>
bool MyVector<T>::is_valid_idx(size_t i) const {
    return i < size;
}

template <typename T>
const T& MyVector<T>::operator[] (size_t i) const {
    if (!is_valid_idx(i))
        throw std::out_of_range("");
    return data[i];
}

template <typename T>
T& MyVector<T>::operator[] (size_t i) {
    if (!is_valid_idx(i))
        throw std::out_of_range("");
    return data[i];
}

template <typename T>
size_t MyVector<T>::get_size() const {
    return size;
}

template <typename T>
void MyVector<T>::push_back(const T& elem) {
    if (size > capacity) {
        capacity *= 2;
        T * new_data = new T[capacity];
        std::copy(data, data + size, new_data);
        delete [] data;
        data = new_data;
    }
    data[size] = elem;
    size++;
}

template<typename T>
void MyVector<T>::pop_back() {
    size--;
}

template<typename T>
T& MyVector<T>::back() {
    return data[size - 1];
}

template<typename T>
bool MyVector<T>::operator==(const MyVector<T>& rhs) {
    if (this->size != rhs.size) {
        return false;
    }
    for (size_t i = 0; i < rhs.size; i++) {
        if (data[i] != rhs.data[i])
            return false;
    }
    return true;
}

template<typename T>
bool MyVector<T>::operator!=(const MyVector<T>& rhs) {
    return !(*this == rhs); 
}

template<typename T>
MyVector<T>::MyVector(const MyVector<T>& copied) : size(copied.size), capacity(copied.capacity), data(new T[copied.capacity]) {
    std::copy(copied.data, copied.data + capacity, data);
}

template<typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector<T>& copied) {
    if (this == &copied)
        return *this;
    char* new_data = new T[copied.capacity];
    delete[] data;
    data = new_data;
    size = copied.size;
    capacity = copied.capacity;
    std::copy(copied.data, copied.data + capacity, data);
    return *this;
}

template<typename T>
MyVector<T>::MyVector (MyVector<T>&& moved) : size(moved.size), capacity(moved.capacity), data(moved.data) {
    moved.data = nullptr;
    moved.size = 0;
    moved.capacity = 0;
}

template<typename T>
MyVector<T>& MyVector<T>::operator=(MyVector<T>&& moved) {
    if (this == &moved)
        return *this;
    delete[] data;
    data = moved.data;
    size = moved.size;
    capacity = moved.capacity;
    moved.data = nullptr;
    moved.size = 0;
    moved.capacity = 0;
    return *this;
}

template<typename T>
MyVector<T>::~MyVector() {
    delete[] data;
}

class BigInt {
private:
    static const size_t order = 9;
    static const size_t base = static_cast<size_t>(pow(10, order));
    bool is_negative;
    std::vector<int> data;
private:
    static void suppress_zeros(std::vector<int>& v);
    BigInt(const std::vector<int>& array);
    static BigInt add_abs_values(const BigInt& lhs, const BigInt& rhs);
    static BigInt subtract_abs_values(const BigInt& lhs, const BigInt& rhs);
    static bool less_abs_value(const BigInt& lhs, const BigInt& rhs);
    void set_negative(bool negative);
    bool is_zero() const;
public:
    explicit BigInt(const std::string& s);
    explicit BigInt(const char* s) : BigInt(std::string(s)) {};
    template<typename T>
    BigInt(T num) : BigInt(std::to_string(num)) {};
    BigInt();
    BigInt operator-() const;
    friend std::ostream& operator<<(std::ostream& out, const BigInt& value);
    friend BigInt operator+(const BigInt& lhs, const BigInt& rhs);
    friend BigInt operator-(const BigInt& lhs, const BigInt& rhs);
    friend bool operator==(const BigInt& lhs, const BigInt& rhs);
    friend bool operator!=(const BigInt& lhs, const BigInt& rhs);
    friend bool operator<(const BigInt& lhs, const BigInt& rhs);
    friend bool operator>(const BigInt& lhs, const BigInt& rhs);
    friend bool operator>=(const BigInt& lhs, const BigInt& rhs);
    friend bool operator<=(const BigInt& lhs, const BigInt& rhs);
    BigInt(BigInt&&) = default;
    BigInt& operator=(BigInt&&) = default;
    BigInt(const BigInt&) = default;
    BigInt& operator=(const BigInt&) = default;
};

BigInt::BigInt(const std::string& s) {
    if (s.empty()) {
        data.push_back(0);
        set_negative(false);
    } else {
        for (int i = static_cast<int>(s.length()); i > 0; i -= order) {
            if (i < static_cast<int>(order)) {
                try {
                    data.push_back(std::stoi(s.substr(0, i)));
                } catch (const std::invalid_argument& ex) {
                    if (s.substr(0, i) == "-") {
                        data.back() *= -1;
                    } else {
                        throw ex;
                    }
                }
            }
            else {
                data.push_back(std::stoi(s.substr(i - order, order)));
            }
        }
        is_negative = data.back() < 0;
        data.back() = abs(data.back());
    }
}

BigInt::BigInt() : is_negative(false) {
    data.push_back(0);
}

BigInt::BigInt(const std::vector<int>& array) {
    data = array;
    is_negative = data.back() < 0;
    data.back() = abs(data.back());
}

bool BigInt::less_abs_value(const BigInt& lhs, const BigInt& rhs) {
    if (lhs.data.size() != rhs.data.size())
        return lhs.data.size() < rhs.data.size();
    for (size_t i = lhs.data.size() - 1; i >= 0; i--) {
        if (lhs.data[i] != rhs.data[i]) {
            return lhs.data[i] < rhs.data[i];
        }
    }
    return false;
}

BigInt BigInt::add_abs_values(const BigInt& lhs, const BigInt& rhs) {
    size_t max_size = std::max(lhs.data.size(), rhs.data.size());
    std::vector<int> res_array (max_size, 0);
    int carry = 0;
    for (size_t i = 0; i < max_size || carry; i++) {
        if (i == res_array.size())
            res_array.push_back (0);
        res_array[i] = lhs.data[i] + carry + (i < rhs.data.size() ? rhs.data[i] : 0);
        carry = res_array[i] >= static_cast<int>(base);
        if (carry) res_array[i] -= base;
    }
    return BigInt(res_array);
}

BigInt BigInt::subtract_abs_values(const BigInt& lhs, const BigInt& rhs) {
    int carry = 0;
    size_t max_size = std::max(lhs.data.size(), rhs.data.size());
    std::vector<int> res_array (max_size, 0);
    for (size_t i = 0; i< max_size || carry; i++) {
        res_array[i] = lhs.data[i] - (carry + (i < rhs.data.size() ? rhs.data[i] : 0));
        carry = res_array[i] < 0;
        if (carry) res_array[i] += base;
    }
    suppress_zeros(res_array);
    return BigInt(res_array);
}

bool BigInt::is_zero() const {
    return this->data.back() == 0;
}

void BigInt::set_negative(bool negative) {
    if (this->is_zero()) {
        is_negative = false;
    } else {
        is_negative = negative;
    }
}

void BigInt::suppress_zeros(std::vector<int>& v) {
    while (v.size() > 1 && v.back() == 0)
        v.pop_back();
}

BigInt operator+(const BigInt& lhs, const BigInt& rhs) {
    BigInt result;
    if (!lhs.is_negative && !rhs.is_negative) {
        result = BigInt::add_abs_values(lhs, rhs);
    } else if (lhs.is_negative && rhs.is_negative) {
        result = -BigInt::add_abs_values(lhs, rhs);
    } else {
        if (BigInt::less_abs_value(lhs, rhs)) {
            result = BigInt::subtract_abs_values(rhs, lhs);
            result.set_negative(rhs.is_negative);
        } else {
            result = BigInt::subtract_abs_values(lhs, rhs);
            result.set_negative(lhs.is_negative);
        }
        
    }
    return result;
}

BigInt operator-(const BigInt& lhs, const BigInt& rhs) {
    return lhs + (-rhs);
}

BigInt BigInt::operator-() const {
    BigInt tmp(*this);
    if (this->is_zero()) {
        tmp.is_negative = false;
    } else {
        tmp.is_negative = !tmp.is_negative;
    }
    return tmp;
}

bool operator==(const BigInt& lhs, const BigInt& rhs) {
    if (rhs.data.size() != lhs.data.size()) return false;
    if (lhs.is_negative ^ rhs.is_negative) return false;
    for (size_t i = 0; i < lhs.data.size(); i++) {
        if (lhs.data[i] != rhs.data[i]) return false;
    }
    return true;
}

bool operator!=(const BigInt& lhs, const BigInt& rhs) {
    return !(lhs == rhs);
}

bool operator<(const BigInt& lhs, const BigInt& rhs) {
    if (lhs == rhs) return false;
    if (lhs.is_negative && rhs.is_negative) {
        return (-rhs < -lhs);
    } else if (!lhs.is_negative && rhs.is_negative) {
        return false;
    } else if (lhs.is_negative && !rhs.is_negative) {
        return true;
    }
    return BigInt::less_abs_value(lhs, rhs);
}

bool operator>=(const BigInt& lhs, const BigInt& rhs) {
    return !(lhs < rhs);
}

bool operator>(const BigInt& lhs, const BigInt& rhs) {
    return lhs >= rhs && lhs != rhs;
}

bool operator<=(const BigInt& lhs, const BigInt& rhs) {
    return lhs < rhs || lhs == rhs;
}

std::ostream& operator<<(std::ostream& out, const BigInt& value) {
    if (value.is_negative) out << "-";
    out << value.data.back();
    for (int i = value.data.size() - 2; i >= 0; i--) {
        out << std::setfill ('0') << std::setw(BigInt::order);
        out << value.data[i];
    }
    return out;
}
