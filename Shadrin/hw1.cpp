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

    constexpr int l_bound = 0;
    constexpr int r_bound = 1e5;
    constexpr int min_num_arg = 3;

    const int *beg = Data;
    const int *end = Data + Size + 1;

    if (argc < min_num_arg || !(argc & 1)) {
        return -1;
    }

    for (int i = 1; i < argc - 1; i += 2) {
        int res = 0;
        int f = std::atoi(argv[i]);
        int s = std::atoi(argv[i + 1]);
        if (!(l_bound <= f && f <= r_bound) ||
                !(l_bound <= s && s <= r_bound)) {
            return -1;
        }
        
        if (f > s) {
            std::cout << 0 << std::endl;
            return 0;
        }

        const int *ff = std::lower_bound(beg, end, f);
        const int *ss = std::upper_bound(beg, end, s);

        if (ff == end || ss == end) {
            std::cout << 0 << std::endl;
        } else {
            for (const int *ptr = ff; ptr != ss; ++ptr) {
                res += is_prime(*ptr);
            }
        }
        std::cout << res << std::endl;
    }

    return 0;
}
