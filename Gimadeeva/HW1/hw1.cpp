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

int find_left_idx (const int * Data, const int & Size, const int & left) {
    int left_idx=-1;
    for (int j=0; j<Size; ++j) {
        if (Data[j] == left) {
            left_idx=j;
            break;
        }
    }
    return left_idx;
}

int find_right_idx (const int * Data, const int & Size, const int & right, const int & left_idx) {
    int right_idx=-1;
    for (int j=left_idx; j<Size; ++j) {
        if ( Data[j] == right) {
            right_idx=j;
        }
        if (Data[j] > right) break;
    }
    return right_idx;
}

void sum_of_prime (const int * Data, const int & left_idx, const int & right_idx) {
    int sum=0;
    for (int j=left_idx; j<=right_idx; j++) {
        sum+=is_prime(Data[j]);
    }
    std::cout << sum << "\n";
}

int main(int argc, char* argv[])
{
    if (argc == 1) return -1;
    if (argc % 2 == 0) return -1;

    for (int i = 1; i <= (argc-1)/2; ++i)
    {
        int left = std::atoi(argv[2*i-1]);
        int right = std::atoi(argv[2*i]);
        
        if (right < left) {
            std::cout << 0 << "\n";
            continue;
        }

        int left_idx = find_left_idx (Data, Size, left);

        if (left_idx == -1) {
            std::cout << 0 << "\n";
            continue;
        }

        int right_idx = find_right_idx(Data, Size, right, left_idx);

        if (right_idx == -1) {
            std::cout << 0 << "\n";
            continue;
        }
        sum_of_prime (Data, left_idx, right_idx);
    }

    return 0;
}
