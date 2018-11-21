#include <iostream>
#include "stdlib.h"
#include "string.h"
#include <algorithm>

using namespace std;

class List {
  
public:
    uint64_t* list_of_numbers;
    int length;
    List() : length(0), list_of_numbers() {}
    List(uint64_t number) {
        list_of_numbers = new uint64_t[1];
        list_of_numbers[0] = (uint64_t) number;
        length = 1;
    }
    List(const List &other) {
        list_of_numbers = new uint64_t[other.length];
        length = other.length;
    }
    List(uint64_t* list, int len) : length(len), list_of_numbers(list) {}
    ~List() {
        delete[] list_of_numbers;
    }
    uint64_t operator[](int ind) {
        return list_of_numbers[ind];
    }    

    List operator+(const List &other) const {
        uint64_t* list_local;
        int len;
        if (length >= other.length) {
            list_local = new uint64_t[length];
            len = length;
        } else {
            list_local = new uint64_t[other.length];
            len = other.length;
        }
        uint64_t reg = 0;
        for (int i = 0; i < len; ++i) {
            if ((i < length) && (i < other.length)) {
                list_local[i] = list_of_numbers[i] + other.list_of_numbers[i] + reg;
                uint64_t two = list_of_numbers[i] + reg;
                if ((two >= list_of_numbers[i]) && (list_local[i] >= two)) {
                    reg = 0;
                } else {
                    reg = 1;
                }
            } 
            if ((i < length) && (i >= other.length)) {
                list_local[i] = list_of_numbers[i] + reg;
                if (list_local[i] >= list_of_numbers[i]) {
                    reg = 0;
                } else {
                    reg = 1;
                }
            } 
            if ((i >= length) && (i < other.length)) {
                list_local[i] = other.list_of_numbers[i] + reg;
                if (list_local[i] >= other.list_of_numbers[i]) {
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
                result[i] = list_local[i];
            } 
            result[len] = (uint64_t) reg;
            len = len + 1;
        } else {
            result = new uint64_t[len];
            for (int i = 0; i < len; ++i) {
                result[i] = list_local[i];
            } 
        }
        delete[] list_local;
        List local(result, len);
        return local;
    } 
    List operator-(const List &other) const {
        uint64_t* list_local;
        int len;
        list_local = new uint64_t[length];
        len = length;
        uint64_t reg = 0;
        for (int i = 0; i < len; ++i) {
            if ((i < length) && (i < other.length)) {
                list_local[i] = list_of_numbers[i] - other.list_of_numbers[i] + reg;
                uint64_t two = list_of_numbers[i] + reg;
                if ((two <= list_of_numbers[i]) && (list_local[i] <= two)) {
                    reg = 0;
                } else {
                    reg = -1;
                }
            } 
            if ((i < length) && (i >= other.length)) {
                list_local[i] = list_of_numbers[i] + reg;
                if (list_local[i] <= list_of_numbers[i]) {
                    reg = 0;
                } else {
                    reg = -1;
                }
            } 
        }
        int ind = len - 1;
        int count = 0;
        while ((ind >=0) && (list_local[ind] == 0)) {
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
            result[i] = list_local[i];
        } 
        delete[] list_local;
        List local(result, len);
        return local;
    }
    List operator=(const List &other) {
        if (length == other.length) {
            for (int i = 0; i < length; ++i) {
                list_of_numbers[i] = other.list_of_numbers[i];
            } 
            return *this;
        }
        delete[] list_of_numbers;
        length = other.length;
        list_of_numbers = new uint64_t[length];
        for (int i = 0; i < length; ++i) {
            list_of_numbers[i] = other.list_of_numbers[i];
        }
        return *this;
    }
    bool operator==(const List &other) const {
        if (length != other.length) {
            return false;
        }
        for (int i = 0; i < length; ++i) {
            if (list_of_numbers[i] != other.list_of_numbers[i]) {
                return false;
            }
        }
        return true;
    }
    bool operator>=(const List &other) const {
        if (length > other.length) {
            return true;
        }
        if (length < other.length) {
            return false;
        }
        for (int i = length - 1; i >= 0; --i) {
            if (list_of_numbers[i] > other.list_of_numbers[i]) {
                return true;
            }
            if (list_of_numbers[i] < other.list_of_numbers[i]) {
                return false;
            }
        }
        return true;
    }
    bool operator>(const List &other) const {
        if (length > other.length) {
            return true;
        }
        if (length < other.length) {
            return false;
        }
        for (int i = length - 1; i >= 0; --i) {
            if (list_of_numbers[i] > other.list_of_numbers[i]) {
                return true;
            }
            if (list_of_numbers[i] < other.list_of_numbers[i]) {
                return false;
            }
        }
        return false;
    }
};

class BigInt {
    
public: 
    List listBigInt;
    bool minus;
    BigInt() : minus(false), listBigInt(0){}
    BigInt(const int64_t number) {
        minus = (number < 0);
        if (minus) {
            listBigInt = -number;
        }
        else {
            listBigInt = number;
        }
    }
    BigInt(const BigInt &other) {
        listBigInt = other.listBigInt;
        minus = other.minus;
    }
    BigInt(const List &other) : minus(false), listBigInt(other) {}
    BigInt operator+(const uint64_t other) {
        return *this + BigInt(other);
    }
    BigInt operator+(const BigInt &other) const {
        BigInt new_bigint = BigInt();
        if (minus == other.minus) {
            new_bigint.listBigInt = listBigInt + other.listBigInt;
            new_bigint.minus = minus;
            return new_bigint;
        }
        if (listBigInt >= other.listBigInt) {
            new_bigint.listBigInt = listBigInt - other.listBigInt;
            if (listBigInt == other.listBigInt) {
                new_bigint.minus = false;
            } else {
                new_bigint.minus = minus;
            }
        } else {
            new_bigint.listBigInt = other.listBigInt - listBigInt;
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
            new_bigint.listBigInt = listBigInt + other.listBigInt;
            new_bigint.minus = minus;
            return new_bigint;
        }
        if (listBigInt >= other.listBigInt) {
            new_bigint.listBigInt = listBigInt - other.listBigInt;
            if (listBigInt == other.listBigInt) {
                new_bigint.minus = false;
            } else {
                new_bigint.minus = minus;
            }
        } else {
            new_bigint.listBigInt = other.listBigInt - listBigInt;
            new_bigint.minus = not minus;
        }
        return new_bigint;
    }
    BigInt operator-() {
        BigInt other = BigInt();
        other.listBigInt = listBigInt;
        other.minus = not minus;
        return *this;
    }
    bool operator==(const BigInt &other) const {
        if (minus == other.minus) {
            return listBigInt == other.listBigInt;
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
            return other.listBigInt >= listBigInt;
        }
        return listBigInt >= other.listBigInt;
    }
    bool operator>(const BigInt &other) const {
        if (not minus && other.minus) {
            return true;
        }
        if (minus && not other.minus) {
            return false;
        }
        if (minus) {
            return other.listBigInt > listBigInt;
        }
        return listBigInt > other.listBigInt;
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
    for (int i = buf.listBigInt.length - 1; i >= 0; --i) {
        out << buf.listBigInt.list_of_numbers[i];
    }
    return out;
}

