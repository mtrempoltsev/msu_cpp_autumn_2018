#include <iostream>
#include "numbers.dat"
#include <stdio.h>
#include <algorithm>
#include <cmath>

const int MAX_N = 1e5 + 1;

int main(int argc, const char * argv[]) {
    if (argc % 2 != 1 || argc == 1) {
        return -1;
    }
    
    int prime_numbers[MAX_N] = {0, };
    prime_numbers[1] = 1;
    prime_numbers[0] = 1;
    for (int i = 2; i * i < MAX_N; ++i) {
        if (!prime_numbers[i]) {
            for (int j = i * i; j < MAX_N; j += i) {
                prime_numbers[j] = 1;
            }
        }
    }
    
    int prefix_ans[Size + 1] = {0, };
    for (int i = 0; i < Size; ++i) {
        if (i == 0 && !prime_numbers[Data[i]]) {
            prefix_ans[i] = 1;
        } else if (!prime_numbers[Data[i]]) {
            prefix_ans[i] = prefix_ans[i - 1] + 1;
        } else {
            prefix_ans[i] = prefix_ans[i - 1];
        }
    }
    
    for (int i = 1; i < argc; i += 2) {
        int first_value = std::atoi(argv[i]);
        int second_value = std::atoi(argv[i + 1]);
        int position_first = std::lower_bound(Data, Data + Size, first_value) - Data;
        int position_second = std::upper_bound(Data, Data + Size, second_value) - Data - 1;
        if (Data[position_first] != first_value || Data[position_second] != second_value) {
            printf("%d\n", 0);
            continue;
        }
        int ans = 0;
        if (position_first <= position_second) {
            if (position_first > 0) {
                ans = prefix_ans[position_second] - prefix_ans[position_first - 1];
            } else {
                ans = prefix_ans[position_second];
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}
