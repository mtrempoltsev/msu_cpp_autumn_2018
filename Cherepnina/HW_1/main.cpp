#include "numbers.dat"
#include <cstdlib>
#include <iostream>
#include <cmath>

bool isPrime(const int digit) {
    if (digit == 1)
        return false;
    for (int i = 2; i <= sqrt(digit); i++) {
        int a = i;
        int b = digit;
        int t;
        while (b != 0) {
            t = b;
            b = a % b;
            a = t;
        }
        if (a > 1)
            return false;
    }
    return true;
}


int main(int argc, char *argv[]) {

    if ((argc % 2 != 1)||(argc == 1)) {
//        std::cout << "incorrect input" << std::endl;
        exit(-1);
    }

    for (int j = 1; j < argc; j += 2) {
        int sum = 0;
        int begin;
        int end;
        try {
            begin = atoi(argv[j]);
            end = atoi(argv[j+1]);
        } catch (...) {
//            std::cout << "incorrect input" << std::endl;
            exit(-1);
        }

        //go to required part of an array
        int i = 0;
        for (i; Data[i] < begin; i++);
        int last = Data[i];
        bool last_is_prime = false;

        if (isPrime(last)) {
            sum++;
            last_is_prime = true;
        }

        for (++i; Data[i] <= end; i++) {
            // for optimization
            if (last != Data[i]) {
                last = Data[i];
                last_is_prime = isPrime(Data[i]);
            }

            sum += last_is_prime;
        }
        std::cout << sum << std::endl;
    }


    return 0;
}