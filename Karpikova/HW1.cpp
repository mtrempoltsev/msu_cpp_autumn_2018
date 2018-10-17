#include <iostream>
#include <cstdlib>
#include "numbers.dat"

using namespace std;

int prime(int x)
{
    if (x == 1) {
        return 0;
    }
    for (int i = 2; i * i <= x; i++) {
        if (!(x % i)) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char **argv)
{
    if (argc < 2 || !(argc & 1)) {
        return -1;
    }
    int *buf = (int *)calloc((Data[Size - 1] + 1), sizeof(int));
    for (int i = 0; i < Size; i++) {
        buf[Data[i]]++;
    }
    for (int i = 1; i < argc; i++) {
        int left = atoi(argv[i++]), right = atoi(argv[i]);
        int res = 0;
        if (left > right) {
            cout << res << endl;
            continue;
        }
        if (left <= Data[Size - 1] && buf[left] != 0 && right <= Data[Size - 1] && buf[right] != 0) {
            for (int i = left; i <= right; i++) {
                if (buf[i] && prime(i)) {
                    res += buf[i];
                }
            }
            cout << res << endl;
        }
    }
    free(buf);
    return 0;
}
