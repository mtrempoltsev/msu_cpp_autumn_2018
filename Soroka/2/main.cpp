#include "numbers.dat"

#include <iostream>
#include <algorithm>


bool isPrime(size_t a)
{
    if (a < 2) return false;

    for (size_t i = 2; i * i <= a; ++i)
    {
        if (a % i == 0) {
            return false;
        }
    }
    return true;
}


int main(int argc, char* argv[])
{
    if(!(argc & 1) || argc < 2) return -1;
    for (int i = 1; i < argc; i += 2)
    {
        int left = std::atoi(argv[i]), right = std::atoi(argv[i + 1]);
        int prime_count = 0;

        if(left > right)
        {
            std::cout << 0 << std::endl;
            return 0;
        }

        auto start = std::lower_bound(Data, Data + Size, left);
        auto end1 = std::upper_bound(Data, Data + Size, right);

        if(*start == Data[Size - 1] || (*end1 == Data[Size - 1] && Data[Size - 2] != right)) return -1;

        for(auto it = start; it != end1; ++it)
        {
            if(isPrime(*it)) prime_count++;
        }
        std::cout << prime_count << std::endl;
    }
    return 0;
}
