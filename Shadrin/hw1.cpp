#include <iostream>
#include <algorithm>

#include "numbers.dat"


bool is_prime(int a) {
    if (a == 1) {
        return false;
    }

    for (int i = 2; i * i <= a; ++i) {
        if (a % i == 0) {
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    const int *beg = Data;
    const int *end = &Data[Size + 1];

    if (argc < 3 || !(argc & 1)) {
        return -1;
    }

    for (int i = 1; i < argc - 1; i += 2) {
        int res = 0;
        int f = std::atoi(argv[i]);
        int s = std::atoi(argv[i + 1]);
        if (!(0 <= f && f <= 100000) ||
                !(0 <= s && s <= 100000)) {
            return -1;
        }
        
        if (f > s) {
            std::cout << 0 << std::endl;
            return 0;
        }

        int *ff = const_cast<int*>(std::lower_bound(beg, end, f));
        int *ss = const_cast<int*>(std::upper_bound(beg, end, s));

        if (ff == end || ss == end) {
            std::cout << 0 << std::endl;
        } else {
            for (int *ptr = ff; ptr != ss; ++ptr) {
                res += is_prime(*ptr);
            }
        }
        std::cout << res << std::endl;
    }

    return 0;
}
