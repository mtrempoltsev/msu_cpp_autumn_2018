#include <iostream>
#include <malloc.h>
#include "numbers.dat"
//можно оптимизировать не считая все 10000 а находя максимальный в массиве и считая соотвественно до него

int mysearch(int x, int y, int* a) {
    int answ = 0;
    int i = -1, j = -1;
    for (int k = 0; k < Size; ++k){
        if (Data[k] == x && i == -1) i = k;
        if (Data[k] == y) j = k;
    }
    if (i == -1 || j == -1) return 0;
    for (int k = i; k <= j; ++k){
        if (a[Data[k]]) answ++;
    }
    return answ;
}

void build_sieve_of_eratosthenes(int* sieve, int n) {
    std::fill_n(sieve, n + 1, true);
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i <= n; ++i) {
        if (sieve[i]) {
            for (int j = i*i; j <= n; j += i) {
                sieve[j] = false;
            }
        }
    }
}

int main(int argc, char* argv[])
{
    const int MAX_VALUE = 100000;
    int* sieve;
    try {
        sieve = (int*)malloc((MAX_VALUE + 1) * sizeof(int));
    } catch(...) {
        return -1;
    }
    try {
        build_sieve_of_eratosthenes(sieve, MAX_VALUE);
        if (argc % 2 == 0 || argc == 1) throw 0;
        for (int i = 1; i < argc; ++i)
        {
            int x = std::atoi(argv[i]);
            int y = std::atoi(argv[++i]);
            std::cout << mysearch(x, y, sieve) << std::endl;
        }
    } catch(...) {
        free(sieve);
        return -1;
    }
    free(sieve);
    return 0;
}
