#include <iostream>
#include "stdlib.h"
#include <cmath>
#include "numbers.dat"

using namespace std;

int define_current(int left) {
    int current = -1;
    for (int i = 0; i < Size; ++i) {
        if (Data[i] == left) {
            current = i;
            break;
        }    
        if (Data[i] > left) {
            std::cout << '0' << endl;
            exit(0);
        }
    }
    return current;
}

int is_number_prime(int number) {
    if (number == 1) {
        return 0;
    }
    for (int j = 2; j <= sqrt(number); ++j) {
        if (number % j == 0) {
            return 0;
        }
    }
    return 1;
}

int define_count_of_prime(int current, int right) {
    int count = 0;
    int last = -1;
    int is_last_number_prime = 0;
    for (int i = current; i < Size; ++i) {
        if (Data[i] > right) {
            if (last == right) {
                break;
            }
            std::cout << '0' << endl;
            exit(0);
        }
        if (Data[i] <= right) {
            last = Data[i];
            is_last_number_prime = is_number_prime(last);  
        } 
        if (is_last_number_prime) {
            ++count;
        }
    }  
    return count;
}

int main(int argc, char* argv[]) {
    if ((argc % 2 == 0) || (argc == 1)) {
        return -1;
    }
    for (int k = 0; 2 * k + 2 < argc; ++k) {
        int count, current;
        current = define_current(std::atoi(argv[1 + k * 2]));
        count = define_count_of_prime(current, std::atoi(argv[2 + k * 2]));
        std::cout << count << endl;  
    }
    return 0;
}
