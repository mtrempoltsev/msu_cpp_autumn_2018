#include "numbers.dat"
#include <iostream>
#include <algorithm>


int is_prime(int n) {
    if (n < 2) {
        return 0;
    }
	for (int i = 2; i * i <= n; ++i) {
		if (n % i == 0) {
			return 0;
		}
	}
	return 1;
}

int main(int argc, char* argv[])
{
    if (argc < 2 || argc % 2 == 0) {
        return -1;
    }
    for (int i = 1; i + 1 < argc; i += 2)
    {
        int l = std::atoi(argv[i]);
        int r = std::atoi(argv[i + 1]);

        if (l > r) {
            std::cout << 0 << std::endl;
            continue;
        }

        l = (std::lower_bound(Data, Data + Size, l)) - Data;
        r = (std::upper_bound(Data, Data + Size, r)) - Data;

        int cur = l, sum = 0;
        while (cur < r) {
            int cnt = 1;
            while (cur + 1 < r && Data[cur] == Data[cur + 1]) {
                ++cnt; ++cur;
            }
        	sum += cnt * is_prime(Data[cur]);
        	++cur;
        }
        std::cout << sum << std::endl;
    }
    return 0;
}