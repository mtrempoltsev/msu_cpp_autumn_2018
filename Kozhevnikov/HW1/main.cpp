#include <iostream>
#include <cmath>
#include "numbers.dat"


//check number for primary
bool is_prime(int n) {
    if (((n % 2) == 0) && (n != 2)) {
        return false;
    }
    if (n == 1) {
        return false;
    }
    for (int i = 3; i < (sqrt(n) + 1); i += 2) {
        if ((n % i) == 0) {
            return false;
        }
    }
    return true;
}

//check for prime numbers in one interval
int interval_check(const int a[], const int len, int left, int right) {
    int left_i = -1, right_i = -1;
    for (int i = 0; i < len; i++) { //find indexes of left and right borders
        if ((left_i == -1) && (a[i] == left)) {
            left_i = i;
        }
        if ((a[i] == right)) {
            right_i = i;
        }
    }

    int counter = 0;
    for (int i = left_i; i <= right_i; i++) { //count prime numbers in existing interval
        if (is_prime(a[i])) {
            counter++;
        }
    }
    return counter;
}

int main(int argc, char* argv[]) {
    if (((argc % 2) == 0) || (argc == 1)) { //check for invalid command line parameters
        return -1;
    }
    int left, right;
    for (int i = 1; i < argc - 1; i += 2) {
        left = std::atoi(argv[i]);
        right = std::atoi(argv[i + 1]);
        std::cout << interval_check(Data, Size, left, right) << std::endl;
    }
    return 0;
}
