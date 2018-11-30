#include <iostream>
#include "stdlib.h"
#include "string.h"
#include <algorithm>

using namespace std;

class Array {
  
public:
    uint64_t* array_of_numbers;
    int length;
    Array() : length(0), array_of_numbers() {}
    Array(uint64_t number) {
        array_of_numbers = new uint64_t[1];
        array_of_numbers[0] = (uint64_t) number;
        length = 1;
    }
    Array(const Array &other) {
        array_of_numbers = new uint64_t[other.length];
        length = other.length;
        for (int i = 0; i < length; ++i) {
            array_of_numbers[i] = other.array_of_numbers[i];
        }
    }
    Array(uint64_t* array, int len) : length(len), array_of_numbers(array) {}
    ~Array() {
        delete[] array_of_numbers;
    }
    uint64_t operator[](int ind) {
        return array_of_numbers[ind];
    }    
    Array operator=(const Array &other) {
        if (length == other.length) {
            for (int i = 0; i < length; ++i) {
                array_of_numbers[i] = other.array_of_numbers[i];
            } 
            return *this;
        }
        delete[] array_of_numbers;
        length = other.length;
        array_of_numbers = new uint64_t[length];
        for (int i = 0; i < length; ++i) {
            array_of_numbers[i] = other.array_of_numbers[i];
        }
        return *this;
    }
    bool operator==(const Array &other) const {
        if (length != other.length) {
            return false;
        }
        for (int i = 0; i < length; ++i) {
            if (array_of_numbers[i] != other.array_of_numbers[i]) {
                return false;
            }
        }
        return true;
    }
    bool operator>=(const Array &other) const {
        if (length > other.length) {
            return true;
        }
        if (length < other.length) {
            return false;
        }
        for (int i = length - 1; i >= 0; --i) {
            if (array_of_numbers[i] > other.array_of_numbers[i]) {
                return true;
            }
            if (array_of_numbers[i] < other.array_of_numbers[i]) {
                return false;
            }
        }
        return true;
    }
    bool operator>(const Array &other) const {
        if (length > other.length) {
            return true;
        }
        if (length < other.length) {
            return false;
        }
        for (int i = length - 1; i >= 0; --i) {
            if (array_of_numbers[i] > other.array_of_numbers[i]) {
                return true;
            }
            if (array_of_numbers[i] < other.array_of_numbers[i]) {
                return false;
            }
        }
        return false;
    }
};

