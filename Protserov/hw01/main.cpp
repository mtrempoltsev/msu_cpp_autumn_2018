#include <iostream>
#include <algorithm>
#include "numbers.dat"

bool isprime(int);

int main(int argc, char* argv[])
{
    int a = 0, b = 0, cnt = 0;
    int* begin_pos = nullptr, *end_pos = nullptr;
    int* Data_begin = const_cast<int*>(Data);
    int* Data_end = const_cast<int*>(Data + Size + 1);
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
        begin_pos = const_cast<int*>(std::lower_bound(Data_begin, Data_end, a));
        end_pos = const_cast<int*>(std::upper_bound(Data_begin, Data_end, b));
        if ((end_pos == Data_end) || (begin_pos == Data_end)) {
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
