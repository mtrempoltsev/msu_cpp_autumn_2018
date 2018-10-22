#include <cstdlib>
#include <iostream>
#include "numbers.dat"

bool isPrime(int num)
{
    if (num < 2)
        return false;
    for (int i = 2; i * i <= num; ++i)
        if ((num % i) == 0)
            return false;
    return true;
}

int getNumPrimes(int left, int right)
{
    // array index
    int i = 0;

    // count of primes
    int answer = 0;

    // going for the first value in range
    while (i < Size && Data[i] < left)
        ++i;

    // last value
    int lastValue = 0;

    // Last was prime
    int lastPrime = 0;

    // Number of consecutive prime numbers
    for(; i < Size && Data[i] <= right; ++i) {
        if (Data[i] == lastValue) {
            answer += lastPrime;
        } else {
            answer += lastPrime = isPrime(lastValue = Data[i]);
        }
    }
    // if we have been calculating primes, but we reached the end of array
    return answer;
}

int main(int argc, char ** argv)
{
    // check for correct number of arguments
    if (argc < 3 || (argc % 2 == 0))
        return -1;

    for (int il = 1, ir = 2; il < argc; il += 2, ir += 2) {
        int left = atoi(argv[il]);
        int right = atoi(argv[ir]);

        std::cout << getNumPrimes(left, right) << std::endl;
    }

	return 0;
}
