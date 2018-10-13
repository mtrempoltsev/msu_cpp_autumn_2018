#include<iostream>
#include<cstdlib>

#include "numbers.dat"

bool is_prime (int number) {
    if (number == 1) return 0;
    for (int i=2; i*i<=number; ++i) {
        if (number % i == 0) return 0;
    }

    return 1;
}


int main(int argc, char* argv[])
{
    if (argc == 1) return -1;
    if (argc % 2 == 0) return -1;

    for (int i = 1; i <= (argc-1)/2; ++i)
    {
        int left= std::atoi(argv[2*i-1]);
        int right= std::atoi(argv[2*i]);
        
        if (right < left) {
            std::cout << 0 << "\n";
            continue;
        }

        int left_idx=-1,right_idx=-1;

        for (int j=0; j<Size; ++j) {
            if (Data[j] == left) {
                left_idx=j;
                break;
            }
        }
        if (left_idx == -1) {
            std::cout << 0 << "\n";
            continue;
        }
        for (int j=left_idx; j<Size; ++j) {
            if (Data[j] == right) {
                right_idx=j;
            }
            if (Data[j] > right) break;
        }
        if (right_idx == -1) {
            std::cout << 0 << "\n";
            continue;
        }
        int sum=0;
        for (int j=left_idx; j<=right_idx; ++j) {
            sum+=is_prime(Data[j]);
        }
        std::cout << sum << "\n";
    }

    return 0;
}
