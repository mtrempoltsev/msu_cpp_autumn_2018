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

int findLeftBoard(const int *Data, int Size, int left) {

    int left_index = -1;

    for (int k = 0; k < Size; ++k) {
        if (Data[k] == left) {
            left_index = k;
            break;
        }
    }

    return left_index;

}

int findRightBoard(const int *Data, int Size, int right) {

    int left_index = -1;

    for (int k = Size-1; k >= 0; --k) {
        if (Data[k] == right) {
            left_index = k;
            break;
        }
    }

    return left_index;

}

int calculateSum(const int *Data, int left, int right) {

    int sum = 0;

    for (int k = left; k <= right; ++k) {
        if (isPrime(Data[k])) ++sum;
    }

    return sum;

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

        int left_index = findLeftBoard(Data, Size, left);


        if (left_index == -1) {
            std::cout << 0 << "\n";
            continue;
        }

        int right_index = findRightBoard(Data, Size, right);

        if (right_index == -1) {
            std::cout << 0 << "\n";
            continue;
        }

        int sum = calculateSum(Data, left_index, right_index);

        std::cout << sum << "\n";

    }

    return 0;
}
