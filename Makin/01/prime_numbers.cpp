#include <iostream>
#include "numbers.dat"
// Data - массив
// Size - размер массива

void eratosthene_sieve(bool* prime_numbers)
{
    for (unsigned i = 0; i < Size; i++)
        prime_numbers[i] = false;
    prime_numbers[1] = true;
    for (unsigned i = 4; i < Size; i += 2)
        prime_numbers[i] = true;
    for (unsigned i = 3; i*i < Size; i += 2)
    {
        if (!prime_numbers[i])
            for (unsigned j = i*i; j < Size; j += i)
                prime_numbers[j] = true;
    }
}

unsigned binary_search(unsigned n, bool begin) 
{
    unsigned left = 0, right = Size - 1, mid;
    while (left < right) {
        mid = (right + left) / 2;
        if (Data[mid] == n) {
            if (begin) {
                if (mid == 0 || Data[mid - 1] != n)
                    return mid;
                else {
                    right = mid;
                    continue;
                }
            }
            else {
                if (mid == Size - 1 || Data[mid + 1] != n)
                    return mid;
                else {
                    left = mid + 1;
                    continue;
                }
            }
        }
        else if (Data[mid] < n) 
            left = mid + 1;
        else
            right = mid;
    }
    if (Data[left] == n)
        return left;
    else
        return -1;
}

int main(int argc, char* argv[]) 
{

    if (argc % 2 != 1 || argc == 1)
        return -1;
    
    bool*prime_numbers = new bool[Size];
    eratosthene_sieve(prime_numbers);
    
    unsigned l_indices, r_indices;
    for (int i = 1; i < argc; i += 2)
    {
        unsigned left = std::atoi(argv[i]);
        unsigned right = std::atoi(argv[i + 1]);
        if (right < left) 
        {
            std::cout << 0 << std::endl;
            continue;
        }

        l_indices = binary_search(left, true);
        r_indices = binary_search(right, false);

        if (l_indices == -1 || r_indices == -1) {
            std::cout << 0 << std::endl;
            continue;
        }
        
        unsigned sum_results = 0;
        for (unsigned i = l_indices; i <= r_indices; i++) 
        {
            if (!prime_numbers[Data[i]])
                sum_results++;
        }
        std::cout << sum_results << std::endl;
    }
    delete[]prime_numbers;
    return 0;
}
