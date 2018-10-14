#include <iostream>
#include <cmath>
#include <string>
#include <exception>

#include "numbers.dat"

// Data - array
// Size - array size
// Data in [0, 10000], typeof(data[0]) == int

int count(int, int, const int*);
bool isPrime(int);
int findl(int, const int*);
int findr(int, const int*);

int main(int argc, char* argv[])
{
    try {
        if (argc % 2 == 0 || argc == 1) {
            throw std::invalid_argument("Usage: {begin end}");
        }
        
        for (int i = 1; i < argc; i+=2) {
            int v = count(std::atoi(argv[i]), std::atoi(argv[i + 1]), Data);
            std::cout << v << std::endl;
        }
    }
    catch (const std::invalid_argument& except) {
        std::cout << "Error:\n\t" << except.what() << std::endl;
        return -1;
    }
    return 0;
}

// counts prime numbers in Data
// searck between first "begin" value to last "end" value
int count(int begin, int end, const int* Data)
{
    int counter = 0;
    for (int i = findl(begin, Data); i <= findr(end, Data); ++i) {
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
int findl(int val, const int* Data)
{
    int beg = 0, end = Size;
    int mid = Size / 2;
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
        return Size + 1;
    }
}

// finds most right of the element val in Data
// if not found returns -1
// binsearch
int findr(int val, const int* Data)
{
    int beg = 0, end = Size;
    int mid = Size / 2;
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
        return -1;
    }
}
