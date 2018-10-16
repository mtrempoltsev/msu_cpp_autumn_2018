#include "numbers.dat"

#include <iostream>
#include <math.h>

int
prime_num(int x){
    if (x < 2){
        return 0;
    }
    for (int i = 2; i * i <= x; ++i){
        if (x % i == 0){
            return 0;
        }
    }
    return 1;
}

int
find(int left_num, int right_num){
    int left_ind = -1, right_ind = -1;
    for (int i = 0; i < Size && left_ind == -1; ++i){
        if (Data[i] == left_num){
            left_ind = i;
        }
    }
    if (left_ind == -1){
        return 0;
    }
    for (int i = Size - 1; i >= 0 && right_ind == -1; --i){
        if (Data[i] == right_num){
            right_ind = i;
        }
    }
    if (right_ind == -1){
        return 0;
    }
    int count = 0;
    int last_pr = 2, last_npr = 0;
    for (int i = left_ind; i <= right_ind; ++i){
        if (Data[i] == last_pr){
                ++count;
        } else if (Data[i] != last_npr){
            if (prime_num(Data[i])){
                ++count;
                last_pr = Data[i];
            } else {
                last_npr = Data[i];
            }
        }
    }
    return count;
}

int
main(int argc, char *argv[]){
    if (argc == 1 || (argc - 1) % 2){
        return -1;
    }
    for (int i = 1; i < argc; i += 2){
        int left_num = atoi(argv[i]);
        int right_num = atoi(argv[i+1]);
        if (left_num > right_num){
            std::cout << 0 << std::endl;
        } else {
            std::cout << find(left_num, right_num) << std::endl;
        }
    }
    return 0;
}
