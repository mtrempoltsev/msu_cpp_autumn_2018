#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "numbers.dat"

bool isPrime(int n) {

    if (n == 1) { return false; }

    for (int i = 2; i <= sqrt(n); ++i) {
        if (n % i == 0) { return false; }
    }

    return true;

}

int main(int argc, char* argv[]) {

    if (argc == 1) { return -1; }

    if (argc % 2 == 0) { return -1; }

    for (int i = 1; i <= (argc - 1) / 2; ++i) {

        int left = atoi(argv[2 * i - 1]);
        int right = atoi(argv[2 * i]);

        if (left > right) {
            std::cout << 0 << "\n";
            continue;
        }

        int left_index = -1;

        for (int k = 0; k < Size; ++k) {
            if (Data[k] == left) {
                left_index = k;
                break;
            }
        }

        if (left_index == -1) {
            std::cout << 0 << "\n";
            continue;
        }

        int right_index = -1;

        for (int k = Size - 1; k >= 0; --k) {
            if(Data[k] == right) {
                right_index = k;
                break;
            }
        }

        if (right_index == -1) {
            std::cout << 0 << "\n";
            continue;
        }

        int sum = 0;

        for (int k = left_index; k <= right_index; ++k) {
            if (isPrime(Data[k])) ++sum;
        }

        std::cout << sum << "\n";

    }

    return 0;
}
