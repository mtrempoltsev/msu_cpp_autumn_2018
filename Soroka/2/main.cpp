#include "numbers.dat"

#include <iostream>
#include <algorithm>


int Prime(const int a, const int i)
{
    if (a == 1) return 0;
    return i*i <= a ? (a%i) && Prime(a, i + 1) : 1;
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

        if(!(std::binary_search(Data, Data + Size, left)) || !(std::binary_search(Data, Data + Size, right))) return -1;

        auto start = std::lower_bound(Data, Data + Size, left);
        auto end = std::upper_bound(Data, Data + Size, right);

        for(auto it = start; it != end; ++it)
        {
            if(Prime(*it, 2)) prime_count++;
        }
        std::cout << prime_count << std::endl;
    }
    return 0;
}
