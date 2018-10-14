#include <iostream>
#include "numbers.dat"
#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <stdlib.h>

const int MAX_N = 1e5 + 1;

void
counting_prime_numbers(int * prime_numbers)
{
    prime_numbers[1] = 1;
    prime_numbers[0] = 1;
    for (int i = 2; i * i < MAX_N; ++i) {
        if (!prime_numbers[i]) {
            for (int j = i * i; j < MAX_N; j += i) {
                prime_numbers[j] = 1;
            }
        }
    }
}

void
counting_prefix_ans(int * prefix_ans, int * prime_numbers)
{
    for (int i = 0; i < Size; ++i) {
        if (i == 0 && !prime_numbers[Data[i]]) {
            prefix_ans[i] = 1;
        } else if (!prime_numbers[Data[i]]) {
            prefix_ans[i] = prefix_ans[i - 1] + 1;
        } else {
            prefix_ans[i] = prefix_ans[i - 1];
        }
    }
}

int
counting_ans(int position_first, int position_second, int * prefix_ans)
{
    int ans = 0;
    if (position_first <= position_second) {
        if (position_first > 0) {
            ans = prefix_ans[position_second] - prefix_ans[position_first - 1];
        } else {
            ans = prefix_ans[position_second];
        }
    }
    return ans;
}

int main(int argc, const char * argv[]) {
    if (argc % 2 != 1 || argc == 1) {
        return -1;
    }
    
    int *prime_numbers = new int[MAX_N];
    memset(prime_numbers, 0, MAX_N);
    counting_prime_numbers(prime_numbers);
    
    int *prefix_ans = new int[Size + 1];
    memset(prefix_ans, 0, Size + 1);
    counting_prefix_ans(prefix_ans, prime_numbers);
    
    for (int i = 1; i < argc; i += 2) {
        int first_value = std::atoi(argv[i]);
        int second_value = std::atoi(argv[i + 1]);
        int position_first = std::lower_bound(Data, Data + Size, first_value) - Data;
        int position_second = std::upper_bound(Data, Data + Size, second_value) - Data - 1;
        if (Data[position_first] != first_value || Data[position_second] != second_value) {
            printf("%d\n", 0);
            continue;
        }
        int ans = counting_ans(position_first, position_second, prefix_ans);
        printf("%d\n", ans);
    }
    delete[] prime_numbers;
    delete[] prefix_ans;
    return 0;
}
