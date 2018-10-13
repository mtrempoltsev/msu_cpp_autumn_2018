#include "numbers.dat"

#include <iostream>
#include <vector>
#include <algorithm>

bool prime(const int n) {
    if (n < 2) {
        return false;
    }

    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    using std::cout;

    std::vector<int> dp(Size + 1);

    for (int i = 0; i < Size; ++i) {
        dp[i + 1] = dp[i] + prime(Data[i]);
    }

    if (!(argc & 1) || argc == 1) {
        return -1;
    }

    for (int i = 1; i < argc; i += 2) {
        int left = std::atoi(argv[i]);
        int right = std::atoi(argv[i + 1]);

        if (left > right) {
            cout << 0;
        } else {
            const auto first = std::lower_bound(Data, Data + Size, left);
            const auto last = std::upper_bound(Data, Data + Size, right);

            if (first == Data + Size || *first != left) {
                cout << 0;
            } else if (last == Data || *(last - 1) != right) {
                cout << 0;
            } else {
                cout << dp[last - Data] - dp[first - Data];
            }
        }
        cout << '\n';
    }
}
