#include <iostream>
#include <cmath>
#include <string>
#include <exception>

#include "numbers.dat"

// Data - array
// Size - array size
// Data in [0, 10000], typeof(data[0]) == int

int count(int, int, const int*, size_t);
bool isPrime(int);
size_t findl(int, const int*, size_t);
size_t findr(int, const int*, size_t);

int main(int argc, char* argv[])
{
    if (argc % 2 == 0 || argc == 1) {
        std::cout << "Usage: {begin end}" << std::endl;
        return -1;
    }
    
    for (int i = 1; i < argc; i+=2) {
        int v = count(std::atoi(argv[i]), std::atoi(argv[i + 1]), Data, Size);
        std::cout << v << std::endl;
    }
    return 0;
}

// counts prime numbers in Data
// searck between first "begin" value to last "end" value
int count(int begin, int end, const int* Data, size_t Size)
{
    int counter = 0;
    for (size_t i = findl(begin, Data, Size); i <= findr(end, Data, Size); ++i) {
        if (isPrime(Data[i])) {
            ++counter;
        }
    }
    return counter;
}

// check for val being prime number
bool isPrime(int val)
{
    if (val == 0 || val == 1) {
        return false;
    }
    for (int i = 2; i < round(sqrt(val)) + 1; ++i) {
        if (val % i == 0) {
            return false;
        }
    }
    return true;
}

// finds most left of the element val in Data
// if not found returns Size + 1
// binsearch
size_t findl(int val, const int* Data, size_t Size)
{
    size_t beg = 0, end = Size;
    size_t mid = Size / 2;
    while (end - beg > 1) {
        if (Data[mid] >= val) {
            end = mid;
        } else {
            beg = mid;
        }
        mid = (end + beg) / 2;
    }
    
    if (Data[mid + 1] == val) {
        return mid + 1;
    } else {
        return Size;
    }
}

// finds most right of the element val in Data
// if not found returns -1
// binsearch
size_t findr(int val, const int* Data, size_t Size)
{
    size_t beg = 0, end = Size;
    size_t mid = Size / 2;
    while (end - beg > 1) {
        if (Data[mid] > val) {
            end = mid;
        } else {
            beg = mid;
        }
        mid = (end + beg) / 2;
    }
    
    if (Data[mid] == val) {
        return mid;
    } else {
        return 0;
    }
}
