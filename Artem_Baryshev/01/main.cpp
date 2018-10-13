#include <iostream>
#include "numbers.dat"
#include <stdio.h>
#include <cmath>

const int MAX_N = 1e5 + 1;
int ans[MAX_N] = {0, };

int find_first(int value) {
    int left = 0, right = Size;
    while (right - left > 1) {
        int mid = (left + right) / 2;
        if (Data[mid] >= value) {
            right = mid;
        } else {
            left = mid;
        }
    }
    if (Data[left] == value) {
        right = left;
    }
    return right;
}

int find_second(int value) {
    int left = 0, right = Size;
    while (right - left > 1) {
        int mid = (left + right) / 2;
        if (Data[mid] <= value) {
            left = mid;
        } else {
            right = mid;
        }
    }
    return left;
}

bool IsPrime(int number) {
    if (!ans[number]) {
        return true;
    } else {
        return false;
    }
}

int main(int argc, const char * argv[]) {
    if (argc % 2 != 1 || argc == 1) {
        return -1;
    }
    
    ans[1] = 1;
    ans[0] = 1;
    for (int i = 2; i * i < MAX_N; ++i) {
        if (!ans[i]) {
            for (int j = i * i; j < MAX_N; j += i) {
                ans[j] = 1;
            }
        }
    }
    
    for (int i = 1; i < argc; i += 2) {
        int first_value = std::atoi(argv[i]);
        int second_value = std::atoi(argv[i + 1]);
        int position_first = find_first(first_value);
        int position_second = find_second(second_value);
        if (Data[position_first] != first_value || Data[position_second] != second_value) {
            printf("%d\n", 0);
            continue;
        }
        int ans = 0;
        for (int i = position_first; i <= position_second; ++i) {
            if (IsPrime(Data[i])) {
                ans++;
            }
        }
        printf("%d\n", ans);
    }
    return 0;
}
