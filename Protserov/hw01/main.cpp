#include <iostream>
#include <cstdlib>
#include "numbers.dat"

bool isprime(int);
int comp(const void*, const void*);

int main(int argc, char* argv[])
{
    int a = 0, b = 0, cnt = 0;
    int* begin_pos = nullptr, *end_pos = nullptr;
    if ((argc == 1) || (argc % 2 != 1)) {
        return -1;
    }
    for (unsigned i = 1; i < argc - 1; i += 2) {
        a = std::atoi(argv[i]);
        b = std::atoi(argv[i + 1]);
        if (a <= 0 || b <= 0) {
            return -1;
        }
        cnt = 0;
        begin_pos = (int*) std::bsearch(&a, Data, Size, sizeof(Data[0]), comp);
        end_pos = (int*) std::bsearch(&b, Data, Size, sizeof(Data[0]), comp);
        if (!begin_pos || !end_pos) {
            std::cout << 0 << std::endl;
        } else {
            for (int* myptr = begin_pos; myptr < end_pos; ++myptr) {
                if (isprime(*myptr)) {
                    ++cnt;
                }
            }
            std::cout << cnt << std::endl;
        }
    }
    return 0;
}

bool isprime(int num)
{
    if (num == 1) {
        return false;
    }
    if (num == 2) {
        return true;
    }
    if (num % 2 == 0) {
        return false;
    }
    for (int i = 3; i * i <= num; i += 2) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

int comp(const void* ap, const void* bp)
{
    const int* a = (int*) ap;
    const int* b = (int*) bp;
    if (*a < *b)
        return -1;
    else if (*a > *b)
        return 1;
    else
        return 0;
}
