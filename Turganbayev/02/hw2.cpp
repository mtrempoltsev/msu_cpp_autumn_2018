#include<iostream>
#include <memory>
#include "numbers.dat"
using namespace std;

int GetMaxNum(const unique_ptr<unique_ptr<int []> []>& pairs, size_t len) {
    int max = pairs[0][1];
    for (size_t i = 0; i < len; i++) {
        if (max < pairs[i][1]) max = pairs[i][1];
    }
    return max;
}

unique_ptr<bool []> IsPrime(size_t maxNum) {
    //решето Эратосфена
    unique_ptr<bool []> result(new bool[maxNum + 1]);
    for (size_t i = 0; i < maxNum + 1; i++) {
        result[i] = true;
    }
    result[0] = result[1] = false;
    for (size_t i=2; i <= maxNum; ++i)
        if (result[i])
            if (i * 1ll * i <= maxNum)
                for (size_t j=i*i; j <= maxNum; j+=i)
                    result[j] = false;
    return result;
}

int SearchStartIdx(const int data[],int  number, size_t size) {
    //бинарный поиск влево
    size_t l = 0, r = size - 1;
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

int SearchFinishIdx(const int data[], int number, size_t size) {
    //бинарный поиск вправо
    size_t l = 0, r = size - 1;
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

int CountPrimes(const int data[], size_t data_size,
                const unique_ptr<bool []>& isPrime, int minNum, int maxNum) {
    int startIdx = SearchStartIdx(data, minNum, data_size);
    int finishIdx = SearchFinishIdx(data, maxNum, data_size);
    int result = 0;
    if (startIdx == -1 || finishIdx == -1) return result;
    for (int i = startIdx; i <= finishIdx; i++) {
        if (isPrime[data[i]]) result++;
    }
    return result;
}

unique_ptr<unique_ptr<int []> []> GetPairs(int argc, char const *argv[]) {
    if ((argc - 1) % 2 != 0 || argc == 1) {
        throw invalid_argument("invalid number of arguments");
    }
    unique_ptr<unique_ptr<int []> []> result(new unique_ptr<int []>[argc / 2]);
    for (int i = 1; i < argc; i += 2) {
        int lhs = atoi(argv[i]), rhs = atoi(argv[i + 1]);
        auto pair = unique_ptr<int []>(new int[2]);
        pair[0] = lhs;
        pair[1] = rhs;
        result[(i - 1) / 2] = move(pair);
    }
    return result;
}

int main(int argc, char const *argv[]) {
    unique_ptr<unique_ptr<int []> []>  pairs = nullptr;
    try {
        pairs = GetPairs(argc, argv);
    } catch (const exception& ex) {
        return -1;
    }
    size_t pairs_count = argc / 2;
    int MaxNum = GetMaxNum(pairs, pairs_count);
    auto primes = IsPrime(MaxNum);
    for (size_t i = 0; i < pairs_count; i++) {
        cout << CountPrimes(Data, Size, primes, pairs[i][0], pairs[i][1]) << endl;
    }
    return 0;
}