class BigInt {
    
public: 
    Array arrayBigInt;
    bool minus;
    BigInt() : minus(false), arrayBigInt(0){}
    BigInt(const int64_t number) {
        minus = (number < 0);
        if (minus) {
            arrayBigInt = -number;
        }
        else {
            arrayBigInt = number;
        }
    }
    BigInt(const BigInt &other) {
        arrayBigInt = other.arrayBigInt;
        minus = other.minus;
    }
    BigInt(const Array &other) : minus(false), arrayBigInt(other) {}
    BigInt operator+(const uint64_t other) {
        return *this + BigInt(other);
    }
    Array oper_plus(const Array arr1, const Array arr2) const {
        uint64_t* array_local;
        int len;
        if (arr1.length >= arr2.length) {
            array_local = new uint64_t[arr1.length];
            len = arr1.length;
        } else {
            array_local = new uint64_t[arr2.length];
            len = arr2.length;
        }
        uint64_t reg = 0;
        for (int i = 0; i < len; ++i) {
            if ((i < arr1.length) && (i < arr2.length)) {
                array_local[i] = arr1.array_of_numbers[i] + arr2.array_of_numbers[i] + reg;
                uint64_t two = arr1.array_of_numbers[i] + reg;
                if ((two >= arr1.array_of_numbers[i]) && (array_local[i] >= two)) {
                    reg = 0;
                } else {
                    reg = 1;
                }
            } 
            if ((i < arr1.length) && (i >= arr2.length)) {
                array_local[i] = arr1.array_of_numbers[i] + reg;
                if (array_local[i] >= arr1.array_of_numbers[i]) {
                    reg = 0;
                } else {
                    reg = 1;
                }
            } 
            if ((i >= arr1.length) && (i < arr2.length)) {
                array_local[i] = arr2.array_of_numbers[i] + reg;
                if (array_local[i] >= arr2.array_of_numbers[i]) {
                    reg = 0;
                } else {
                    reg = 1;
                }
            } 
        }
        uint64_t* result;
        if (reg != 0) {
            result = new uint64_t[len + 1];
            for (int i = 0; i < len; ++i) {
                result[i] = array_local[i];
            } 
            result[len] = (uint64_t) reg;
            len = len + 1;
        } else {
            result = new uint64_t[len];
            for (int i = 0; i < len; ++i) {
                result[i] = array_local[i];
            } 
        }
        delete[] array_local;
        Array local(result, len);
        return local;
    } 
    Array oper_minus(const Array arr1, const Array arr2) const {
        uint64_t* array_local;
        int len;
        array_local = new uint64_t[arr1.length];
        len = arr1.length;
        uint64_t reg = 0;
        for (int i = 0; i < len; ++i) {
            if ((i < arr1.length) && (i < arr2.length)) {
                array_local[i] = arr1.array_of_numbers[i] - arr2.array_of_numbers[i] + reg;
                uint64_t two = arr1.array_of_numbers[i] + reg;
                if ((two <= arr1.array_of_numbers[i]) && (array_local[i] <= two)) {
                    reg = 0;
                } else {
                    reg = -1;
                }
            } 
            if ((i < arr1.length) && (i >= arr2.length)) {
                array_local[i] = arr1.array_of_numbers[i] + reg;
                if (array_local[i] <= arr1.array_of_numbers[i]) {
                    reg = 0;
                } else {
                    reg = -1;
                }
            } 
        }
        int ind = len - 1;
        int count = 0;
        while ((ind >=0) && (array_local[ind] == 0)) {
            ++count;
            --ind;
        }
        uint64_t* result;
        if (count == len) {
            result = new uint64_t[1];
            len = 1;
        } else {
            result = new uint64_t[len - count];
            len = len - count;
        }
        for (int i = 0; i < len; ++i) {
            result[i] = array_local[i];
        } 
        delete[] array_local;
        Array local(result, len);
        return local;
    }
    BigInt operator+(const BigInt &other) const {
        BigInt new_bigint = BigInt();
        if (minus == other.minus) {
            new_bigint.arrayBigInt = oper_plus(arrayBigInt, other.arrayBigInt);
            new_bigint.minus = minus;
            return new_bigint;
        }
        if (arrayBigInt >= other.arrayBigInt) {
            new_bigint.arrayBigInt = oper_minus(arrayBigInt, other.arrayBigInt);
            if (arrayBigInt == other.arrayBigInt) {
                new_bigint.minus = false;
            } else {
                new_bigint.minus = minus;
            }
        } else {
            new_bigint.arrayBigInt = oper_minus(other.arrayBigInt, arrayBigInt);
            new_bigint.minus = other.minus;
        }
        return new_bigint;
    }
    BigInt operator-(const uint64_t other) const {
        return *this - BigInt(other);
    }
    BigInt operator-(const BigInt &other) const {
        BigInt new_bigint = BigInt();
        if (minus == (not other.minus)) {
            new_bigint.arrayBigInt = oper_plus(arrayBigInt, other.arrayBigInt);
            new_bigint.minus = minus;
            return new_bigint;
        }
        if (arrayBigInt >= other.arrayBigInt) {
            new_bigint.arrayBigInt = oper_minus(arrayBigInt, other.arrayBigInt);
            if (arrayBigInt == other.arrayBigInt) {
                new_bigint.minus = false;
            } else {
                new_bigint.minus = minus;
            }
        } else {
            new_bigint.arrayBigInt = oper_minus(other.arrayBigInt, arrayBigInt);
            new_bigint.minus = not minus;
        }
        return new_bigint;
    }
    BigInt operator-() {
        BigInt other = BigInt();
        other.arrayBigInt = arrayBigInt;
        other.minus = not minus;
        return *this;
    }
    bool operator==(const BigInt &other) const {
        if (minus == other.minus) {
            return arrayBigInt == other.arrayBigInt;
        }
        return false;
    }
    bool operator!=(const BigInt &other) const {
        return (not (*this == other));
    }
    bool operator>=(const BigInt &other) const {
        if (not minus && other.minus) {
            return true;
        }
        if (minus && not other.minus) {
            return false;
        }
        if (minus) {
            return other.arrayBigInt >= arrayBigInt;
        }
        return arrayBigInt >= other.arrayBigInt;
    }
    bool operator>(const BigInt &other) const {
        if (not minus && other.minus) {
            return true;
        }
        if (minus && not other.minus) {
            return false;
        }
        if (minus) {
            return other.arrayBigInt > arrayBigInt;
        }
        return arrayBigInt > other.arrayBigInt;
    }
    bool operator<(const BigInt &other) const {
        return not (*this >= other);
    }
    bool operator<=(const BigInt &other) const {
        return not (*this > other);
    }    
};

std::ostream& operator<<(std::ostream& out, const BigInt buf) {
    if (buf.minus) {
        out << "-";
    }
    for (int i = buf.arrayBigInt.length - 1; i >= 0; --i) {
        out << buf.arrayBigInt.array_of_numbers[i];
    }
    return out;
}

