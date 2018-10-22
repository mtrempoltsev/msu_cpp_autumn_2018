#include<iostream>
#include "numbers.dat"
using namespace std;

bool is_prime(int number) {
    if (number <= 1) {
        return false;
    }
    else if (number <= 3) {
        return true;
    }
    else if (number % 2 == 0 || number % 3 == 0) {
        return false;
    }
    int i = 5;
    while (i * i <= number) {
        if (number % i == 0 || number % (i + 2) == 0) {
            return false;
        }
        i += 6;
    }
    return true;
}

int search_start_idx(const int data[], size_t data_size, int  number) {
    //бинарный поиск влево
    size_t l = 0, r = data_size - 1;
    while (l < r) {
        size_t m = l + (r - l) / 2;
        if (data[m] == number && data[m - 1] < number) {
            return m;
        } else if (data[m] < number) {
            l = m + 1;
        } else {
            r = m;
        }
    }
    if (data[l] == number) return l;
    return -1;
}

int search_finish_idx(const int data[], size_t data_size, int number) {
    //бинарный поиск вправо
    size_t l = 0, r = data_size - 1;
    while (r - l > 1) {
        size_t m = l + (r - l) / 2;
        if (data[m] == number && data[m + 1] > number) {
            return m;
        } else if (data[m] > number) {
            r = m - 1;
        } else {
            l = m;
        }
    }
    if (data[l] == number) return l;
    if (data[r] == number) return l;
    return -1;
}

int count_primes(const int data[], size_t data_size, int minNum, int maxNum) {
    int startIdx = search_start_idx(data, data_size, minNum);
    int finishIdx = search_finish_idx(data, data_size, maxNum);
    int result = 0;
    if (startIdx == -1 || finishIdx == -1) return result;
    for (int i = startIdx; i <= finishIdx; i++) {
        if (is_prime(data[i])) result++;
    }
    return result;
}

int main(int argc, char const *argv[]) {
    if (argc % 2 != 1) return -1;
    for (int i = 1; i < argc; i += 2) {
        int lhs = atoi(argv[i]), rhs = atoi(argv[i + 1]);
        cout << count_primes(Data, Size, lhs, rhs) << endl;
    }
    return 0;
}