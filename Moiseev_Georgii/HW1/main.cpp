#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <assert.h>
#include <cstdint>
#include <memory>
#include "numbers.dat" 


class PrimeTable
{
//table of primes on [0, n]
//table[i] equals 1 if prime; equals 0 if composite or 0, 1
public:
    PrimeTable(int32_t _n): n(_n)
    {
        assert(n > 1);

        table.assign(n + 1, 1);

        table[0] = 0;
        table[1] = 0;

        for (int32_t i = 2; i * i <= n; i++)
            if (table[i] == 1)
                for (int32_t j = i * i; j <= n; j += i)
                    table[j] = 0;
    }

    int8_t operator[](size_t ind) const
    {
        return table[ind];
    }

private:
    std::vector<int8_t> table;
    int32_t n;

};


int32_t primeCountArray(int32_t lowerBound, int32_t upperBound, const PrimeTable& primeTable)
{
    if (lowerBound > upperBound)
        return 0;

    auto leftIter = std::lower_bound(Data, Data + Size, lowerBound);

    //lowerBound is not in array
    if (leftIter >= Data + Size)
        return 0;
    if ((*leftIter) != lowerBound)
        return 0;

    //upperBound is not in array
    if (std::binary_search(Data, Data + Size, upperBound) == false)
        return 0;

    //now we know that upperBound exists in array
    int32_t ans = 0;
    for (; (*leftIter) <= upperBound && leftIter < Data + Size; leftIter++)
        ans += primeTable[(*leftIter)];

    return ans;
}

int main(int argc, char* argv[])
{
    const int32_t maxRightBound = 1e5;
    PrimeTable primeTable(maxRightBound);

    //incorrect input
    if (argc == 1 || argc % 2 == 0)
        return -1;

    for (size_t i = 1; i < argc; ++i)
    {
        int32_t lowerBound, upperBound;
        //incorrect input
        if (sscanf(argv[i], "%d", &lowerBound) != 1)
            return -1;

        ++i;
        //incorrect input
        if (sscanf(argv[i], "%d", &upperBound) != 1)
            return -1;

        std::cout << primeCountArray(lowerBound, upperBound, primeTable) << std::endl;
    }

    return 0;
}