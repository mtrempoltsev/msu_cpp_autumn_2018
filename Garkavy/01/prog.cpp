#include <iostream>
#include <stdlib.h>
#include "numbers.dat"

// Data - массив
// Size - размер массива

int binsearch(const int a[], size_t first, size_t last, int x)
// искать в диапазоне индексов [first, last]
{
    if(x < a[0] || x > a[last]) {
        return -1;
    }
    last += 1;
    // теперь last означает индекс не последнего элемента, а следующего за ним
    while(first < last) {
        size_t mid = first + (last - first) / 2;
        if(x <= a[mid]) {
            last = mid;
        } else {
            first = mid + 1;
        }
    }
    if(a[last] == x) {
        return last;
    } else {
        return -1;
    }
}

bool is_prim(int n)
{
    static int last_in = 0;
    static bool last_res = false;
    // если функция вызвана несколько раз подряд с одним и тем же результатом,
    //     можно не вычислять ответ заново
    if(n <= 1) {
        return false;
    }
    if(n == last_in) {
        return last_res;
    }
    last_in = n;
    for(int i = 2; i * i <= n; ++i) {
        if(n % i == 0) {
            return (last_res = false);
        }
    }
    return (last_res = true);
}

int main(int argc, char* argv[])
{
    if(argc == 1) {
        return -1;
    } else if(argc % 2 == 0) {
        return -1;
    }
    for(int i = 1; i < argc; i += 2) {
        int first = std::atoi(argv[i]);
        int last = std::atoi(argv[i + 1]);
        if(first > last) {
            std::cout << "0\n";
            continue;
        }
        int last_idx = binsearch(Data, 0, Size - 1, last);
        if(last_idx < 0) {
            std::cout << "0\n";
            continue;
        }
        int first_idx = binsearch(Data, 0, last_idx, first);
        if(first_idx < 0) {
            std::cout << "0\n";
            continue;
        }
        while(last_idx < Size - 1 && Data[last_idx + 1] == last) {
            ++last_idx;
        }
        while(first_idx > 0 && Data[first_idx - 1] == last) {
            --first_idx;
        }
        int prim_count = 0;
        for(int j = first_idx; j <= last_idx; ++j) {
            if(is_prim(Data[j])) {
                ++prim_count;
            }
        }
        std::cout << prim_count << std::endl;
    }
    return 0;
}
