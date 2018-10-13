#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <assert.h>
#include "numbers.dat" 

//table of primes on [0, n]
class PrimeTable
{
public:
    PrimeTable(int _n): n(_n)
    {
        assert(n > 1);

        table.assign(n + 1, true);

        table[0] = false;
        table[1] = false;

        for (int i = 2; i * i <= n; i++)
            if (table[i])
                for (int j = i * i; j <= n; j += i)
                    table[j] = false;
    }

    bool operator[](size_t ind)
    {
        return table[ind];
    }

private:
    std::vector<bool> table;
    int n;

};


int primeCountArray(int lowerBound, int upperBound, int maxRightBound)
{
    static PrimeTable primeTable(maxRightBound);

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
    int ans = 0;
    for (; (*leftIter) <= upperBound && leftIter < Data + Size; leftIter++)
        ans += primeTable[(*leftIter)] ? 1 : 0;

    return ans;
}

int main(int argc, char* argv[])
{
    const int maxRightBound = 1e5;
    
    //incorrect input
    if (argc == 1 || argc % 2 == 0)
        return -1;

    for (int i = 1; i < argc; ++i)
    {
        int lowerBound, upperBound;
        //incorrect input
        if (sscanf(argv[i], "%d", &lowerBound) != 1)
            return -1;

        ++i;
        //incorrect input
        if (sscanf(argv[i], "%d", &upperBound) != 1)
            return -1;

        std::cout << primeCountArray(lowerBound, upperBound, maxRightBound) << std::endl;
    }

    return 0;
}