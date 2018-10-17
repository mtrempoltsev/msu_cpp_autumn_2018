#include <iostream>
#include "numbers.dat"
#include <algorithm>

void find_simp_nums(bool* p, const size_t arraySize) {
    p[0] = 0;
    p[1] = 0;
    for (int i = 2; i <= arraySize; ++i) {
        p[i] = 1;
    }
    int i = 2;
    while (i * i <= arraySize) {
        for (int j = 2; j * i <= arraySize; ++j)
            p[i * j] = 0;
        ++i;
    }
}

int main(int argc, char* argv[]) {
    const size_t arraySize = 100000;
    bool p[arraySize];
    find_simp_nums(p, arraySize);
    int* a = (int*) malloc(sizeof(int) * (argc - 1));
    if (argc == 1) { return -1; }
    if (!(argc & 1)) { return -1; }
    for (int i = 1; i < argc; ++i) {
        if (sscanf(argv[i], "%d", &a[i - 1])) {} else { return -1; }
        if (a[i - 1] >= Data[0] && a[i - 1] <= Data[Size - 1]) {} else { return -1; }
    }
    for (int i = 0; i < argc - 1; i += 2) {
        int cnt = 0;
        if (a[i] > a[i + 1]) { std::cout << 0 << std::endl; continue; }
        const int *pt = std::lower_bound(Data, Data + Size, a[i]);
        if (*pt != a[i]) {
            std::cout << 0 << std::endl;
            continue;
        }
        while ((pt != Data + Size) && (*pt <= a[i + 1])) {
            if (p[*pt])
                ++cnt;
            ++pt;
        }
        if (*(--pt) != a[i + 1]) {
            std::cout << 0 << std::endl;
            continue;
        }
        std::cout << cnt << std::endl;
    }
    free(a);
    return 0;
}
