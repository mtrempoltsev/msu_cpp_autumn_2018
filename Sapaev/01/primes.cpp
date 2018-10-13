#include <iostream>
#include <cmath>
#include <string>

#include "numbers.dat"

// Data - array
// Size - array size
// Data in [0, 10000], typeof(data[0]) == int

int count(int, int);
bool isPrime(int);
int findl(int);
int findr(int);

int main(int argc, char* argv[])
{
    try {
        if (argc % 2 == 0 || argc == 1) {
            throw "Usage: {start, end}";
        }
        
        for (int i = 1; i < argc; i+=2) {
            int v = count(std::atoi(argv[i]), std::atoi(argv[i + 1]));
            std::cout << v << std::endl;
        }
    }
    catch (const char* str) {
        std::cout << "Error:\n\t" << str << std::endl;
        return -1;
    }
    return 0;
}

// counts prime numbers in Data
// searck between first "begin" value to last "end" value
int count(int begin, int end)
{
    int counter = 0;
    for (int i = findl(begin); i <= findr(end); ++i) {
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
int findl(int val)
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
int findr(int val)
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
