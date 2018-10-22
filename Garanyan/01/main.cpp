#include <iostream>
#include <math.h>
#include "numbers.dat"

bool prime(int n)
{
    if (n <= 1)
    {
        return false;
    }

    for (int i = 2; i <= sqrt(n); i++)
    {
        if (n % i == 0)
        {
            return false;
        }
    }

    return true;
}

enum states
{
    BEFORE = 0,
    INSIDE = 1
};

int countPrimes(const int data[], const int Size, const int begin, const int end)
{
    states state = BEFORE;
    size_t counst = 0;
    for (size_t i = 0; i < Size; ++i)
    {
        switch (state)
        {
            case BEFORE:
                if (data[i] == begin)
                {
                    if (prime(data[i]))
                    {
                        ++counst;
                    }
                    state = INSIDE;
                }
                break;
            case INSIDE:
                if (prime(data[i]))
                {
                    ++counst;
                }
                if (data[i] == end && ((i < Size - 1 && data[i + 1] != end) || i == Size - 1))
                {
                    return counst;
                }
                break;
        }
    }
    if (state == INSIDE || state == BEFORE)
    {
        return 0;
    }
}


int main(int argc, char* argv[])
{
    if (argc > 2 && argc % 2 == 1)
    {
        for (int i = 1; i < argc - 1; i += 2)
        {
            const int begin = std::atoi(argv[i]);
            const int end = std::atoi(argv[i + 1]);
            std::cout << countPrimes(Data, Size, begin, end) << std::endl;
        }
        return 0;
    }
    else
    {
        return -1;
    }
}
